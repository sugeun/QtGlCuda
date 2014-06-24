import QtQuick 2.2
import OpenGLUnderQML 1.0

Item {
    visible: true
    width: 640
    height: 480
   // title: qsTr("Hello World")


    BasicGlTest{
        width: 400
        height: 400
        x: 0
        y: 0
        SequentialAnimation on rotate {
            NumberAnimation { to: 1; duration: 4500;  }
           // NumberAnimation { to: 0; duration: 2500;  }
            loops: Animation.Infinite
            running: true
        }
    }

}
