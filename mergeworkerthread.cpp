#include "mergeworkerthread.h"
#include "MCResourcePack.h"
#include "utils.h"

MergeWorkerThread::MergeWorkerThread(QObject *parent)
    : QThread{parent}
{}

void MergeWorkerThread::setPaths(const QString& p1, const QString& p2){
    this->p1path = p1;this->p2path = p2;
}

void MergeWorkerThread::setOpts(int modeR, int descriptionR, int nameR, int imageR,
                                          bool toMinecraftR, bool exportAsR,
                                          const QString& customDescriptionR, const QString& customNameR, const QString& customImageR, const QString& customPathR, int selectedFormatR)
{
    this->mode               =               modeR;
    this->description        =        descriptionR;
    this->name               =               nameR;
    this->image              =              imageR;
    this->toMinecraft        =        toMinecraftR;
    this->exportAs           =           exportAsR;
    this->customDescription  =  customDescriptionR;
    this->customName         =         customNameR;
    this->customImage        =        customImageR;
    this->customPath         =         customPathR;
    this->selectedFormat     =     selectedFormatR;
}

void MergeWorkerThread::setGUIThreadSlotter(GUIThreadSlotter& i){
    this->slotter = &i;
}

void MergeWorkerThread::acceptConflictResult(int i){
    this->conflictResult = i;
}

