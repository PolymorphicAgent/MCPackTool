#ifndef MCRESOURCEPACK_H
#define MCRESOURCEPACK_H

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileInfo>
#include <QObject>
#include <QString>
#include <QImage>
#include <QDebug>
#include <QList>
#include <QFile>
#include <QDir>

#include <KZip>

// Enumeration to allow different return types.
// Currently only Memory is implemented.
enum ResourceDataType {
    Memory, // fully loaded into memory (QImage, QJsonDocument)
    Stream  // pointer/stream to data (not implemented here)
};

class MCResourcePackElement
{
public:
    explicit MCResourcePackElement(const QString &baseName);
    ~MCResourcePackElement();

    // Setters used by the parser.
    void setTexture(const QImage &texture);
    void setModel(const QJsonDocument &model);
    void setAnimation(const QJsonDocument &animation);

    // Getters – return in–memory objects for now.
    QImage getTexture(ResourceDataType type = Memory) const;
    QJsonDocument getModel(ResourceDataType type = Memory) const;
    QJsonDocument getAnimation(ResourceDataType type = Memory) const;
    bool isAnimated() const;

    // Returns the grouping identifier (for example, "creeper" if the files were under assets/creeper/…)
    QString getBaseName() const;

private:
    QString m_baseName;
    QImage m_texture;
    QJsonDocument m_model;
    QJsonDocument m_animation;
    bool m_animated;
};

class MCResourcePack : public QObject
{
    Q_OBJECT
public:
    explicit MCResourcePack(const QString &path, QObject *parent = nullptr);
    ~MCResourcePack();

    // Loads the resourcepack from a zip file or folder and groups related files
    // into MCResourcePackElement instances. This method is expected to run in a worker thread.
    void parse();

    // Get the resourcepack’s pack image (pack.png) or its file stream, depending on the type.
    QImage getPackPng(ResourceDataType type = Memory) const;
    // Get the pack metadata (pack.mcmeta) as a QJsonDocument or file stream.
    QJsonDocument getPackMcmeta(ResourceDataType type = Memory) const;

    // Returns the list of parsed resourcepack elements.
    QList<MCResourcePackElement*> getElements() const;

signals:
    // Emitted to update progress – connect to ProgressMessageBox::setProgress(int)
    void progressUpdated(int progress);
    // Emitted when parsing is finished.
    void parsingFinished();

private:
    QString m_path;
    QList<MCResourcePackElement*> m_elements;
    QImage m_packPng;
    QJsonDocument m_packMcmeta;
    bool m_parsed;

    // Helper function that processes a file’s data (given its relative path)
    // and groups texture.png, model.json, and animation.mcmeta into the appropriate element.
    void processFile(const QString &filePath, const QByteArray &data);
};

#endif // MCRESOURCEPACK_H
