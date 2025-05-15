#include "mergedefaultworkerthread.h"
#include "MCResourcePack.h"
// #include "utils.h"

MergeDefaultWorkerThread::MergeDefaultWorkerThread(QObject *parent)
    : QThread{parent}
{}

void MergeDefaultWorkerThread::setPaths(const QString& p1, const QString& p2){
    this->p1path = p1;
    this->p2path = p2;
}

void MergeDefaultWorkerThread::run() {
    //load both packs
    MCResourcePack pack1(p1path), pack2(p2path);
    connect(&pack1, &MCResourcePack::progressUpdated, this, [this](int p){
        emit progress(0, p);
    });
    connect(&pack2, &MCResourcePack::progressUpdated, this, [this](int p){
        emit progress(1, p);
    });

    pack1.parse();  // emits progress 0–100
    pack2.parse();  // emits progress 0–100

    //get element lists
    auto elems1 = pack1.getElements();
    auto elems2 = pack2.getElements();

    //merge default: all elems1, then any elems2 not in elems1 by baseName
    QList<MCResourcePackElement*> merged = elems1;
    QSet<QString> seen;
    for (auto *e : elems1) seen.insert(e->getBaseName());

    int total = elems2.size();
    for (int i = 0; i < total; ++i) {
        auto *e2 = elems2[i];
        if (!seen.contains(e2->getBaseName())) {
            merged.append(e2);
        }
        emit progress(2, (i * 10) / total + 10 );  // 10–20% for merging
    }

    // --- Build output ZIP via KArchive ---
    //determine unique output path
    QDir tmp(".mctmp");
    if (!tmp.exists()) tmp.mkpath(".");

    QString baseName = QString("%1+%2[Merged By MCPackTool]").arg(
        QFileInfo(p1path).baseName(),
        QFileInfo(p2path).baseName());
    QString outZip = tmp.filePath(baseName + ".zip");

    //keep iterating until a valid file name is found
    int idx = 1;
    while (QFile::exists(outZip)) {
        outZip = tmp.filePath(baseName + QString("(%1).zip").arg(idx++));
    }

    //create and open the zip
    KZip zip(outZip);
    if (!zip.open(QIODevice::WriteOnly)) {
        //TODO: error handling
        emit progress(2, 100);
        emit doneMerging(QString());
        return;
    }

    // auto root = zip.directory();

    //export each element’s files
    int totalElems = merged.size();
    for (int i = 0; i < totalElems; ++i) {
        auto *e = merged[i];
        QString key     = e->getBaseName();             // e.g. "minecraft:item/fishing_rod"
        auto parts      = key.split(':').last().split('/');
        QString subtype = parts[0], base = parts[1];
        QString ns      = key.section(':', 0, 0);

        //texture
        QImage tex = e->getTexture();
        if (!tex.isNull()) {
            QByteArray imgBytes;
            QBuffer buf(&imgBytes);
            buf.open(QIODevice::WriteOnly);
            tex.save(&buf, "PNG");
            QString arcPath = QString("assets/%1/textures/%2/%3.png")
                                  .arg(ns, subtype, base);
            zip.writeFile(arcPath, imgBytes);
            // auto fileEntry = new KArchiveFile(arcPath, &zip);
            // fileEntry->write(imgBytes);
            // delete fileEntry;
        }

        //animation
        if (e->isAnimated()) {
            QJsonDocument animDoc = e->getAnimation();
            QByteArray jsonBytes = animDoc.toJson(QJsonDocument::Compact);
            QString arcPath = QString("assets/%1/textures/%2/%3.png.mcmeta")
                                  .arg(ns, subtype, base);
            zip.writeFile(arcPath, jsonBytes);
            // auto fileEntry = new KArchiveFile(arcPath, &zip);
            // fileEntry->write(jsonBytes);
            // delete fileEntry;
        }

        //model
        QJsonDocument mdl = e->getModel();
        if (!mdl.isNull()) {
            QByteArray jsonBytes = mdl.toJson(QJsonDocument::Compact);
            QString arcPath = QString("assets/%1/models/%2/%3.json")
                                  .arg(ns, subtype, base);
            zip.writeFile(arcPath, jsonBytes);
            // auto fileEntry = new KArchiveFile(arcPath, &zip);
            // fileEntry->write(jsonBytes);
            // delete fileEntry;
        }

        emit progress(2, 20 + (i * 60) / totalElems); // 20–80% exporting
    }

    //write pack.mcmeta
    {
        QJsonDocument meta = pack1.getPackMcmeta();
        QByteArray jb = meta.toJson(QJsonDocument::Compact);
        zip.writeFile("pack.mcmeta", jb);
        // auto fileEntry = new KArchiveFile("pack.mcmeta", &zip);
        // fileEntry->write(jb);
        // delete fileEntry;
    }
    emit progress(2, 85);

    //write pack.png (if any)
    {
        QImage img = pack1.getPackPng();
        if (!img.isNull()) {
            QByteArray imgBytes;
            QBuffer buf(&imgBytes);
            buf.open(QIODevice::WriteOnly);
            img.save(&buf, "PNG");
            zip.writeFile("pack.png", imgBytes);
            // auto fileEntry = new KArchiveFile("pack.png", &zip);
            // fileEntry->write(imgBytes);
            // delete fileEntry;
        }
    }
    emit progress(2, 90);

    zip.close();
    emit progress(2, 100);
    emit doneMerging(outZip);

}
