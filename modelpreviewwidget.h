#ifndef MODELPREVIEWWIDGET_H
#define MODELPREVIEWWIDGET_H

#include "MCResourcePack.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QTimer>
#include <QImage>

class ModelPreviewWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit ModelPreviewWidget(MCResourcePackElement* element, QWidget *parent = nullptr);
    void setTexture(const QImage& texture);
    void setModelVertices(const QVector<float>& vertices);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    MCResourcePackElement* element;
    QOpenGLShaderProgram shader;
    QVector<float> modelVertices;
    GLuint textureId = 0;
    GLuint vbo = 0;
    QTimer animationTimer;
    QVector<QImage> animationFrames;
    int currentFrame;

    float yaw = 0.0f;
    float pitch = 0.0f;
    float zoom = 3.0f;

    QPoint lastMousePos;

    void loadTexture(const QImage &img);
};

#endif // MODELPREVIEWWIDGET_H
