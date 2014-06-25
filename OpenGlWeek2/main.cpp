#include <QApplication>
#include <QtQuick/QQuickView>

#include "window.h"
#include "BasicGl.h"


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(shaders);
    //Q_INIT_RESOURCE(textures);

    QApplication app(argc, argv);

//    qmlRegisterType<BasicGl>("OpenGLUnderQML", 1, 0, "BasicGlTest");

//    QQuickView view;
//    view.setResizeMode(QQuickView::SizeRootObjectToView);
//    view.setSource(QUrl("qrc:///main.qml"));
//    view.show();

    Window window;
    window.show();


    return app.exec();
}
