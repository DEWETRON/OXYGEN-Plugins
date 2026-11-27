// Details page of a Sample Gain channel (spec: GAIN-7, GAIN-8).
// Selected by the channel property "ODK/ChannelDetailsPage". Oxygen sets channel and readOnly.
import QtQuick 2.3
import QtQuick.Layouts 1.1
import Oxygen.ChannelTable 1.0
import Oxygen.Layouts 1.0
import Oxygen.Themes 1.0
import Oxygen.Widgets 1.0

Item
{
    id: root

    property var channel
    property bool readOnly: false

    ColumnLayout
    {
        anchors.fill: parent
        anchors.leftMargin: Theme.smallMargin
        anchors.rightMargin: Theme.smallMargin
        spacing: Theme.mediumSpacing

        Label
        {
            text: qsTranslate("ConfigKeys", "SAMPLE_GAIN/Gain")
        }

        ChannelPropertyEditor
        {
            Layout.fillWidth: true
            channel: root.channel
            key: "SAMPLE_GAIN/Gain"
            readOnly: root.readOnly
            onConfigItemChanged: root.channel.setConfigItem(key, value)
        }

        Label
        {
            text: qsTranslate("ConfigKeys", "SAMPLE_GAIN/Offset")
        }

        ChannelPropertyEditor
        {
            Layout.fillWidth: true
            channel: root.channel
            key: "SAMPLE_GAIN/Offset"
            readOnly: root.readOnly
            onConfigItemChanged: root.channel.setConfigItem(key, value)
        }

        VerticalSpacer {}
    }
}
