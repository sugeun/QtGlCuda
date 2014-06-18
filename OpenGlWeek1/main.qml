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
    Rectangle
    {
        x: 300
        y: 300
        width: 100
        height: 100
        color: "#0ff000"
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
        MouseArea {
            anchors.fill: parent
            drag.target: parent
            //drag.axis: Drag.XAxis

        }
    }

    BasicGlTest{
        width: 120
        height: 120
        x: 100
        y: 100
        SequentialAnimation on rotate {
            NumberAnimation { to: 1; duration: 4000;  }
            NumberAnimation { to: 0; duration: 2500;  }
            loops: Animation.Infinite
            running: true
        }
    }
}
