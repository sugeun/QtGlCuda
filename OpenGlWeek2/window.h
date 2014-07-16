#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class basicGlWidget;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private slots:
    void setCurrentGlWidget();
    void rotateOneStep();

private:
    enum { NumRows = 1, NumColumns = 2 };

    basicGlWidget *glWidgets[NumRows][NumColumns];
    basicGlWidget *currentGlWidget;
};

#endif // WINDOW_H
