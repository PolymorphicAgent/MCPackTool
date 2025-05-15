#include "modelpreviewwidget.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QWheelEvent>
#include <QMouseEvent>

ModelPreviewWidget::ModelPreviewWidget(MCResourcePackElement* element, QWidget *parent) :
    QOpenGLWidget(parent), element(element), zoom(3.0f), currentFrame(0)
{
    setFocusPolicy(Qt::StrongFocus);

    //load texture image
    QImage img = element->getTexture();
    if (!img.isNull()) {
        loadTexture(img);
    }

    //parse model JSON and build vertices
    QJsonDocument doc = element->getModel();  // parse JSON model
    if (!doc.isNull() && doc.isObject()) {
        QJsonObject obj = doc.object();        // get root object
        QJsonArray elements = obj.value("elements").toArray();
        for (const QJsonValue &val : elements) {
            QJsonObject elemObj = val.toObject();
            //each element defines a cuboid: from[], to[]
            QJsonArray from = elemObj.value("from").toArray();
            QJsonArray to   = elemObj.value("to").toArray();
            //face UV mapping
            QJsonObject faces = elemObj.value("faces").toObject();
            //build 8 corner points
            QVector3D corners[8];
            for (int i = 0; i < 8; ++i) {
                float x = (i & 1) ? to[0].toDouble()   : from[0].toDouble();
                float y = (i & 2) ? to[1].toDouble()   : from[1].toDouble();
                float z = (i & 4) ? to[2].toDouble()   : from[2].toDouble();
                corners[i] = QVector3D(x/16.0f, y/16.0f, z/16.0f);
            }
            //for each face (north, south, east, west, up, down), build two triangles
            static const char* faceNames[] = {"north","south","east","west","up","down"};
            static const int faceCorners[6][4] = {
                {0,4,6,2}, {1,3,7,5},
                {4,5,7,6}, {0,1,3,2},
                {2,6,7,3}, {0,1,5,4}
            };
            for (int f=0; f<6; ++f) {
                if (!faces.contains(faceNames[f])) continue;
                QJsonObject faceObj = faces.value(faceNames[f]).toObject();
                QJsonArray uv = faceObj.value("uv").toArray();
                float u1 = uv[0].toDouble()/img.width(), v1 = uv[1].toDouble()/img.height();
                float u2 = uv[2].toDouble()/img.width(), v2 = uv[3].toDouble()/img.height();
                int c0 = faceCorners[f][0], c1 = faceCorners[f][1],
                    c2 = faceCorners[f][2], c3 = faceCorners[f][3];
                //triangle 1
                modelVertices << corners[c0].x() << corners[c0].y() << corners[c0].z() << u1 << v1;
                modelVertices << corners[c1].x() << corners[c1].y() << corners[c1].z() << u1 << v2;
                modelVertices << corners[c2].x() << corners[c2].y() << corners[c2].z() << u2 << v2;
                //triangle 2
                modelVertices << corners[c2].x() << corners[c2].y() << corners[c2].z() << u2 << v2;
                modelVertices << corners[c3].x() << corners[c3].y() << corners[c3].z() << u2 << v1;
                modelVertices << corners[c0].x() << corners[c0].y() << corners[c0].z() << u1 << v1;
            }
        }
    }

    //load animation frames if present
    if (element->isAnimated()) {
        QJsonDocument animDoc = element->getAnimation();  // .mcmeta JSON :contentReference[oaicite:4]{index=4}
        QJsonObject animObj = animDoc.object();
        QJsonArray frames = animObj.value("animation").toObject().value("frames").toArray();
        QImage baseTex = element->getTexture();
        for (const QJsonValue &f : frames) {
            int idx = f.toInt();
            // For simplicity, assume vertical strip: each frame = full width × height
            int h = baseTex.height() / frames.size();
            animationFrames.append(baseTex.copy(0, idx*h, baseTex.width(), h));
        }
        connect(&animationTimer, &QTimer::timeout, this, [this]() {
            currentFrame = (currentFrame + 1) % animationFrames.size();
            loadTexture(animationFrames[currentFrame]);
            update();
        });
        int frameTime = animObj.value("animation").toObject().value("frametime").toInt(1) * 50;
        animationTimer.start(frameTime);
    }
}

void ModelPreviewWidget::initializeGL() {
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/basic.vert");
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/basic.frag");
    shader.link();

    glGenBuffers(1, &vbo);
}

void ModelPreviewWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void ModelPreviewWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 projection;
    projection.perspective(45.0f, float(width()) / height(), 0.1f, 100.0f);

    QMatrix4x4 view;
    view.translate(0, 0, -zoom);
    view.rotate(pitch, 1, 0, 0);
    view.rotate(yaw, 0, 1, 0);

    QMatrix4x4 model;
    model.scale(1.0f);

    shader.bind();
    shader.setUniformValue("projection", projection);
    shader.setUniformValue("view", view);
    shader.setUniformValue("model", model);
    shader.setUniformValue("textureSampler", 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(float), modelVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1); // TexCoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glDrawArrays(GL_TRIANGLES, 0, modelVertices.size() / 5);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    shader.release();
}

void ModelPreviewWidget::mousePressEvent(QMouseEvent *event) {
    lastMousePos = event->pos();
}

void ModelPreviewWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->x() - lastMousePos.x();
    int dy = event->y() - lastMousePos.y();

    if (event->buttons() & Qt::LeftButton) {
        yaw += dx * 0.5f;
        pitch += dy * 0.5f;
        update();
    }

    lastMousePos = event->pos();
}

void ModelPreviewWidget::wheelEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0)
        zoom = std::max(1.0f, zoom - 0.25f);
    else
        zoom = std::min(10.0f, zoom + 0.25f);

    update();
}

void ModelPreviewWidget::setTexture(const QImage& image) {
    makeCurrent();
    loadTexture(image);
    doneCurrent();
    update();
}

void ModelPreviewWidget::setModelVertices(const QVector<float> &vertices) {
    modelVertices = vertices;
    update();
}

void ModelPreviewWidget::loadTexture(const QImage &img) {
    if (textureId != 0) {
        glDeleteTextures(1, &textureId);
    }

    QImage tex = img.convertToFormat(QImage::Format_RGBA8888).mirrored();
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
