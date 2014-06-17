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

      //connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
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



void BasicGl::paint()
{
   if( m_pShaderProgram  == 0)
   {
      m_pShaderProgram = new QOpenGLShaderProgram();
      bool bSuccess
         = m_pShaderProgram->addShaderFromSourceFile( QOpenGLShader::Vertex,
                                                      ":/BasicGlVertex.vert");
      bSuccess
          = m_pShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                ":/BasicGlFragment.frag");

      m_pShaderProgram->bindAttributeLocation("vertices", 0);

      m_pShaderProgram->link();
      m_iMatrixUniform = m_pShaderProgram->uniformLocation("matrix");
   }

   m_pShaderProgram->bind();
   m_pShaderProgram->enableAttributeArray(0);
   QMatrix4x4 matrix;
   matrix.rotate( 360.0f * m_rRotate, 0, 1, 0);

   m_pShaderProgram->setUniformValue(m_iMatrixUniform, matrix);

   float values[] = {
      0.0f, 0.707f,
      -0.5f, -0.5f,
      0.5f, -0.5f
   };
   m_pShaderProgram->setAttributeArray(0, GL_FLOAT, values, 2);

   int w = this->width();
   int h = this->height();
   int x = this->x();
   int y = this->y();
   glViewport(x, y, w, h);

   glDisable(GL_DEPTH_TEST);

//   glClearColor(0, 0, 0, 1);
//   glClear(GL_COLOR_BUFFER_BIT);
   glEnable(GL_SCISSOR_TEST);
   glScissor(x, y, w, h);
   glClearColor(0, 0, 0, 1);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glDisable(GL_SCISSOR_TEST);

   glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

   m_pShaderProgram->disableAttributeArray(0);
   m_pShaderProgram->release();
}
