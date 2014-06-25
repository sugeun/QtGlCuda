TEMPLATE = app

QT += qml quick widgets opengl widgets

SOURCES += main.cpp \
    basicgl.cpp \
    basicglwidget.cpp \
    window.cpp

RESOURCES += qml.qrc \
    shaders.qrc \
    textures.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    basicgl.h \
    basicglwidget.h \
    window.h

#OTHER_FILES += \
#    BasicGlVertex.vert \
#    BasicGlFragment.frag
