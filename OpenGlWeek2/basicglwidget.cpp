#include <QtWidgets>
#include <QtOpenGL>


#include "basicGlWidget.h"


basicGlWidget::basicGlWidget(QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(parent, shareWidget)
    , m_iSphereRes( 100 )
    , m_iNumTextures(2)
{
    clearColor = Qt::black;
    xRot = 0;
    yRot = 0;
    zRot = 0;
    program = 0;
}

basicGlWidget::~basicGlWidget()
{
}

QSize basicGlWidget::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize basicGlWidget::sizeHint() const
{
    return QSize(400, 400);
}

void basicGlWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    updateGL();
}

void basicGlWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    updateGL();
}

void basicGlWidget::initializeGL()
{
    initializeOpenGLFunctions();

    makeObject();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
#ifdef GL_TEXTURE_2D
    glEnable(GL_TEXTURE_2D);
#endif

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1
#define PROGRAM_SURFACENORMAL_ATTRIBUTE 2

    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    vshader->compileSourceFile(":/BasicGlVertex.vert");

    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);
    fshader->compileSourceFile( ":/BasicGlFragment.frag" );


    program = new QGLShaderProgram(this);
    program->addShader(vshader);
    program->addShader(fshader);
    program->bindAttributeLocation("vertices", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->bindAttributeLocation("surfNormal", PROGRAM_SURFACENORMAL_ATTRIBUTE);
    program->link();

    program->bind();
    program->setUniformValue("sphereTexmap", 0);
}

void basicGlWidget::paintGL()
{
    qglClearColor(clearColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 mvmatrix, mvpMatrix;
    mvmatrix.rotate( xRot / 160.0f, 0.2f, 1, 0);

    program->setUniformValue("mvMatrix", mvmatrix);
    mvpMatrix = mvmatrix;
    mvpMatrix.ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);

    program->setUniformValue("mvpMatrix", mvpMatrix);
    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_SURFACENORMAL_ATTRIBUTE);
    program->setAttributeArray
        (PROGRAM_VERTEX_ATTRIBUTE, vertices.constData());
    program->setAttributeArray
        (PROGRAM_TEXCOORD_ATTRIBUTE, texCoords.constData());
    program->setAttributeArray
        (PROGRAM_SURFACENORMAL_ATTRIBUTE, surfNorms.constData());


    int tIdx = 0;
    for (int i = 0; i < m_iSphereRes - 1 ; ++i)
    {
        //glActiveTexture( textures[tIdx]);
        glBindTexture(GL_TEXTURE_2D, textures[tIdx]);
        int numVertexStrip = m_iSphereRes * 2 + 2;
        glDrawArrays(GL_TRIANGLE_STRIP, i * numVertexStrip , numVertexStrip * 2) ;
        //qDebug()<< i << ", " << i * numVertexStrip  << ", " << numVertexStrip * 2 ;
        //glDrawArrays(GL_LINE_STRIP, i * numVertexStrip , numVertexStrip * 2) ;
        tIdx = (tIdx + 1) %  m_iNumTextures;
    }

}

void basicGlWidget::resizeGL(int width, int height)
{

    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}


void basicGlWidget::makeObject()
{
    initVertices();


    for( int i = 0 ; i < m_iNumTextures ; i++)
    {
        textures[i] = bindTexture
            (QPixmap(QString(":/Marble_%1.png").arg(i)), GL_TEXTURE_2D);
    }


    int vIdx = 0;
    for (int i = 0; i < m_iNumVertices; ++i)
    {
        int idx = vIdx;
        texCoords.append
            (QVector2D(m_fVertices[idx], m_fVertices[idx + 1 ]));

        float x = m_fVertices[idx ];
        float y = m_fVertices[idx + 1];
        float theta = x * 2.0f * 3.14f;
        float phi = (1.0f - y ) * 3.14f;

        float sphereX = sin(theta) * sin(phi);
        float sphereY = cos(phi);
        float sphereZ = cos(theta) * sin(phi);

        vertices.append
            (QVector3D( sphereX,
                        sphereY,
                        sphereZ));

        surfNorms.append(QVector3D( sphereX,
                                    sphereY,
                                    sphereZ).normalized());
        //qDebug()<< "(" << x << "," << y << ")  " << sphereX << ", " << sphereY << ", " << sphereZ  ;

        vIdx += 2;

    }
    qDebug()<< "vIdx: " << vIdx;


//    for( int j = 0 ; j < vertices.count() - 2 ; j++)
//    {
//        QVector3D normalVector;
//        QVector3D uVector;
//        QVector3D vVector;
//        QVector3D tPoint[3];

//        for( int i = 0 ; i < 3 ; i++)
//        {
//           tPoint[i] = vertices[j + i];
//        }
////        tPoint[0] = QVector3D(1.0,-1.0,-1.0);
////        tPoint[1] = QVector3D(1.0,-1.0,1.0);
////        tPoint[2] = QVector3D(-1.0,-1.0,1.0);

//        uVector = tPoint[1] - tPoint[0];
//        vVector = tPoint[2] - tPoint[0];

//        normalVector = QVector3D::crossProduct(uVector,vVector);
//        normalVector.normalize();

//        qDebug()<< "(" << j << ")  " << normalVector.x() << ", " << normalVector.y() << ", " << normalVector.z()  ;

//        surfNorms.append(normalVector);
//    }

}

void basicGlWidget::initVertices()
{
    m_iNumVertices = (m_iSphereRes )  * (m_iSphereRes + 1 ) * 2;

    m_fVertices = new float[m_iNumVertices * 2];

    float delta = 1.0f / float(m_iSphereRes);
    float deltax = 1.0f / float(m_iSphereRes);

    int vIdx = 0;
    for( int j = 0 ; j < m_iSphereRes ; j++ )
    {
        for( int i = 0 ; i <= m_iSphereRes  ; i++ )
        {
            float x, y1, y2;
            x = float(i) * deltax;
            y1 = 1.0f - float(j) * delta;
            y2 = 1.0f - float(j+1) * delta;
            //qDebug()<< x << ", " << y1 << ", " << y2 ;
            m_fVertices[vIdx++] = x;
            m_fVertices[vIdx++] = y1;
            m_fVertices[vIdx++] = x;
            m_fVertices[vIdx++] = y2;

        }
    }
    qDebug()<< "vIdx: " << vIdx;
}

//QVector3D basicGlWidget::calculateSurfaceNormal(float* triangle)
//{
//   QVector3D normalVector;
//   QVector3D uVector;
//   QVector3D vVector;
//   QVector3D tPoint[3];

//   for( int i = 0 ; i < 3 ; i++)
//   {
//      tPoint[i].setX( qreal(triangle[i * 3 ]));
//      tPoint[i].setY( qreal(triangle[i * 3 + 1]));
//      tPoint[i].setZ( qreal(triangle[i * 3 + 2]));
//   }

//   uVector = tPoint[1] - tPoint[0];
//   vVector = tPoint[2] - tPoint[0];

//   normalVector = QVector3D::crossProduct(uVector,vVector);
//   return normalVector;
//}
