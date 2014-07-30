#include <QApplication>
#include <QtQuick/QQuickView>

#include "window.h"
#include "BasicGl.h"


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(shaders);

    QApplication app(argc, argv);

    Window window;
    window.show();


    return app.exec();
}
