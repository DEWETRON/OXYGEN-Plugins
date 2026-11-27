// Settings shown in Oxygen's "Add Channel" dialog (spec: GAIN-4, GAIN-7).
// The input channel is selected in the dialog's own channel list, not here.
// Interface: external/odk/docs/12_UIExtensions/UIExtensions.rst
import QtQuick 2.3
import QtQuick.Layouts 1.1
import Oxygen.Layouts 1.0
import Oxygen.Themes 1.0
import Oxygen.Widgets 1.0

Item
{
    id: root

    // Oxygen enables the "Add" button only if this is true
    readonly property bool settingsValid: isValidNumber(gainField.text) && isValidNumber(offsetField.text)

    // Number() rejects text such as "1,5" or "2x"; isFinite() rejects "Infinity"
    function isValidNumber(text)
    {
        return text.trim() !== "" && isFinite(Number(text));
    }

    // Oxygen passes these values to the plugin's init()
    function queryProperties()
    {
        var props = plugin.createPropertyList();
        props.setDouble("SAMPLE_GAIN/Gain", Number(gainField.text));
        props.setDouble("SAMPLE_GAIN/Offset", Number(offsetField.text));
        return props;
    }

    ColumnLayout
    {
        anchors.fill: parent
        anchors.leftMargin: Theme.smallMargin
        anchors.rightMargin: Theme.smallMargin
        spacing: Theme.mediumSpacing

        // GAIN-7: this page cannot see the selection, so it cannot disable "Add"
        Label
        {
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
            text: qsTranslate("SAMPLE_GAIN/AddChannel",
                              "Select exactly one input channel. Otherwise, Add creates nothing.")
        }

        Label
        {
            text: qsTranslate("SAMPLE_GAIN/AddChannel", "Gain")
        }

        TextField
        {
            id: gainField
            Layout.fillWidth: true
            text: "1"
        }

        Label
        {
            text: qsTranslate("SAMPLE_GAIN/AddChannel", "Offset (in the unit of the input)")
        }

        TextField
        {
            id: offsetField
            Layout.fillWidth: true
            text: "0"
        }

        Label
        {
            text: qsTranslate("SAMPLE_GAIN/AddChannel", "Enter numbers with a decimal point")
            visible: !root.settingsValid
            color: Theme.error
        }

        VerticalSpacer {}
    }
}
