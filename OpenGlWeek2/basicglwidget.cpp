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

    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    vshader->compileSourceFile(":/BasicGlVertex.vert");

    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);
    fshader->compileSourceFile( ":/BasicGlFragment.frag" );


    program = new QGLShaderProgram(this);
    program->addShader(vshader);
    program->addShader(fshader);
    program->bindAttributeLocation("vertices", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->link();

    program->bind();
    program->setUniformValue("sphereTexmap", 0);
}

void basicGlWidget::paintGL()
{
    qglClearColor(clearColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 matrix;
    matrix.rotate( xRot / 16.0f, 0.2f, 1, 0);

    matrix.ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);

    program->setUniformValue("mvpMatrix", matrix);
    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->setAttributeArray
        (PROGRAM_VERTEX_ATTRIBUTE, vertices.constData());
    program->setAttributeArray
        (PROGRAM_TEXCOORD_ATTRIBUTE, texCoords.constData());



    int tIdx = 0;
    for (int i = 0; i < m_iSphereRes  ; ++i)
    {
        //glActiveTexture( textures[tIdx]);
        glBindTexture(GL_TEXTURE_2D, textures[tIdx]);
        int numVertexStrip = m_iSphereRes * 2 + 2;
        glDrawArrays(GL_TRIANGLE_STRIP, i * numVertexStrip , numVertexStrip * 2) ;
        qDebug()<< i << ", " << i * numVertexStrip  << ", " << numVertexStrip * 2 ;
        //glDrawArrays(GL_LINE_STRIP, i * numVertexStrip , numVertexStrip * 2) ;
        tIdx = (tIdx + 1) %  m_iNumTextures;
    }

//    for (int i = 0; i < m_iSphereRes   ; ++i)
//    {
//        //glActiveTexture( textures[tIdx]);
//        glBindTexture(GL_TEXTURE_2D, textures[0]);
//        int numVertexStrip = m_iSphereRes * 2 + 2;
//        glDrawArrays(GL_TRIANGLE_STRIP, i * numVertexStrip , numVertexStrip) ;
//        glBindTexture(GL_TEXTURE_2D, textures[1]);
//        glDrawArrays(GL_TRIANGLE_STRIP, i * numVertexStrip + numVertexStrip  , numVertexStrip) ;
//        qDebug()<< i << ", " << i * numVertexStrip  << ", " << numVertexStrip * 2 ;
//        //glDrawArrays(GL_LINE_STRIP, i * numVertexStrip , numVertexStrip * 2) ;
//        tIdx = (tIdx + 1) %  m_iNumTextures;
//    }
}

void basicGlWidget::resizeGL(int width, int height)
{

    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}


void basicGlWidget::makeObject()
{
    initSphereVertices();


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
        vertices.append
            (QVector3D(m_fVertices[idx ],
                       m_fVertices[idx + 1],
                       0.0f));
        vIdx += 2;

    }
}

void basicGlWidget::initSphereVertices()
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
            qDebug()<< x << ", " << y1 << ", " << y2 ;
            m_fVertices[vIdx++] = x;
            m_fVertices[vIdx++] = y1;
            m_fVertices[vIdx++] = x;
            m_fVertices[vIdx++] = y2;

        }
    }
}