void MergeWorkerThread::run() {
    //parse both packs
    MCResourcePack pack1(p1path), pack2(p2path);
    connect(&pack1, &MCResourcePack::progressUpdated, this, [this](int p){
        emit progress(0, p);
    });
    connect(&pack2, &MCResourcePack::progressUpdated, this, [this](int p){
        emit progress(1, p);
    });

    pack1.parse();
    pack2.parse();

    auto elems1 = pack1.getElements();
    auto elems2 = pack2.getElements();
    QList<MCResourcePackElement*> merged;

    //helper that finds by key
    auto findIdx = [&](const QString &key)->int {
        for (int i = 0; i < merged.size(); ++i)
            if (merged[i]->getBaseName() == key) return i;
        return -1;
    };

    //merge by mode
    if (mode == 0) {
        // p1 first, then p2 uniques
        merged = elems1;
        QSet<QString> seen;
        for (auto *e: elems1) seen.insert(e->getBaseName());
        for (auto *e2: elems2)
            if (!seen.contains(e2->getBaseName()))
                merged.append(e2);
    }
    else if (mode == 1) {
        // p2 first, then p1 uniques
        merged = elems2;
        QSet<QString> seen;
        for (auto *e: elems2) seen.insert(e->getBaseName());
        for (auto *e1: elems1)
            if (!seen.contains(e1->getBaseName()))
                merged.append(e1);
    }
    else if (mode == 2) {
        // remove any conflicts entirely
        merged = elems1;
        QSet<QString> seenP2;
        for (auto *e2: elems2) {
            auto key = e2->getBaseName();
            if (findIdx(key) >= 0) {
                merged.removeAt(findIdx(key));
            } else {
                merged.append(e2);
            }
        }
    }
    else if (mode == 3) {
        // prompt on each conflict
        merged = elems1;
        for (auto *e2 : elems2) {
            QString key = e2->getBaseName();
            int idx1 = findIdx(key);
            if (idx1 >= 0) {
                // conflict: prepare and execute dialog
                // QString img1, img2;
                // bool p1animated, p2animated;
                // //check if image
                // if(!e2->getTexture().isNull()){
                //     //extract images to tmp
                //     QByteArray imgBytes;
                //     QBuffer buf(&imgBytes);
                //     buf.open(QIODevice::WriteOnly);
                //     merged.at(idx1)->getTexture().save(&buf, "PNG");
                //     QFile tmp(".mctmp/tmpimg1");
                //     tmp.write(imgBytes);
                //     tmp.close();
                //     img1 = QFileInfo(tmp).filePath();
                //     //check if it's animated
                // }

                QEventLoop loop;

                connect(slotter, &GUIThreadSlotter::conflictResult, &loop, &QEventLoop::quit);
                connect(slotter, &GUIThreadSlotter::conflictResult, this , &MergeWorkerThread::acceptConflictResult);
                //cleanup graphics resources, etc. (you're welcome, OpenGL...)
                connect(slotter, &GUIThreadSlotter::conflictResult, this, [this](){slotter->resetDialog();});

                emit conflict(QFileInfo(p1path).baseName(), QFileInfo(p2path).baseName(), merged.at(idx1), e2);

                // auto *dlg = new ConflictPrompt(
                //     QFileInfo(p1path).baseName(), QFileInfo(p2path).baseName(),
                //     merged.at(idx1), e2);
                // dlg->exec();

                loop.exec();
                int choice = conflictResult; // 1=left, 2=right, -1 = cancel

                if(choice == -1){
                    return;
                }

                if (choice == 2) {
                    // replace with pack2’s version
                    merged[idx1] = e2;
                }
                // if choice==1, keep merged[idx1]

                //TODO: get "apply to rest" from user

                //cleanup graphics resources, etc. (you're welcome, OpenGL...)
                // slotter->resetDialog();
            } else {
                merged.append(e2);
            }
        }
    }

    emit progress(2, 20); // merging done

    //export merged pack
    QDir tmp(".mctmp");
    if (!tmp.exists()) tmp.mkpath(".");
    //if user selected a custom name, implement it
    QString baseName;
    if(name == 0)
        baseName = QString("%1+%2 [Made With MCPackTool (Mode %3)]")
                       .arg(QFileInfo(p1path).baseName(),
                            QFileInfo(p2path).baseName(),
                            QString::number(mode));
    else if(name == 1)
        baseName = QString("%1 [Made With MCPackTool (Mode %2)]")
                       .arg(QFileInfo(p1path).baseName(),
                            QString::number(mode));
    else if(name == 2)
        baseName = QString("%1 [Made With MCPackTool (Mode %2)]")
                       .arg(QFileInfo(p2path).baseName(),
                            QString::number(mode));
    else if(name == 3)
        baseName = QString("%1 [Made With MCPackTool (Mode %2)]")
                       .arg(customName, QString::number(mode));

    QString outZip = tmp.filePath(baseName + ".zip");

    //iterate over names until one is valid
    int idx = 1;
    while (QFile::exists(outZip)) {
        outZip = tmp.filePath(baseName + QString("(%1).zip").arg(idx++));
    }

    KZip zip(outZip);
    if (!zip.open(QIODevice::WriteOnly)) {
        //TODO: error handling
        emit progress(2, 100);
        emit doneMerging(QString());
        return;
    }

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

        emit progress(2, 20 + (i * 60) / totalElems); // 20–80%
    }

    // pack.mcmeta & pack.png

    //update the pack.mcmeta entry by modifying JSON content.
    QJsonDocument outputMcmeta = pack1.getPackMcmeta();
    QJsonObject outputMcmetaPack = outputMcmeta["pack"].toObject();

    if(description == 0)
        outputMcmetaPack["description"] = outputMcmetaPack["description"].toString() + " {[Merged by MCPackTool]}";
    else if(description == 1)
        outputMcmetaPack["description"] =
            pack2.getPackMcmeta()["pack"].toObject()["description"].toString() +
            " {[Merged by MCPackTool]}";
    else if(description == 2)
        outputMcmetaPack["description"] =
            outputMcmetaPack["description"].toString() + " & " +
            pack2.getPackMcmeta()["pack"].toObject()["description"].toString() +
            " {[Merged by MCPackTool]}";
    else if(description == 3)
        outputMcmetaPack["description"] = customDescription + " {[Merged by MCPackTool]}";

    outputMcmetaPack["pack_description"] = selectedFormat;

    QByteArray outputMcmetaData = QJsonDocument(outputMcmetaPack).toJson();

    const KArchiveDirectory *root = zip.directory();

    const KArchiveEntry *mcmetaEntry = root->entry("pack.mcmeta");

    bool deleteSuccess = true;

    if (mcmetaEntry)
        deleteSuccess = ((KArchiveDirectory*)root)->removeEntryV2((KArchiveEntry*)mcmetaEntry);
    zip.writeFile("pack.mcmeta", outputMcmetaData);

    //copy pack.png into the output archive based on user selection
    QByteArray packImgBytes;
    QBuffer buf(&packImgBytes);
    buf.open(QIODevice::WriteOnly);

    if(image == 0)
        pack1.getPackPng().save(&buf, "PNG");
    else if(image == 1)
        pack2.getPackPng().save(&buf, "PNG");
    else if(image == 2)
        QImage(customImage).save(&buf, "PNG");

    const KArchiveEntry *testPngEntry = root->entry("pack.png");

    if (testPngEntry)
        deleteSuccess = ((KArchiveDirectory*)root)->removeEntryV2((KArchiveEntry*)testPngEntry);

    Q_UNUSED(deleteSuccess);

    zip.writeFile("pack.png", packImgBytes);

    zip.close();
    emit progress(2, 100);

    //execute copy operations
    if(toMinecraft == 0){
        if(exportAs == 1){//export as an extracted folder
            //create the destination folder
            QString outPath = Utils::MC_PATH + "/resourcepacks/"+QFileInfo(outZip).baseName();
            QDir destination;
            destination.mkpath(outPath);

            //extract the archive: first open the archive
            if (!zip.open(QIODevice::ReadOnly)) {
                QMessageBox::critical(nullptr, "MCPackTool", "Failed to locate archive. This is a bug.", QMessageBox::Ok);
            }

            const KArchiveDirectory *rootDir = zip.directory();

            //update progress
            emit progress(3, 69);

            rootDir->copyTo(outPath, true);

            //update progress
            emit progress(3, 100);

            zip.close();

            return;
        }
        else if(!Utils::cp(outZip, Utils::MC_PATH + "/resourcepacks")){
            QMessageBox::warning(nullptr, "MCPackTool", "Failed to copy to minecraft.", QMessageBox::Ok);
            return;
        }
    }
    else{
        if(exportAs == 1){//export as an extracted folder
            //create the destination folder
            QString outPath = customPath+"/"+QFileInfo(outZip).baseName();
            QDir destination;
            destination.mkpath(outPath);

            //extract the archive: first open the archive
            if (!zip.open(QIODevice::ReadOnly)) {
                QMessageBox::critical(nullptr, "MCPackTool", "Failed to locate archive. This is a bug.", QMessageBox::Ok);
            }

            const KArchiveDirectory *rootDir = zip.directory();

            //update progress
            emit progress(3, 69);

            rootDir->copyTo(outPath, true);

            //update progress
            emit progress(3, 100);

            zip.close();

            return;
        }
        else if(!Utils::cp(outZip, customPath)){
            QMessageBox::warning(nullptr, "MCPackTool", "Failed to copy to custom folder.", QMessageBox::Ok);
            return;
        }
    }

    emit doneMerging(outZip);
}
