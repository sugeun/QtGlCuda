#ifndef BASICGL_H
#define BASICGL_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QVector3D>

class BasicGl : public QQuickItem
{
   Q_OBJECT

    Q_PROPERTY(qreal rotate READ rotate WRITE setRotate NOTIFY rotateChanged)

public:
   BasicGl();

   qreal rotate() const { return m_rRotate; }
   void setRotate(qreal rotate);

private:
   void init();
   QVector3D calculateSurfaceNormal(float* triangle);

signals:
   void rotateChanged();

public slots:
   void paint();

private slots:
   void handleWindowChanged(QQuickWindow* win);

private:
   QOpenGLShaderProgram* m_pShaderProgram;

   GLint m_lMvpMatrix;
   GLint m_lMvMatrix;
   GLint m_lNormalMatrix;
   GLint m_lLightPosition;
   qreal m_rRotate;

};

#endif // BASICGL_H
