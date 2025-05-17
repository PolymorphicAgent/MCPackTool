#include "mcresourcepack.h"

//================ MCResourcePackElement Implementation ================

MCResourcePackElement::MCResourcePackElement(const QString &baseName)
    : m_baseName(baseName), m_animated(false)
{}

MCResourcePackElement::~MCResourcePackElement()
{}

void MCResourcePackElement::setTexture(const QImage &texture){
    m_texture = texture;
}

void MCResourcePackElement::setModel(const QJsonDocument &model){
    m_model = model;
}

void MCResourcePackElement::setAnimation(const QJsonDocument &animation){
    m_animation = animation;
    m_animated = !animation.isNull();
}

QImage MCResourcePackElement::getTexture(ResourceDataType type) const{
    // Only Memory type is implemented.
    Q_UNUSED(type);
    return m_texture;
}

QJsonDocument MCResourcePackElement::getModel(ResourceDataType type) const{
    // Only Memory type is implemented.
    Q_UNUSED(type);
    return m_model;
}

QJsonDocument MCResourcePackElement::getAnimation(ResourceDataType type) const{
    // Only Memory type is implemented.
    Q_UNUSED(type);
    return m_animation;
}

bool MCResourcePackElement::isAnimated() const{
    return m_animated;
}

QString MCResourcePackElement::getBaseName() const{
    return m_baseName;
}

//================ MCResourcePack Implementation ======================

MCResourcePack::MCResourcePack(const QString &path, QObject *parent)
    : QObject(parent),
    m_path(path),
    m_parsed(false)
{}

MCResourcePack::~MCResourcePack(){
    // Clean up allocated elements.
    qDeleteAll(m_elements);
    m_elements.clear();
}

// This method should be executed in a worker thread.
// It demonstrates a simple approach: if m_path is a zip file, you would use a zip
// library (like QuaZip or QZipReader) to read entries. Here we simulate parsing
// by checking whether m_path is a directory.
void MCResourcePack::parse()
{
    QList<QPair<QString, QByteArray>> files; // Pair: relative file path and file data.

    QFileInfo info(m_path);
    if (info.isFile() && m_path.endsWith(".zip", Qt::CaseInsensitive)) {
        // Pseudo-code: read zip archive entries.
        // For each entry in the zip:
        //    QByteArray data = ...;
        //    QString relPath = ...;
        //    files.append(qMakePair(relPath, data));
        // qDebug() << "Parsing zip file:" << m_path;

        KZip archive(m_path);
        if (!archive.open(QIODevice::ReadOnly)) {
            qWarning() << "MCResourcePack: Cannot open ZIP:" << m_path;
            //TODO: alert user
            return;
        }
        const KArchiveDirectory *root = archive.directory();
        // Recursive lambda to traverse directories
        std::function<void(const KArchiveDirectory*, const QString&)> traverse;
        traverse = [&](const KArchiveDirectory *dir, const QString &path) {
            for (const QString &name : dir->entries()) {
                const KArchiveEntry *entry = dir->entry(name);
                QString relPath = path + name;
                if (entry->isDirectory()) {
                    traverse(static_cast<const KArchiveDirectory*>(entry), relPath + '/');
                } else {
                    auto fileEntry = static_cast<const KArchiveFile*>(entry);
                    QByteArray data = fileEntry->data();
                    files.append(qMakePair(relPath, data));
                }
            }
        };
        traverse(root, QString());
        archive.close();
    } else if (info.isDir()) {
        // Recursively scan directory.
        // QDir dir(m_path);
        // QStringList nameFilters;
        // nameFilters << "*.*"; // all files
        // QFileInfoList list = dir.entryInfoList(nameFilters, QDir::Files | QDir::NoSymLinks, QDir::Name);
        // for (const QFileInfo &fi : list) {
        //     QFile file(fi.absoluteFilePath());
        //     if (file.open(QIODevice::ReadOnly)) {
        //         QByteArray data = file.readAll();
        //         // Compute a relative path with respect to m_path.
        //         QString relPath = dir.relativeFilePath(fi.absoluteFilePath());
        //         files.append(qMakePair(relPath, data));
        //         file.close();
        //     }
        // }
        QDir dir(m_path);
        QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoSymLinks, QDir::NoSort);
        for (const QFileInfo &fi : list) {
            QFile f(fi.absoluteFilePath());
            if (f.open(QIODevice::ReadOnly)) {
                QByteArray data = f.readAll();
                QString relPath = dir.relativeFilePath(fi.absoluteFilePath());
                files.append(qMakePair(relPath, data));
                f.close();
            }
        }
    }

    // Process each file.
    int totalFiles = files.size();
    int count = 0;
    for (const QPair<QString, QByteArray> &pair : files) {
        processFile(pair.first, pair.second);
        count++;
        int progress = (totalFiles > 0) ? (count * 100 / totalFiles) : 100;
        emit progressUpdated(progress);
    }

    // Additionally, load the pack-level files (pack.png and pack.mcmeta) if they exist.
    for (const QPair<QString, QByteArray> &pair : files) {
        if (pair.first.compare("pack.png", Qt::CaseInsensitive) == 0) {
            QImage img;
            img.loadFromData(pair.second);
            m_packPng = img;
        }
        if (pair.first.compare("pack.mcmeta", Qt::CaseInsensitive) == 0) {
            m_packMcmeta = QJsonDocument::fromJson(pair.second);
        }
    }

    m_parsed = true;
    emit parsingFinished();
}

