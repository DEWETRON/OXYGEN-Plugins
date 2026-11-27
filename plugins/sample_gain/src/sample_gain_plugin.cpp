// Sample Gain: output = gain * input + offset. Rules: ../spec/gain.md
#include "odkfw_properties.h"
#include "odkfw_software_channel_plugin.h"
#include "odkapi_utils.h"

#include "gain.h"
#include "qml.rcc.h"

#include <algorithm>
#include <memory>
#include <vector>

namespace
{

// name:  unique plugin id, also the prefix of all config item keys
// uuid:  stored in setups to find the plugin again. Every new plugin needs its own.
const char* PLUGIN_MANIFEST =
R"XML(<?xml version="1.0"?>
<OxygenPlugin name="SAMPLE_GAIN" version="0.1" uuid="DA77C778-D968-4769-9BA1-8D07D1932BF0">
  <Info name="Sample Gain">
    <Vendor name="DEWETRON GmbH"/>
    <Description>Sample plugin: output = gain * input + offset</Description>
  </Info>
  <Host minimum_version="8.3"/>
  <UsesUIExtensions/>
</OxygenPlugin>
)XML";

const char* TRANSLATION_EN =
R"XML(<?xml version="1.0"?>
<TS version="2.1" language="en" sourcelanguage="en">
    <context><name>ConfigKeys</name>
        <message><source>SAMPLE_GAIN/InputChannel</source><translation>Input channel</translation></message>
        <message><source>SAMPLE_GAIN/Gain</source><translation>Gain</translation></message>
        <message><source>SAMPLE_GAIN/Offset</source><translation>Offset</translation></message>
    </context>
</TS>
)XML";

const char* KEY_INPUT_CHANNEL = "SAMPLE_GAIN/InputChannel";
const char* KEY_GAIN = "SAMPLE_GAIN/Gain";
const char* KEY_OFFSET = "SAMPLE_GAIN/Offset";
const char* KEY_DETAILS_PAGE = "ODK/ChannelDetailsPage";

using namespace odk::framework;

class SampleGainInstance : public SoftwareChannelInstance
{
public:
    SampleGainInstance()
        : m_input(std::make_shared<EditableChannelIDProperty>())
        , m_gain(std::make_shared<EditableFloatingPointProperty>(sample_gain::DEFAULT_GAIN))
        , m_offset(std::make_shared<EditableFloatingPointProperty>(sample_gain::DEFAULT_OFFSET))
    {
        m_input->setVisiblity("PUBLIC");
        m_gain->setVisiblity("PUBLIC");
        m_offset->setVisiblity("PUBLIC");
    }

    // How the plugin appears in the "Add Channel" dialog
    static odk::RegisterSoftwareChannel getSoftwareChannelInfo()
    {
        odk::RegisterSoftwareChannel telegram;
        telegram.m_display_name = "Sample Gain";
        telegram.m_service_name = "CreateSampleGain";
        telegram.m_display_group = "Samples";
        telegram.m_description = "Sample plugin: output = gain * input + offset";
        telegram.m_ui_item_add = "SampleGainAddChannel";
        telegram.m_analysis_capable = true;
        return telegram;
    }

    // Defines the output channel and its settings. Also runs before a setup is loaded.
    void create(odk::IfHost* host) override
    {
        ODK_UNUSED(host);

        getRootChannel()->setDefaultName("Sample Gain")
            .setSampleFormat(
                odk::ChannelDataformat::SampleOccurrence::SYNC,
                odk::ChannelDataformat::SampleFormat::DOUBLE,
                1)
            .setDeletable(true)
            .addProperty(KEY_INPUT_CHANNEL, m_input)
            .addProperty(KEY_GAIN, m_gain)
            .addProperty(KEY_OFFSET, m_offset)
            .addProperty(KEY_DETAILS_PAGE, odk::Property(KEY_DETAILS_PAGE, "SampleGainChannelDetails"));
    }

    // A new channel was added by the user (GAIN-1, GAIN-7)
    InitResult init(const InitParams& params) override
    {
        if (params.m_input_channels.size() != 1)
        {
            return { false };
        }
        m_input->setValue(params.m_input_channels.front().m_channel_id);

        const odk::PropertyList properties(params.m_properties);
        applySetting(*m_gain, properties, KEY_GAIN);
        applySetting(*m_offset, properties, KEY_OFFSET);

        InitResult result(true);
        result.showChannelDetails(getRootChannel()->getLocalId());
        return result;
    }

    // A saved setup is loaded (GAIN-9)
    bool configure(const odk::UpdateChannelsTelegram& request,
        std::map<std::uint32_t, std::uint32_t>& channel_id_map) override
    {
        configureFromTelegram(request, channel_id_map);
        return true;
    }

