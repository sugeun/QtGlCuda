#ifndef BASICGLWIDGET_H
#define BASICGLWIDGET_H


#include <QtWidgets>
#include <QGLWidget>
#include <QOpenGLFunctions>

QT_FORWARD_DECLARE_CLASS(QGLShaderProgram);

class basicGlWidget : public QGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit basicGlWidget(QWidget *parent = 0, QGLWidget *shareWidget = 0);
    ~basicGlWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void setClearColor(const QColor &color);

//signals:
//    void clicked();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);


private:
    void makeObject();
    void initVertices();

    QColor clearColor;
    QPoint lastPos;
    int xRot;
    int yRot;
    int zRot;
    GLuint textures[6];
    QVector<QVector3D> vertices;
    QVector<QVector2D> texCoords;
    QVector<QVector3D> surfNorms;
    QGLShaderProgram *program;

    float* m_fVertices;
    int    m_iSphereRes;
    int    m_iNumTextures;
    int    m_iNumVertices;
};

#endif

