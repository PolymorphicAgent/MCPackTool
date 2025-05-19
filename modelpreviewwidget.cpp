#include "modelpreviewwidget.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QWheelEvent>
#include <QMouseEvent>
#include <QDateTime>

#include <cmath>

struct Vertex { float x,y,z; float r,g,b; };

ModelPreviewWidget::ModelPreviewWidget(MCResourcePackElement* element, QWidget* parent)
    : QOpenGLWidget(parent), m_element(element),
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(200,200);

    m_animated = m_element->isAnimated();
    if (m_animated) {
        QJsonObject anim = m_element->getAnimation().object();
        QJsonArray frames = anim.value("frames").toArray();
        int defaultDuration = anim.value("animation_time").toInt(1);
        for (const QJsonValue& v : std::as_const(frames)) {
            if (v.isObject()) {
                int index = v.toObject().value("index").toInt();
                int duration = v.toObject().value("time").toInt(defaultDuration);
                m_frameList.append(index);
                m_frameDurations.append(duration);
            } else if (v.isDouble()) {
                int index = v.toInt();
                m_frameList.append(index);
                m_frameDurations.append(defaultDuration);
            }
        }
        m_frameCount = m_frameList.size();
        connect(&m_timer, &QTimer::timeout, this, &ModelPreviewWidget::onTimeout);
        m_timer.start(50); // ~20 FPS updates
    }
}

ModelPreviewWidget::~ModelPreviewWidget() {
    cleanupResources();
}

void ModelPreviewWidget::initializeGL() {
    qDebug() << "initializeGL called";

    initializeOpenGLFunctions();

    // auto fmt = context()->format();
    // qDebug() << "Got GL:" << fmt.renderableType()
    //          << fmt.majorVersion() << fmt.minorVersion()
    //          << (fmt.profile()==QSurfaceFormat::CoreProfile?"Core":"Compat");

    // Ensure cleanup before context is destroyed
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &ModelPreviewWidget::cleanupResources, Qt::DirectConnection);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);    // <- non‑black background
    initShaders();
    initTestGeometry();
    // initGeometry();
    // loadTexture();
    glEnable(GL_DEPTH_TEST);

    // initializeOpenGLFunctions();

    // // glEnable(GL_DEPTH_TEST);
    // // glEnable(GL_TEXTURE_2D);

    // // shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/gl/shaders/basic.vert");
    // // shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/gl/shaders/basic.frag");
    // // shader.link();

    // // glGenBuffers(1, &vbo);

    // glEnable(GL_DEPTH_TEST);

    // // **1) Generate & bind VAO**
    // // Create and bind the VAO
    // vao.create();
    // vao.bind();

    // // 2) Generate VBO
    // glGenBuffers(1, &vbo);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // // 3) Shader setup...
    // shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/gl/shaders/basic.vert");
    // shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/gl/shaders/basic.frag");
    // shader.link();

    // if (!shader.link()) {
    //     qWarning() << "Shader link error:" << shader.log();
    // }


    // // 4) Configure vertex attributes
    // shader.bind();
    // // int posLoc = shader.attributeLocation("position");
    // // int uvLoc  = shader.attributeLocation("texCoord");
    // // glEnableVertexAttribArray(posLoc);
    // // glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    // // glEnableVertexAttribArray(uvLoc);
    // // glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    // shader.enableAttributeArray("position");
    // shader.setAttributeBuffer("position", GL_FLOAT, 0, 3, 5 * sizeof(float));
    // shader.enableAttributeArray("texCoord");
    // shader.setAttributeBuffer("texCoord", GL_FLOAT, 3 * sizeof(float), 2, 5 * sizeof(float));
    // shader.release();

    // // Unbind VAO to avoid accidental modification
    // vao.release();

    // parseVertices();
}

void ModelPreviewWidget::resizeGL(int w, int h) {
    qDebug() << "resizeGL called";
    glViewport(0, 0, w, h);
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.1f, 100.0f);
}

