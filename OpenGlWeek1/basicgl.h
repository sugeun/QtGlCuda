#ifndef BASICGL_H
#define BASICGL_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>

class BasicGl : public QQuickItem
{
   Q_OBJECT

    Q_PROPERTY(qreal rotate READ rotate WRITE setRotate NOTIFY rotateChanged)

public:
   BasicGl();

   qreal rotate() const { return m_rRotate; }
   void setRotate(qreal rotate);

signals:
   void rotateChanged();

public slots:
   void paint();

private slots:
   void handleWindowChanged(QQuickWindow* win);

private:
   QOpenGLShaderProgram* m_pShaderProgram;

   GLint m_iMatrixUniform;
   qreal m_rRotate;

};

#endif // BASICGL_H
