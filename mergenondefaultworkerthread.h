#ifndef MERGENONDEFAULTWORKERTHREAD_H
#define MERGENONDEFAULTWORKERTHREAD_H

#include <istream>
#include <fstream>

#include <QEventLoop>
#include <QFileInfo>
#include <QThread>
#include <QTimer>
#include <QFile>
#include <QDir>

#include <ZipFile.h>

#include "utils.h"
#include "guithreadslotter.h"

class MergeNonDefaultWorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit MergeNonDefaultWorkerThread(QObject *parent = nullptr);
    void setPtrs(ZipArchive::Ptr&, ZipArchive::Ptr&);
    void setPaths(const QString&, const QString&);
    void setOpts(int, int, int, int, bool, bool, const QString&, const QString&, const QString&, const QString&);
    void setGUIThreadSlotter(GUIThreadSlotter&);
    void run() override {
        /* ... here is the expensive or blocking operation ...
         *
         *  -> do setters and not getters for all the settings
         *
         *  What info we need from call:
         *  -> Pack 1 ZipArchive::Ptr
         *  -> Pack 2 ZipArchive::Ptr
         *
         * --- MAPPINGS (relative and respective) ---
         * int modeR: 0, 1, 2, 3
         *   -> prioritize p1 nodes, prioritize p2 nodes, leave out both, prompt at every conflict
         * int descriptionR: 0, 1, 2, 3
         *   -> p1 description, p2 description, p1+p2 descriptions, custom description
         * QString customDescriptionR:  "ANY STRING"
         *   -> Don't read unless 'descriptionR' is 3; holds the custom description
         * int nameR: 0, 1, 2, 3
         *   -> p1+p2 filename, p1 filename, p2 filename, custom name
         * QString customNameR:  "ANYSTRINGWITHOUTSPACES"
         *   -> Don't read unless 'nameR' is 3; holds the custom filename
         * int imageR: 0, 1, 2
         *   -> p1 image, p2 image, custom image
         * QString customImageR:  "C:/Users/Peter/Pictures/dead-henry.png"
         *   -> Don't read unless 'imageR' is 2; holds the path to the custom image (already checked if it's square)
         * int toMinecraftR: 0, 1
         *   -> export to minecraft, export to custom folder
         * QString customPathR:  "C:/Users/Peter/Documents/MCPackTool"
         *   -> Don't read unless 'customPathR' is 1; holds the path to the custom output directory
         * int exportAsR: 0, 1
         *   -> export as zipped archive, export as unpacked folder
         */

        std::vector<ZipArchiveEntry::Ptr>merged;
        QSet<QString> mergedNames;

        if(mode == 0){//prioritize p1 nodes
            qDebug()<<"Priority: p1 nodes (0)";

            // int p1count = static_cast<int>(pack1->GetEntriesCount());
            // for(int i=0;i<p1count;i++){
            //     if(pack1->GetEntry(i)->GetFullName().length()>7&&
            //         pack1->GetEntry(i)->GetFullName().substr(0,7)=="assets/")
            //     {
            //         merged.push_back(pack1->GetEntry(i));
            //         emit progress(static_cast<int>((static_cast<double>(i)/p1count)*10));
            //     }
            // }

            // int p2count = static_cast<int>(pack2->GetEntriesCount());
            // for(int i=0;i<p2count;i++){
            //     if(pack2->GetEntry(i)->GetFullName().length()>7&&
            //         pack2->GetEntry(i)->GetFullName().substr(0,7)=="assets/")
            //     {
            //         if(!Utils::find(QString::fromStdString(pack2->GetEntry(i)->GetFullName()), merged))
            //             merged.push_back(pack2->GetEntry(i));
            //         emit progress(10+(i/p2count)*10);//keep track of progress
            //     }
            // }

            // Process pack1 entries and add assets entries to merged vector.
            int p1count = static_cast<int>(pack1->GetEntriesCount());
            for (int i = 0; i < p1count; ++i) {
                auto entry = pack1->GetEntry(i);
                QString fullName = QString::fromStdString(entry->GetFullName());
                if (fullName.startsWith("assets/")) {
                    merged.push_back(entry);
                    mergedNames.insert(fullName);
                }
                // Use floating-point division to update progress smoothly.
                emit progress(static_cast<int>((static_cast<double>(i) / p1count) * 10));
            }

            // Process pack2, add assets entries that aren't already in mergedNames.
            int p2count = static_cast<int>(pack2->GetEntriesCount());
            for (int i = 0; i < p2count; ++i) {
                auto entry = pack2->GetEntry(i);
                QString fullName = QString::fromStdString(entry->GetFullName());
                if (fullName.startsWith("assets/") && !mergedNames.contains(fullName)) {
                    merged.push_back(entry);
                    mergedNames.insert(fullName);
                }
                emit progress(10 + static_cast<int>((static_cast<double>(i) / p2count) * 10));
            }
        }
        else if(mode == 1){//prioritize p2 nodes
            qDebug()<<"Priority: p2 nodes (1)";

            // int p2count = static_cast<int>(pack2->GetEntriesCount());
            // for(int i=0;i<p2count;i++){
            //     if(pack2->GetEntry(i)->GetFullName().length()>7&&
            //         pack2->GetEntry(i)->GetFullName().substr(0,7)=="assets/")
            //     {
            //         merged.push_back(pack2->GetEntry(i));
            //         emit progress((i/p2count)*10);//keep track of progress
            //     }
            // }

            // int p1count = static_cast<int>(pack1->GetEntriesCount());
            // for(int i=0;i<p1count;i++){
            //     if(pack1->GetEntry(i)->GetFullName().length()>7&&
            //         pack1->GetEntry(i)->GetFullName().substr(0,7)=="assets/")
            //     {
            //         if(!Utils::find(QString::fromStdString(pack1->GetEntry(i)->GetFullName()), merged))
            //             merged.push_back(pack1->GetEntry(i));
            //         emit progress(10+(i/p1count)*10);//keep track of progress
            //     }
            // }

            // Process pack2 entries and add assets entries to merged vector.
            int p2count = static_cast<int>(pack2->GetEntriesCount());
            for (int i = 0; i < p2count; ++i) {
                auto entry = pack2->GetEntry(i);
                QString fullName = QString::fromStdString(entry->GetFullName());
                if (fullName.startsWith("assets/")) {
                    merged.push_back(entry);
                    mergedNames.insert(fullName);
                }
                // Use floating-point division to update progress smoothly.
                emit progress(static_cast<int>((static_cast<double>(i) / p2count) * 10));
            }

            // Process pack2, add assets entries that aren't already in mergedNames.
            int p1count = static_cast<int>(pack1->GetEntriesCount());
            for (int i = 0; i < p1count; ++i) {
                auto entry = pack1->GetEntry(i);
                QString fullName = QString::fromStdString(entry->GetFullName());
                if (fullName.startsWith("assets/") && !mergedNames.contains(fullName)) {
                    merged.push_back(entry);
                    mergedNames.insert(fullName);
                }
                emit progress(10 + static_cast<int>((static_cast<double>(i) / p1count) * 10));
            }

        }
        else if(mode == 2){//leave out both nodes
            qDebug()<<"Priority: leave out both nodes (2)";
            int p1count = static_cast<int>(pack1->GetEntriesCount());
            for(int i=0;i<p1count;i++){
                auto entry = pack1->GetEntry(i);
                QString fullName = QString::fromStdString(entry->GetFullName());
                if(fullName.startsWith("assets/") && !mergedNames.contains(fullName)){
                    merged.push_back(entry);
                    mergedNames.insert(QString::fromStdString(entry->GetFullName()));
                    emit progress(static_cast<int>((static_cast<double>(i)/p1count)*10));
                }
            }

            int p2count = static_cast<int>(pack2->GetEntriesCount());
            for(int i=0;i<p2count;i++){
                auto entry = pack2->GetEntry(i);
                QString fullName = QString::fromStdString(entry->GetFullName());
                if(fullName.startsWith("assets/")){
                    if(mergedNames.contains(fullName)){
                        merged.erase(merged.begin()+Utils::iFind(QString::fromStdString(pack2->GetEntry(i)->GetFullName()), merged));
                    }
                    emit progress(10+static_cast<int>((static_cast<double>(i)/p2count)*10));//keep track of progress
                }
            }
        }
        else if(mode == 3){//prompt at every conflict (oh no)
            qDebug()<<"Priority: prompt at every node conflict (3)";
            //add pack1 asset entries
            int p1count = static_cast<int>(pack1->GetEntriesCount());
            for(int i=0;i<p1count;i++){
                auto entry = pack1->GetEntry(i);
                QString fullName = QString::fromStdString(entry->GetFullName());
                if(fullName.startsWith("assets/") && !mergedNames.contains(fullName)){
                    merged.push_back(entry);
                    mergedNames.insert(QString::fromStdString(entry->GetFullName()));
                    emit progress(static_cast<int>((static_cast<double>(i)/p1count)*10));
                }
            }

            //cleanup from (possible) previous runs
            if(QFile::exists(".mctmp/pack1image.png.tmp"))
                QFile::remove(".mctmp/pack1image.png.tmp");

            if(QFile::exists(".mctmp/pack2image.png.tmp"))
                QFile::remove(".mctmp/pack2image.png.tmp");

            if(QFile::exists(".mctmp/downer.tmp"))
                QFile::remove(".mctmp/downer.tmp");

            int p2count = static_cast<int>(pack2->GetEntriesCount());
            // int tmp = 0;
            for(int i=0;i<p2count;i++){
                auto entry = pack2->GetEntry(i);
                std::string fullNameStr = entry->GetFullName();
                QString assetName = QString::fromStdString(fullNameStr);
                if(assetName.startsWith("assets/")&&
                    !pack2->GetEntry(i)->IsDirectory()&&
                    mergedNames.contains(assetName))
                {
                    //deal w/ conflict
                    QString p1imagepath, p2imagepath, p1imagename, p2imagename;
                    // tmp++;
                    // a bunch of poorly optimized crap just to try to make it barely work

                    //the most important 5 lines of code in this whole project
                    bool thinksHesCool = true,isActuallyCool = false, IH8Redst0ne = true;
                    if(IH8Redst0ne == thinksHesCool) qDebug()<<"IH8Redst0ne thinks he's cool!";
                    else qDebug()<<"IH8Redst0ne doesn't think he's cool!";
                    if(IH8Redst0ne == isActuallyCool) qDebug()<<"IH8Redst0ne is actually cool!";
                    else qDebug()<<"IH8Redst0ne isn't actually cool!";

                    std::ofstream downer;
                    downer.open(".mctmp/downer.tmp", std::ios::binary);
                    downer<<pack2->GetEntry(i)->GetDecompressionStream()->rdbuf();
                    downer.close();
                    pack2->GetEntry(i)->CloseDecompressionStream();

                    std::ifstream goober;
                    goober.open(".mctmp/downer.tmp", std::ios::binary);

                    //check if conflict is json/mcmeta
                    if(Utils::isJson(&goober)){
                        //if so, use json icons and continue to prompt
                        // qDebug()<<"I'm a JSON!";
                        p1imagepath = ":/images/JSON.png";p2imagepath = ":/images/JSON.png";
                        p1imagename = Utils::extractName(QString::fromStdString(pack1->GetEntry(Utils::iFind(
                                                                                                  QString::fromStdString(
                                                                                                  pack2->GetEntry(i)->GetFullName()), merged))
                                                                                                  ->GetFullName()));
                        p2imagename = Utils::extractName(QString::fromStdString(pack2->GetEntry(i)->GetFullName()));
                    }
                    //check conflict is image
                    else if(Utils::isImage(&goober)){
                        //if so, extract images and continue to prompt
                        // qDebug()<<"I'm an image!";

                        //get the entry for the pack1 conflict (verified, it works)
                        int p1entry = Utils::iFind(QString::fromStdString(pack2->GetEntry(i)->GetFullName()), merged);

                        //extract the image from pack1 to temp file (verified, it works)
                        std::ofstream f;
                        f.open(".mctmp/pack1image.png.tmp", std::ios::binary);
                        f<<pack1->GetEntry(p1entry)->GetDecompressionStream()->rdbuf();
                        f.close();

                        //set args for conflictprompt dialog
                        p1imagepath = ".mctmp/pack1image.png.tmp";
                        std::string fpath = pack1->GetEntry(p1entry)->GetFullName();
                        p1imagename = Utils::extractName(QString::fromStdString(fpath));

                        //reuse downer
                        goober.close();
                        QFile::rename(".mctmp/downer.tmp", ".mctmp/pack2image.png.tmp");

                        //set args for conflictprompt dialog
                        p2imagepath = ".mctmp/pack2image.png.tmp";
                        fpath = pack2->GetEntry(i)->GetFullName();
                        p2imagename = Utils::extractName(QString::fromStdString(fpath));

                    }
                    //if neither
                    else {
                        //if so, use unknown icon representations and continue to prompt
                        // qDebug()<<"Idk what the hell I am!";
                        p1imagepath = ":/images/UNKNOWN.png";p2imagepath = ":/images/UNKNOWN.png";
                        p1imagename = Utils::extractName(QString::fromStdString(
                            pack1->GetEntry(Utils::iFind(QString::fromStdString(pack2->GetEntry(i)->GetFullName()), merged))->GetFullName()));
                        p2imagename = Utils::extractName(QString::fromStdString(pack2->GetEntry(i)->GetFullName()));
                    }

                    goober.close();

                    QEventLoop loop;

                    connect(slotter, &GUIThreadSlotter::conflictResult, &loop, &QEventLoop::quit);
                    connect(slotter, &GUIThreadSlotter::conflictResult, this , &MergeNonDefaultWorkerThread::acceptConflictResult);

                    emit conflict(Utils::extractName(p1path), p1imagename, p1imagepath, Utils::extractName(p2path), p2imagename, p2imagepath);

                    loop.exec();

                    // qDebug()<<conflictResult;

                    //cleanup just in case
                    if(QFile::exists(".mctmp/pack1image.png.tmp"))
                        QFile::remove(".mctmp/pack1image.png.tmp");

                    if(QFile::exists(".mctmp/pack2image.png.tmp"))
                        QFile::remove(".mctmp/pack2image.png.tmp");

                    if(QFile::exists(".mctmp/downer.tmp"))
                        QFile::remove(".mctmp/downer.tmp");

                    emit progress(10+(i/p2count)*10);//keep track of progress
                }
            }

            // qDebug()<<"tmp: "<<tmp;
            // qDebug()<<"merged: "<<Utils::stringify(merged);
            //whoah not even close to being done so im lazy so ill put this project on hold
        }
    }
public slots:
    void acceptConflictResult(int);
signals:
    void doneMerging(QString,ZipArchive::Ptr);
    void conflict(const QString&, const QString&, const QString&,
                  const QString&, const QString&, const QString&);
    void progress(int);
private:
    ZipArchive::Ptr pack1;
    ZipArchive::Ptr pack2;
    ZipArchive::Ptr output;

    //set before exec
    int mode, description, name, image;
    bool toMinecraft, exportAs;
    QString customDescription, customName, customImage, customPath;
    GUIThreadSlotter *slotter;
    //end set before exec

    QString p1path, p2path;

    int conflictResult;
};

#endif // MERGENONDEFAULTWORKERTHREAD_H