void ModelPreviewWidget::paintGL() {
    // qDebug() << "paintGL called";
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program.bind();
    m_vao.bind();

    QMatrix4x4 mv;
    mv.translate(0,0,-3);
    mv.scale(m_zoom);
    mv.rotate(m_pitch,1,0,0);
    mv.rotate(m_yaw,0,1,0);

    m_program.setUniformValue("u_projMatrix", m_proj);
    m_program.setUniformValue("u_modelViewMatrix", mv);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_vao.release();
    m_program.release();

    // QMatrix4x4 modelView;
    // modelView.translate(0, 0, -5.0f);
    // modelView.scale(m_zoom);
    // modelView.rotate(m_pitch, 1, 0, 0);
    // modelView.rotate(m_yaw, 0, 1, 0);

    // m_program.setUniformValue("u_projMatrix", m_proj);
    // m_program.setUniformValue("u_modelViewMatrix", modelView);

    // glActiveTexture(GL_TEXTURE0);

    // if (m_texture) {
    //     // qDebug()<<"binding texture...";
    //     m_texture->bind();
    //     m_program.setUniformValue("u_texture", 0);
    //     m_program.setUniformValue("u_lightDir",  QVector3D(0.5f, 1.0f, 0.8f));
    // }

    // glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

    // m_vao.release();
    // m_program.release();

    // QMatrix4x4 projection;
    // projection.perspective(45.0f, float(width()) / height(), 0.1f, 100.0f);

    // QMatrix4x4 view;
    // view.translate(0, 0, -zoom);
    // view.rotate(pitch, 1, 0, 0);
    // view.rotate(yaw, 0, 1, 0);

    // QMatrix4x4 model;
    // model.scale(1.0f);

    // shader.bind();
    // shader.setUniformValue("projection", projection);
    // shader.setUniformValue("view", view);
    // shader.setUniformValue("model", model);
    // shader.setUniformValue("textureSampler", 0);

    // glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(float), modelVertices.data(), GL_STATIC_DRAW);

    // glEnableVertexAttribArray(0); // Position
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(1); // TexCoord
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, textureId);

    // glDrawArrays(GL_TRIANGLES, 0, modelVertices.size() / 5);

    // glDisableVertexAttribArray(0);
    // glDisableVertexAttribArray(1);
    // shader.release();
}

void ModelPreviewWidget::mousePressEvent(QMouseEvent* event) {
    m_lastMousePos = QVector2D(event->pos());
}

void ModelPreviewWidget::mouseMoveEvent(QMouseEvent* event) {
    QVector2D delta = QVector2D(event->pos()) - m_lastMousePos;
    m_yaw   += delta.x() * 0.5f;
    m_pitch += delta.y() * 0.5f;
    m_lastMousePos = QVector2D(event->pos());
    update();
}

void ModelPreviewWidget::wheelEvent(QWheelEvent* event) {
    float delta = event->angleDelta().y() / 120.0f;
    m_zoom *= std::pow(1.1f, delta);
    update();
}

void ModelPreviewWidget::onTimeout() {
    if (!m_animated || m_frameCount == 0)
        return;
    m_elapsedTime += m_timer.interval();
    int duration = m_frameDurations.at(m_currentFrameIndex);
    if (m_elapsedTime >= duration * 50) {
        m_elapsedTime = 0;
        m_currentFrameIndex = (m_currentFrameIndex + 1) % m_frameCount;
        // Reload texture for next frame
        delete m_texture;
        m_texture = nullptr;
        loadTexture();
        update();
    }
}

// void ModelPreviewWidget::setTexture(const QImage& image) {
//     makeCurrent();
//     loadTexture(image);
//     doneCurrent();
//     update();
// }

void ModelPreviewWidget::initShaders() {

    const char* vsrc = R"(
        #version 330 core
        layout(location=0) in vec3 a_position;
        layout(location=1) in vec3 a_color;
        out vec3 v_color;
        uniform mat4 u_projMatrix;
        uniform mat4 u_modelViewMatrix;
        void main() {
            gl_Position = u_projMatrix * u_modelViewMatrix * vec4(a_position, 1.0);
            v_color = a_color;
        }
    )";
    const char* fsrc = R"(
        #version 330 core
        in vec3 v_color;
        out vec4 fragColor;
        void main() {
            fragColor = vec4(v_color, 1.0);
        }
    )";

    if (!m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc))
        qDebug() << "Vertex shader compile error:" << m_program.log();
    if (!m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc))
        qDebug() << "Fragment shader compile error:" << m_program.log();
    m_program.bindAttributeLocation("a_position",0);
    m_program.bindAttributeLocation("a_color",1);
    if (!m_program.link())
        qDebug() << "Shader link error:" << m_program.log();

    // m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
    // m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);

    // m_program.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/gl/shaders/vsrc.glsl");
    // m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/gl/shaders/fsrc.glsl");

    // m_program.bindAttributeLocation("a_position", 0);
    // m_program.bindAttributeLocation("a_normal",   1);
    // m_program.bindAttributeLocation("a_texcoord", 2);

    // if (!m_program.link()) {
    //     qFatal("Shader link failed:\n%s", qPrintable(m_program.log()));
    // }

}