void MCResourcePack::processFile(const QString &filePath, const QByteArray &data)
{
    // Only process files under: assets/<namespace>/(models|textures)/<category>/
    QStringList parts = filePath.split('/');
    if (parts.size() != 5 || parts[0] != "assets")
        return;

    QString ns        = parts[1];    // e.g. "minecraft"
    QString category  = parts[2];    // "models" or "textures"
    QString subtype   = parts[3];    // "block", "item", etc.
    QString filename  = parts[4];    // e.g. "fishing_rod.png" or "fishing_rod.json" or "fishing_rod.png.mcmeta"

    bool isAnimMeta = filename.endsWith(".png.mcmeta", Qt::CaseInsensitive);
    QString base    = filename;
    if (isAnimMeta) {
        base.chop(QString(".png.mcmeta").length());
    } else if (category == "textures" && filename.endsWith(".png", Qt::CaseInsensitive)) {
        base.chop(QString(".png").length());
    } else if (category == "models" && filename.endsWith(".json", Qt::CaseInsensitive)) {
        base.chop(QString(".json").length());
    } else {
        return; // skip non-model/texture files
    }

    // Build the resource key: "<namespace>:<subtype>/<base>"
    QString resKey = ns + ":" + subtype + "/" + base;

    // Find or create the corresponding element
    MCResourcePackElement *elem = nullptr;
    for (auto *e : std::as_const(m_elements)) {
        if (e->getBaseName() == resKey) {
            elem = e;
            break;
        }
    }
    if (!elem) {
        elem = new MCResourcePackElement(resKey);
        m_elements.append(elem);
    }

    // Assign data based on category and file-type
    if (category == "textures") {
        if (isAnimMeta) {
            // Load animation JSON
            QJsonDocument animDoc = QJsonDocument::fromJson(data);
            elem->setAnimation(animDoc);
        } else {
            // Load texture image
            QImage img;
            img.loadFromData(data);
            elem->setTexture(img);
        }
    }
    else if (category == "models") {
        // Load model JSON
        QJsonDocument modelDoc = QJsonDocument::fromJson(data);
        elem->setModel(modelDoc);
    }
}

QImage MCResourcePack::getPackPng(ResourceDataType type) const
{
    Q_UNUSED(type);
    return m_packPng;
}

QJsonDocument MCResourcePack::getPackMcmeta(ResourceDataType type) const
{
    Q_UNUSED(type);
    return m_packMcmeta;
}

QList<MCResourcePackElement*> MCResourcePack::getElements() const
{
    if (!m_parsed)
        return QList<MCResourcePackElement*>();
    return m_elements;
}
