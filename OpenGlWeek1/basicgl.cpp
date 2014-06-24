#include "basicgl.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QtOpenGL/qgl.h>

BasicGl::BasicGl()
   : m_pShaderProgram(0)
   , m_rRotate(0.0f)
   , m_iSphereRes( 10 )
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
   m_lTextureUniform = m_pShaderProgram->uniformLocation("sphereTexmap");

   initSphereVertices();
   initSphereTexture();
}

void BasicGl::initSphereVertices()
{
    m_iNumVertices = m_iSphereRes * (m_iSphereRes - 1 ) * 2;

    m_fVertices = new float[m_iNumVertices * 2];

    float delta = 1.0f / float(m_iSphereRes);

    int vIdx = 0;
    for( int j = 0 ; j < m_iSphereRes - 1 ; j++ )
    {
        for( int i = 0 ; i < m_iSphereRes ; i++ )
        {
            float x, y1, y2;
            x = float(i) * delta;
            y1 = 1.0f - float(j) * delta;
            y2 = 1.0f - float(j+1) * delta;
            m_fVertices[vIdx++] = x;
            m_fVertices[vIdx++] = y1;
            m_fVertices[vIdx++] = x;
            m_fVertices[vIdx++] = y2;

        }
    }
}

bool BasicGl::initSphereTexture()
{

    GLbyte* pBits;
    int nWidth, nHeight, nComponents;
    GLenum eFormat;
    QPixmap texturePixmap(QString(":/Marble.png"));
    nWidth = texturePixmap.width();
    nHeight = texturePixmap.height();

    pBits = (GLbyte*) texturePixmap.toImage().bits();

    glGenTextures(1, m_iTexture);

    glBindTexture(GL_TEXTURE_2D, m_iTexture[0]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, nWidth, nHeight, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, pBits);

    return true;
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
   matrix.rotate( 360.0f * m_rRotate, 0, 1, 0);
   m_pShaderProgram->setUniformValue(m_lMvMatrix, matrix);
   matrix.ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);
   m_pShaderProgram->setUniformValue(m_lMvpMatrix, matrix);
   m_pShaderProgram->setUniformValue(m_lNormalMatrix, matrix.normalMatrix() );

   //m_pShaderProgram->setUniformValue(m_lTextureUniform, matrix);

   QMatrix3x3 normalMatrix = matrix.normalMatrix();

   QVector3D surfaceNormal;// = calculateSurfaceNormal(m_fVertices);
   m_pShaderProgram->setAttributeArray(0, GL_FLOAT, m_fVertices, 2);

   m_pShaderProgram->setAttributeValue(1, surfaceNormal );

   int w = this->width();
   int h = this->height();
   int x = this->x();
   int y = window()->height() - (this->y() + h );
   glViewport(x, y, w, h);

   glEnable(GL_DEPTH_TEST);
   glClear(GL_COLOR_BUFFER_BIT);

   glBindTexture(GL_TEXTURE_2D, m_iTexture[0]);
   //glDrawArrays(GL_TRIANGLE_STRIP, 0 , m_iNumVertices) ;
   glDrawArrays(GL_LINE_STRIP, 0 , m_iNumVertices) ;

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
