import QtQuick
import QtQuick.Window

// minimum window for test
Window{
    visible: true
    width: 640 // to be reviewed
    height: 480 // to be reviewed
    title: qsTr("Fisoa")

    Text{
        anchors.centerIn: parent
        text: "Main window Fisoa"
        font.pixelSize: 24
    }
}
