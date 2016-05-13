import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

RadioButton {
    property string firmwareUrl

    id: firmwareRadioButton

    checked: firmwareCurrentUrl === firmwareUrl
    onClicked: {
        firmwareCurrentUrl = firmwareUrl
    }

    style: RadioButtonStyle{
        label: Text {
            renderType: Text.NativeRendering
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: "Helvetica"
            font.pointSize: 12
            text: control.text
        }
    }
}
