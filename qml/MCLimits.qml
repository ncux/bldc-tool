import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

BasicPage {
    id:rootMotorConfigLimit
    showTopbar: true
    showBackButton: true
    pageTitle: "Motor Configuration(limits)"

    property int headingTopMargin:4
    property int headingLeftMargin:6
    property int rowVerticalMargin:6
    property int rowLeftMargin:16
    property int rowContentSpacing: 10
    property int rectGap: 20

    Binding{
        target:rootMotorConfigLimit
        property: "pageComponent"
        value:mainComponent
        when:true
    }

    property Component mainComponent: Rectangle {
        id: rootRect
        color: "#DCDCDC"
        Flickable{
            id: flickableMain
            width: parent.width
            height: parent.height
            contentHeight: rectMain.height
            contentWidth: width
            Rectangle{
                id:rectMain
                anchors.fill: parent
                color: "#DCDCDC"
                height: rectMotortype.height + rectCurrentLimit.height + rectRPMLimit.height + rectTempLimit.height + rectOtherLimit.height  + rectCurrentControl.height + rectBottomButtons.height + (rectGap*7)

                Rectangle{
                    id: rectMotortype
                    width: parent.width
                    anchors.top: parent.top
                    height: textMotorTypeHeading.height + headingTopMargin + rowMotorTypes.height + rowVerticalMargin
                    color: parent.color

                    Text{
                        id:textMotorTypeHeading
                        text: "Motor Type"
                        anchors.top: parent.top
                        anchors.topMargin: headingTopMargin
                        anchors.left: parent.left
                        anchors.leftMargin: headingLeftMargin

                    }

                    Row{
                        id:rowMotorTypes
                        anchors.top: textMotorTypeHeading.bottom
                        anchors.topMargin: rowVerticalMargin
                        anchors.left: parent.left
                        anchors.leftMargin: rowLeftMargin

                        ExclusiveGroup{
                            id :groupOptions
                        }

                        RadioButton{
                            id:rbBLDC
                            checked: true
                            text: "BLDC"
                            exclusiveGroup :groupOptions
                            style: RadioButtonStyle{
                                label: Text {
                                    renderType: Text.NativeRendering
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: "Helvetica"
                                    font.pointSize: 14
                                    maximumLineCount: 2
                                    wrapMode: Text.WordWrap
                                    text: control.text
                                    width: rectMotortype.width *0.25
                                }
                            }
                        }
                        RadioButton{
                            id:rbDC
                            checked: false
                            text: "DC"
                            exclusiveGroup :groupOptions
                            style: RadioButtonStyle{
                                label: Text {
                                    renderType: Text.NativeRendering
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: "Helvetica"
                                    font.pointSize: 14
                                    maximumLineCount: 2
                                    wrapMode: Text.WordWrap
                                    text: control.text
                                    width: rectMotortype.width *0.25
                                }
                            }
                        }
                    }
                }

                Rectangle{
                    id:rectCurrentLimit
                    width: parent.width
                    anchors.top: rectMotortype.bottom
                    anchors.topMargin: rectGap
                    height: textCurrentHeading.height + headingTopMargin + rectGridCurrent.height  + rowVerticalMargin
                    color: parent.color

                    Text{
                        id:textCurrentHeading
                        text: "Current Limits"
                        anchors.top: parent.top
                        anchors.topMargin: headingTopMargin
                        anchors.left: parent.left
                        anchors.leftMargin: headingLeftMargin

                    }

                    Rectangle{
                        id:rectGridCurrent
                        anchors.top: textCurrentHeading.bottom
                        anchors.topMargin: rowVerticalMargin
                        anchors.left: parent.left
                        anchors.leftMargin: rowLeftMargin
                        anchors.right : parent.right
                        anchors.rightMargin: rowLeftMargin
                        width:parent.width - (2 * rowLeftMargin)
                        color: parent.color
                        height: gridCurrent.height

                        Grid{
                            id : gridCurrent
                            columns: 2
                            columnSpacing: 6
                            rowSpacing: rowVerticalMargin * 0.6
                            width: parent.width

                            Text{
                                id:textMotor
                                text: "Motor max"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridCurrent.width * 0.73
                            }
                            TextField{
                                id:textFieldMotor
                                width: rectGridCurrent.width*0.25

                            }

                            Text{
                                id:textMotorMin
                                text: "Motor min (regen)"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridCurrent.width * 0.73
                            }
                            TextField{
                                id:textFieldMotorMin
                                width: rectGridCurrent.width*0.25

                            }

                            Text{
                                id:textBattMax
                                text: "Batt max"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridCurrent.width * 0.73
                            }
                            TextField{
                                id:textFieldBattMax
                                width: rectGridCurrent.width*0.25

                            }

                            Text{
                                id:textBattMin
                                text: "Batt min (regen)"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridCurrent.width * 0.73
                            }
                            TextField{
                                id:textFieldBattMin
                                width: rectGridCurrent.width*0.25

                            }

                            Text{
                                id:textAbsoluteMax
                                text: "Absolute max"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridCurrent.width * 0.73
                            }
                            TextField{
                                id:textFieldAbsoluteMax
                                width: rectGridCurrent.width*0.25

                            }

                            CheckBox{
                                id:cbSlowAbsMax
                                checked: false
                                text: "Slow absolute max"
                                style: CheckBoxStyle{
                                    label: Text {
                                        renderType: Text.NativeRendering
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: "Helvetica"
                                       // font.bold: true
                                        font.pointSize: 14
                                        text: control.text
                                    }
                                }
                            }

                        }

                    }



                }

                Rectangle{
                    id:rectRPMLimit
                    width: parent.width
                    anchors.top: rectCurrentLimit.bottom
                    anchors.topMargin: rectGap
                    height: textRPMHeading.height + headingTopMargin + rectGridRPM.height  + (2* rowVerticalMargin) + cbLimitERPM.height
                    color: parent.color

                    Text{
                        id:textRPMHeading
                        text: "RPM Limits"
                        anchors.top: parent.top
                        anchors.topMargin: headingTopMargin
                        anchors.left: parent.left
                        anchors.leftMargin: headingLeftMargin

                    }

                    CheckBox{
                        id:cbLimitERPM
                        checked: false
                        anchors.top: textRPMHeading.bottom
                        anchors.topMargin: rowVerticalMargin
                        anchors.left: parent.left
                        anchors.leftMargin: rowLeftMargin
                        text: "Limit ERPM with negative torque"
                        style: CheckBoxStyle{
                            label: Text {
                                renderType: Text.NativeRendering
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                font.family: "Helvetica"
                               // font.bold: true
                                font.pointSize: 14
                                text: control.text
                            }
                        }
                    }

                    Rectangle{
                        id:rectGridRPM
                        anchors.top: cbLimitERPM.bottom
                        anchors.topMargin: rowVerticalMargin
                        anchors.left: parent.left
                        anchors.leftMargin: rowLeftMargin
                        anchors.right : parent.right
                        anchors.rightMargin: rowLeftMargin
                        width:parent.width - (2 * rowLeftMargin)
                        color: parent.color
                        height: gridRPM.height

                        Grid{
                            id : gridRPM
                            columns: 2
                            columnSpacing: 6
                            rowSpacing: rowVerticalMargin * 0.6
                            width: parent.width

                            Text{
                                id:textMinERPM
                                text: "Min ERPM"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridRPM.width * 0.73                            }
                            TextField{
                                id:textFieldMinERPM
                                width: rectGridRPM.width*0.25

                            }

                            Text{
                                id:textMaxERPM
                                text: "Max ERPM"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridRPM.width * 0.73
                            }
                            TextField{
                                id:textFieldMaxERPM
                                width: rectGridRPM.width*0.25

                            }

                            Text{
                                id:textMaxERPMBrake
                                text: "Max ERPM at full brake"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridRPM.width * 0.73
                            }
                            TextField{
                                id:textFieldMaxERPMBrake
                                width: rectGridRPM.width*0.25

                            }

                            Text{
                                id:textMaxERPMBrakeCurr
                                text: "Max ERPM at full brake in current control mode"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridRPM.width * 0.73
                            }
                            TextField{
                                id:textFieldMaxERPMBrakeCurr
                                width: rectGridRPM.width*0.25

                            }


                        }

                    }



                }

                Rectangle{
                    id:rectTempLimit
                    width: parent.width
                    anchors.top: rectRPMLimit.bottom
                    anchors.topMargin: rectGap
                    height: textTempHeading.height + headingTopMargin + rectGridTemp.height  + rowVerticalMargin
                    color: parent.color

                    Text{
                        id:textTempHeading
                        text: "Temperature Limits"
                        anchors.top: parent.top
                        anchors.topMargin: headingTopMargin
                        anchors.left: parent.left
                        anchors.leftMargin: headingLeftMargin

                    }

                    Rectangle{
                        id:rectGridTemp
                        anchors.top: textTempHeading.bottom
                        anchors.topMargin: rowVerticalMargin
                        anchors.left: parent.left
                        anchors.leftMargin: rowLeftMargin
                        anchors.right : parent.right
                        anchors.rightMargin: rowLeftMargin
                        width:parent.width - (2 * rowLeftMargin)
                        color: parent.color
                        height: gridTemp.height

                        Grid{
                            id : gridTemp
                            columns: 2
                            columnSpacing: 6
                            rowSpacing: rowVerticalMargin * 0.6
                            width: parent.width

                            Text{
                                id:textMOSFETStart
                                text: "MOSFET Start"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridTemp.width * 0.73
                            }
                            TextField{
                                id:textFieldMOSFETStart
                                width: rectGridTemp.width*0.25

                            }

                            Text{
                                id:textMOSFETEnd
                                text: "MOSFET End"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridTemp.width * 0.73
                            }
                            TextField{
                                id:textFieldMOSFETEnd
                                width: rectGridTemp.width*0.25

                            }

                            Text{
                                id:textMotorStart
                                text: "Motor Start"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridTemp.width * 0.73
                            }
                            TextField{
                                id:textFieldMotorStart
                                width: rectGridTemp.width*0.25

                            }

                            Text{
                                id:textMotorEnd
                                text: "Motor End"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridTemp.width * 0.73
                            }
                            TextField{
                                id:textFieldMotorEnd
                                width: rectGridTemp.width*0.25

                            }


                        }

                    }



                }

                Rectangle{
                    id:rectOtherLimit
                    width: parent.width
                    anchors.top: rectTempLimit.bottom
                    anchors.topMargin: rectGap
                    height: textOtherHeading.height + headingTopMargin + rectGridOther.height  + rowVerticalMargin
                    color: parent.color

                    Text{
                        id:textOtherHeading
                        text: "Other Limits"
                        anchors.top: parent.top
                        anchors.topMargin: headingTopMargin
                        anchors.left: parent.left
                        anchors.leftMargin: headingLeftMargin

                    }

                    Rectangle{
                        id:rectGridOther
                        anchors.top: textOtherHeading.bottom
                        anchors.topMargin: rowVerticalMargin
                        anchors.left: parent.left
                        anchors.leftMargin: rowLeftMargin
                        anchors.right : parent.right
                        anchors.rightMargin: rowLeftMargin
                        width:parent.width - (2 * rowLeftMargin)
                        color: parent.color
                        height: gridOther.height

                        Grid{
                            id : gridOther
                            columns: 2
                            columnSpacing: 6
                            rowSpacing: rowVerticalMargin * 0.6
                            width: parent.width

                            Text{
                                id:textMinINV
                                text: "Minimum input voltage"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridOther.width * 0.73
                            }
                            TextField{
                                id:textFieldMinINV
                                width: rectGridOther.width*0.25

                            }

                            Text{
                                id:textMaxINV
                                text: "Maximum input voltage"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridOther.width * 0.73
                            }
                            TextField{
                                id:textFieldMaxINV
                                width: rectGridOther.width*0.25

                            }

                            Text{
                                id:textMinDC
                                text: "Minimum duty cycle"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridOther.width * 0.73
                            }
                            TextField{
                                id:textFieldMinDC
                                width: rectGridOther.width*0.25

                            }

                            Text{
                                id:textMaxDC
                                text: "Maximum duty cycle"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridOther.width * 0.73
                            }
                            TextField{
                                id:textFieldMaxDC
                                width: rectGridOther.width*0.25

                            }


                        }

                    }



                }

                Rectangle{
                    id:rectCurrentControl
                    width: parent.width
                    anchors.top: rectOtherLimit.bottom
                    anchors.topMargin: rectGap
                    height: textCurrentControlHeading.height + headingTopMargin + rectGridCurrControl.height  + rowVerticalMargin
                    color: parent.color

                    Text{
                        id:textCurrentControlHeading
                        text: "Current Control"
                        anchors.top: parent.top
                        anchors.topMargin: headingTopMargin
                        anchors.left: parent.left
                        anchors.leftMargin: headingLeftMargin

                    }

                    Rectangle{
                        id:rectGridCurrControl
                        anchors.top: textCurrentControlHeading.bottom
                        anchors.topMargin: rowVerticalMargin
                        anchors.left: parent.left
                        anchors.leftMargin: rowLeftMargin
                        anchors.right : parent.right
                        anchors.rightMargin: rowLeftMargin
                        width:parent.width - (2 * rowLeftMargin)
                        color: parent.color
                        height: gridCurrControl.height

                        Grid{
                            id : gridCurrControl
                            columns: 2
                            columnSpacing: 6
                            rowSpacing: rowVerticalMargin * 0.6
                            width: parent.width

                            Text{
                                id:textStartBoost
                                text: "Startup boost"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridCurrControl.width * 0.73
                            }
                            TextField{
                                id:textFieldStartBoost
                                width: rectGridCurrControl.width*0.25

                            }

                            Text{
                                id:textMinCurrent
                                text: "Min current"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridCurrControl.width * 0.73
                            }
                            TextField{
                                id:textFieldMinCurr
                                width: rectGridCurrControl.width*0.25

                            }

                            Text{
                                id:textContGain
                                text: "Control gain"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                maximumLineCount: 2
                                wrapMode: Text.WordWrap
                                width: rectGridCurrControl.width * 0.73
                            }
                            TextField{
                                id:textFieldContGain
                                width: rectGridCurrControl.width*0.25

                            }

                        }

                    }



                }

                Rectangle{

                    id:rectBottomButtons
                    width: parent.width
                    anchors.top: rectCurrentControl.bottom
                    anchors.topMargin: rectGap
                    height: buttonLoadXML.height + rectButtons.height +rowVerticalMargin
                    color: parent.color

                    Button
                    {
                        id:buttonLoadXML
                        anchors.left: parent.left
                        anchors.leftMargin: 12
                        anchors.top: parent.top
                        text:"Load XML"
                        onClicked: {
                            console.log("Load XML tapped")
                        }
                        style: ButtonStyle {
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

                    Rectangle{
                        id:rectButtons
                        width: rectMain.width - (2 * headingLeftMargin)
                        anchors.top: buttonLoadXML.bottom
                        anchors.topMargin: rowVerticalMargin
                        anchors.left: parent.left
                        anchors.leftMargin: headingLeftMargin
                        color: parent.color
                        height: childrenRect.height
                        Grid{
                            id:gridButtons
                            columns: 3
                            columnSpacing: headingLeftMargin/1.5

                            Button{
                                id:buttonReadConf
                                height: textfieldPath.height
                                text:"Read Config"
                                width: rectButtons.width * 0.31
                                onClicked: {

                                }
                                style: ButtonStyle {
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

                            Button{
                                id:buttonWriteConf
                                height: textfieldPath.height
                                text:"Write Config"
                                width: rectMain.width * 0.31
                                onClicked: {

                                }
                                style: ButtonStyle {
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

                            Button{
                                id:buttonReboot
                                height: textfieldPath.height
                                text:"Reboot"
                                width: rectMain.width * 0.31
                                onClicked: {

                                }
                                style: ButtonStyle {
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

                        }
                    }

                }

            }
        }
    }
}

