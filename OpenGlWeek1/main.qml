import QtQuick 2.2
import OpenGLUnderQML 1.0

Item {
    visible: true
    width: 640
    height: 480
   // title: qsTr("Hello World")


    Text {
        color: "#00ff00"
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
    BasicGlTest{
        width: 200
        height: 200
        x: 300
        y: 300
        SequentialAnimation on rotate {
            NumberAnimation { to: 1; duration: 2500;  }
            NumberAnimation { to: 0; duration: 2500;  }
            loops: Animation.Infinite
            running: true
        }
    }

    BasicGlTest{
        width: 120
        height: 120
        x: 100
        y: 100
    }
}