void ModelPreviewWidget::initGeometry() {
    // Parse JSON model to fill m_vertices and m_indices
    QJsonObject obj = m_element->getModel().object();
    QJsonArray elements = obj.value("elements").toArray();
    int baseVertex = 0;

    for (const QJsonValue& elVal : std::as_const(elements)) {
        QJsonObject el = elVal.toObject();
        QJsonArray from = el.value("from").toArray();
        QJsonArray to   = el.value("to").toArray();
        QVector3D v0(from[0].toDouble(), from[1].toDouble(), from[2].toDouble());
        QVector3D v1(to[0].toDouble(),   to[1].toDouble(),   to[2].toDouble());
        // For simplicity, build a box: 8 vertices, 36 indices (12 triangles)
        QVector< QVector3D > verts = {
            {v0.x(), v0.y(), v0.z()}, {v1.x(), v0.y(), v0.z()},
            {v1.x(), v1.y(), v0.z()}, {v0.x(), v1.y(), v0.z()},
            {v0.x(), v0.y(), v1.z()}, {v1.x(), v0.y(), v1.z()},
            {v1.x(), v1.y(), v1.z()}, {v0.x(), v1.y(), v1.z()}
        };
        // UV from the face definition
        QJsonObject faces = el.value("faces").toObject();
        for (auto it = faces.begin(); it != faces.end(); ++it) {
            QJsonObject face = it.value().toObject();
            QJsonArray uv = face.value("uv").toArray();
            float u0 = uv[0].toDouble(), v0_ = uv[1].toDouble();
            float u1 = uv[2].toDouble(), v1_ = uv[3].toDouble();
            QVector< QVector2D > uvs = {{u0, v0_}, {u1, v0_}, {u1, v1_}, {u0, v1_}};
            // Quad indices for this face (assumes ordering matches verts array)
            QVector<int> idx = {0,1,2, 2,3,0};
            for (int i = 0; i < 6; ++i) {
                QVector3D pos = verts[idx[i]];
                QVector2D tc  = uvs[idx[i] % 4];
                m_vertices.push_back(pos.x());
                m_vertices.push_back(pos.y());
                m_vertices.push_back(pos.z());
                m_vertices.push_back(tc.x() / 16.0f);
                m_vertices.push_back(1.0f - tc.y() / 16.0f);
                m_indices.push_back(baseVertex++);
            }
        }
    }

    m_vao.create();
    m_vao.bind();
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(m_vertices.constData(), m_vertices.size() * sizeof(float));

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(m_indices.constData(), m_indices.size() * sizeof(unsigned int));

    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, 5 * sizeof(float));
    m_program.enableAttributeArray(1);
    m_program.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 2, 5 * sizeof(float));

    m_vao.release();
    m_vertexBuffer.release();
    m_indexBuffer.release();
}

void ModelPreviewWidget::loadTexture() {
    QImage image = m_element->getTexture().convertToFormat(QImage::Format_RGBA8888);
    if (image.isNull()) {
        qWarning("ModelPreviewWidget: texture image is null");
        return;
    }
    if (m_animated && m_frameCount > 0) {
        // Animated: choose frame by cropping image
        int frameIndex = m_frameList.at(m_currentFrameIndex);
        int size = image.width();
        int framesPerRow = size / 16;
        int x = (frameIndex % framesPerRow) * 16;
        int y = (frameIndex / framesPerRow) * 16;
        image = image.copy(x, y, 16, 16);
    }
    if (m_texture) delete m_texture;
    m_texture = new QOpenGLTexture(image.flipped());
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Nearest);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

void ModelPreviewWidget::initTestGeometry() {
    // Simple colored triangle
    Vertex verts[3] = {
        { 0.0f,  1.0f, 0.0f,   1,0,0},
        {-1.0f, -1.0f, 0.0f,   0,1,0},
        { 1.0f, -1.0f, 0.0f,   0,0,1}
    };
    m_vao.create(); m_vao.bind();
    m_vertexBuffer.create(); m_vertexBuffer.bind();
    m_vertexBuffer.allocate(verts, 3 * sizeof(Vertex));

    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, GL_FLOAT, offsetof(Vertex,x), 3, sizeof(Vertex));
    m_program.enableAttributeArray(1);
    m_program.setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex,r), 3, sizeof(Vertex));

    m_vao.release();
}

void ModelPreviewWidget::cleanupResources() {
    makeCurrent();
    if (m_vao.isCreated())          m_vao.destroy();
    if (m_vertexBuffer.isCreated()) m_vertexBuffer.destroy();
    if (m_indexBuffer.isCreated())  m_indexBuffer.destroy();
    if (m_program.isLinked())       m_program.removeAllShaders();
    if (m_texture) {
        if (m_texture->isCreated()) m_texture->destroy();
        delete m_texture; m_texture = nullptr;
    }
    doneCurrent();
}

// void ModelPreviewWidget::setModelVertices(const QVector<float> &vertices) {
//     modelVertices = vertices;
//     update();
// }

// void ModelPreviewWidget::loadTexture(const QImage &img) {
//     if (textureId != 0) {
//         glDeleteTextures(1, &textureId);
//     }

//     QImage tex = img.convertToFormat(QImage::Format_RGBA8888).mirrored();
//     glGenTextures(1, &textureId);
//     glBindTexture(GL_TEXTURE_2D, textureId);

//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
// }
