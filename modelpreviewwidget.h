#ifndef MODELPREVIEWWIDGET_H
#define MODELPREVIEWWIDGET_H

#include "MCResourcePack.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QTimer>
#include <QImage>

class ModelPreviewWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit ModelPreviewWidget(MCResourcePackElement* element, QWidget *parent = nullptr);
    ~ModelPreviewWidget();

    void cleanupResources();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;

private slots:
    void onTimeout();  // Timer slot for animation updates

private:
    void initShaders();
    void initTestGeometry();
    void initGeometry();
    void loadTexture();
    void updateAnimation();

    MCResourcePackElement* m_element;
    QOpenGLShaderProgram m_program;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLTexture* m_texture{nullptr};

    QVector<float> m_vertices;
    QVector<unsigned int> m_indices;

    // Transformation
    QMatrix4x4 m_proj;
    float m_zoom{1.0f};
    QVector2D m_lastMousePos;
    float m_pitch{0.0f};
    float m_yaw{0.0f};

    // Animation
    bool m_animated{false};
    QVector<int> m_frameList;
    QVector<int> m_frameDurations;
    int m_frameCount{0};
    int m_currentFrameIndex{0};
    int m_elapsedTime{0};
    QTimer m_timer;

    // void parseVertices();

    // MCResourcePackElement* element;
    // QOpenGLShaderProgram shader;
    // QOpenGLVertexArrayObject vao;
    // QVector<float> modelVertices;
    // GLuint textureId = 0;
    // GLuint vbo = 0;
    // QTimer animationTimer;
    // QVector<QImage> animationFrames;
    // int currentFrame;

    // float yaw = 0.0f;
    // float pitch = 0.0f;
    // float zoom = 3.0f;

    // QPoint lastMousePos;

    // void loadTexture(const QImage &img);
};

#endif // MODELPREVIEWWIDGET_H
