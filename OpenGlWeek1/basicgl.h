#ifndef BASICGL_H
#define BASICGL_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>

class BasicGl : public QQuickItem
{
   Q_OBJECT
public:
   BasicGl();

public slots:
   void paint();

private slots:
   void handleWindowChanged(QQuickWindow* win);

private:
   QOpenGLShaderProgram* m_pShaderProgram;

};

#endif // BASICGL_H
