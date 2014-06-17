import QtQuick 2.2
import OpenGLUnderQML 1.0

Item {
    visible: true
    width: 640
    height: 480
   // title: qsTr("Hello World")


    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
    BasicGlTest{
        width: 100
        height: 100
        x: 300
        y: 300
    }

    BasicGlTest{
        width: 120
        height: 120
        x: 100
        y: 100
    }
}