    // Runs after every configuration change. Returns whether the channel can compute.
    bool update() override
    {
        keepSettingValid(*m_gain, m_last_gain);
        keepSettingValid(*m_offset, m_last_offset);

        const auto input = getInput();
        if (!input || !input->updateDataFormat())
        {
            return false;
        }

        // GAIN-1: numeric scalar input only
        const auto format = input->getDataFormat();
        const bool is_sync = format.m_sample_occurrence == odk::ChannelDataformat::SampleOccurrence::SYNC;
        const bool is_async = format.m_sample_occurrence == odk::ChannelDataformat::SampleOccurrence::ASYNC;
        if (format.m_sample_value_type != odk::ChannelDataformat::SampleValueType::SAMPLE_VALUE_SCALAR
            || !(is_sync || is_async))
        {
            return false;
        }
        m_is_sync = is_sync;

        const auto channel = getRootChannel();
        channel->setSampleFormat(format.m_sample_occurrence, odk::ChannelDataformat::SampleFormat::DOUBLE, 1);
        applyTimebase(*input);

        // GAIN-6
        const std::string unit = input->getUnit();
        channel->setUnit(unit);

        // GAIN-10: the output range is the transformed input range
        const odk::Range input_range = input->getRange();
        const double first = sample_gain::apply(input_range.m_min, m_gain->getValue(), m_offset->getValue());
        const double second = sample_gain::apply(input_range.m_max, m_gain->getValue(), m_offset->getValue());
        double range_min = std::min(first, second);
        double range_max = std::max(first, second);
        if (range_min == range_max)
        {
            range_min -= 1.0;
            range_max += 1.0;
        }
        if (std::isfinite(range_min) && std::isfinite(range_max))
        {
            channel->setRange(odk::Range(range_min, range_max, unit, unit));
        }

        return true;
    }

    // Runs before acquisition starts, when the input timebase is known
    void initTimebases(odk::IfHost* host) override
    {
        ODK_UNUSED(host);
        if (const auto input = getInput())
        {
            applyTimebase(*input);
        }
    }

    // Runs repeatedly during acquisition (GAIN-2)
    void process(ProcessingContext& context, odk::IfHost* host) override
    {
        const auto channel = getRootChannel();
        if (!channel->getUsedProperty()->getValue())
        {
            return;
        }

        const auto found = context.m_channel_iterators.find(m_input->getValue());
        if (found == context.m_channel_iterators.end())
        {
            return;
        }
        auto& iterator = found->second;

        const double gain = m_gain->getValue();
        const double offset = m_offset->getValue();

        if (m_is_sync)
        {
            // A sync channel has one sample per tick: write the window as one block
            iterator.setSkipGaps(false);
            const auto start = odk::convertTimeToTickAtOrAfter(context.m_window.first, m_frequency);
            const auto end = odk::convertTimeToTickAtOrAfter(context.m_window.second, m_frequency);

            std::vector<double> output;
            output.reserve(static_cast<std::size_t>(end - start));
            for (auto tick = start; tick < end && iterator.valid(); ++tick, ++iterator)
            {
                output.push_back(sample_gain::apply(iterator.value<double>(), gain, offset));
            }
            if (!output.empty())
            {
                odk::addSamples(host, channel->getLocalId(), start, output.data(), output.size() * sizeof(double));
            }
        }
        else
        {
            // An async channel keeps the timestamp of every input sample
            for (; iterator.valid(); ++iterator)
            {
                const double value = sample_gain::apply(iterator.value<double>(), gain, offset);
                odk::addSample(host, channel->getLocalId(), iterator.timestamp(), value);
            }
        }
    }

private:
    InputChannelPtr getInput()
    {
        return m_input->isValid() ? getInputChannelProxy(m_input->getValue()) : InputChannelPtr();
    }

    void applyTimebase(InputChannel& input)
    {
        input.updateTimeBase();
        m_frequency = m_is_sync ? input.getSampleRate().m_val : input.getTimeBase().m_frequency;
        if (m_frequency > 0.0)
        {
            getRootChannel()->setSimpleTimebase(m_frequency);
        }
    }

    static void applySetting(EditableFloatingPointProperty& setting,
        const odk::PropertyList& properties, const char* key)
    {
        setting.setValue(sample_gain::acceptSetting(properties.getDouble(key), setting.getValue()));
    }

    // GAIN-4: the ODK property accepts NaN, so the plugin has to restore the last valid value
    static void keepSettingValid(EditableFloatingPointProperty& setting, double& last_valid)
    {
        last_valid = sample_gain::acceptSetting(setting.getValue(), last_valid);
        setting.setValue(last_valid);
    }

    std::shared_ptr<EditableChannelIDProperty> m_input;
    std::shared_ptr<EditableFloatingPointProperty> m_gain;
    std::shared_ptr<EditableFloatingPointProperty> m_offset;
    double m_last_gain = sample_gain::DEFAULT_GAIN;
    double m_last_offset = sample_gain::DEFAULT_OFFSET;
    double m_frequency = 0.0;
    bool m_is_sync = true;
};

class SampleGainPlugin : public SoftwareChannelPlugin<SampleGainInstance>
{
public:
    void registerResources() final
    {
        addTranslation(TRANSLATION_EN);
        addQtResources(plugin_resources::QML_RCC_data, plugin_resources::QML_RCC_size);
    }
};

} // namespace

OXY_REGISTER_PLUGIN1("SAMPLE_GAIN", PLUGIN_MANIFEST, SampleGainPlugin);
