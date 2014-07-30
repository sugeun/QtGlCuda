#include "window.h"

#include "basicglwidget.h"

Window::Window()
{
    QGridLayout *mainLayout = new QGridLayout;

    for (int i = 0; i < NumRows; ++i) {
        for (int j = 0; j < NumColumns; ++j) {
            QColor clearColor;
            clearColor.setBlue(0);
            clearColor.setRed(0);
            clearColor.setGreen(0);

            glWidgets[i][j] = new basicGlWidget(0, 0);
            glWidgets[i][j]->setClearColor(clearColor);
            mainLayout->addWidget(glWidgets[i][j], i, j);

        }
    }
    setLayout(mainLayout);

    currentGlWidget = glWidgets[0][0];

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(rotateOneStep()));
    timer->start(20);

    setWindowTitle(tr("Textures"));
}

void Window::setCurrentGlWidget()
{
    currentGlWidget = qobject_cast<basicGlWidget *>(sender());
}

void Window::rotateOneStep()
{
    if (currentGlWidget)
        currentGlWidget->rotateBy(+2 * 16, +2 * 16, -1 * 16);
}
