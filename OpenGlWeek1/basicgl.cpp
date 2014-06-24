#include "basicgl.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

BasicGl::BasicGl()
   : m_pShaderProgram(0)
   , m_rRotate(0.0f)
{
   connect( this,
            SIGNAL(windowChanged(QQuickWindow*)),
            this,
            SLOT(handleWindowChanged(QQuickWindow*)));
}


void BasicGl::handleWindowChanged(QQuickWindow* win)
{
   if( win )
   {
      connect(win, SIGNAL(beforeRendering()), this,
              SLOT(paint()), Qt::DirectConnection );

      win->setClearBeforeRendering(false);
   }
}


void BasicGl::setRotate(qreal rotate )
{
   if (rotate == m_rRotate)
       return;
   m_rRotate = rotate;
   emit rotateChanged();
   if (window())
       window()->update();
}



void BasicGl::init()
{
   if( m_pShaderProgram  != 0) return;

   m_pShaderProgram = new QOpenGLShaderProgram();
   bool bSuccess
      = m_pShaderProgram->addShaderFromSourceFile( QOpenGLShader::Vertex,
                                                   ":/BasicGlVertex.vert");
   bSuccess
       = m_pShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                             ":/BasicGlFragment.frag");

   m_pShaderProgram->bindAttributeLocation("vertices", 0);
   m_pShaderProgram->bindAttributeLocation("vNormal", 1);

   m_pShaderProgram->link();
   m_lMvpMatrix = m_pShaderProgram->uniformLocation("mvpMatrix");
   m_lMvMatrix = m_pShaderProgram->uniformLocation("mvMatrix");
   m_lNormalMatrix = m_pShaderProgram->uniformLocation("normalMatrix");
   m_lLightPosition = m_pShaderProgram->uniformLocation("vLightPosition");

}


void BasicGl::paint()
{
   init();
   m_pShaderProgram->bind();
   m_pShaderProgram->enableAttributeArray(0);


   float vEyeLight[] = { -1.0f, 1.0f, 1.0f };

   QMatrix4x4 mNormal;
   mNormal.normalMatrix();

   QMatrix4x4 matrix;
   matrix.rotate( 180.0f * m_rRotate, 0, 1, 0);
   m_pShaderProgram->setUniformValue(m_lMvMatrix, matrix);
   matrix.ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
   m_pShaderProgram->setUniformValue(m_lMvpMatrix, matrix);
   m_pShaderProgram->setUniformValue(m_lNormalMatrix, matrix.normalMatrix() );

   QMatrix3x3 normalMatrix = matrix.normalMatrix();

   float vertices[] = {
      0.0f, 0.707f, 0.0f,
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f
   };
   QVector3D surfaceNormal = calculateSurfaceNormal(vertices);
   m_pShaderProgram->setAttributeArray(0, GL_FLOAT, vertices, 3);
   m_pShaderProgram->setAttributeValue(1, surfaceNormal );

   int w = this->width();
   int h = this->height();
   int x = this->x();
   int y = window()->height() - (this->y() + h );
   glViewport(x, y, w, h);

   //glDisable(GL_DEPTH_TEST);

//   glClearColor(0, 0, 0, 1);
//   glClear(GL_COLOR_BUFFER_BIT);
   glEnable(GL_SCISSOR_TEST);
   glScissor(x, y, w, h);
   glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glDisable(GL_SCISSOR_TEST);

   glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

   m_pShaderProgram->disableAttributeArray(0);
   m_pShaderProgram->release();
}

QVector3D BasicGl::calculateSurfaceNormal(float* triangle)
{
   QVector3D normalVector;
   QVector3D uVector;
   QVector3D vVector;
   QVector3D tPoint[3];

   for( int i = 0 ; i < 3 ; i++)
   {
      tPoint[i].setX( qreal(triangle[i * 3 ]));
      tPoint[i].setY( qreal(triangle[i * 3 + 1]));
      tPoint[i].setZ( qreal(triangle[i * 3 + 2]));
   }

   uVector = tPoint[1] - tPoint[0];
   vVector = tPoint[2] - tPoint[0];

   normalVector = QVector3D::crossProduct(uVector,vVector);
   return normalVector;
}
