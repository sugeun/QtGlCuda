#include "basicgl.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

BasicGl::BasicGl()
   : m_pShaderProgram(0)
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
   }

   m_pShaderProgram->bind();
   m_pShaderProgram->enableAttributeArray(0);

   float values[] = {
       -1, -1,
       1, -1,
       -1, 1
   };
   m_pShaderProgram->setAttributeArray(0, GL_FLOAT, values, 2);

   int w = this->width();
   int h = this->height();
   int x = this->x();
   int y = this->y();
   glViewport(x, y, w, h);

   glDisable(GL_DEPTH_TEST);

   glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

   m_pShaderProgram->disableAttributeArray(0);
   m_pShaderProgram->release();

}
