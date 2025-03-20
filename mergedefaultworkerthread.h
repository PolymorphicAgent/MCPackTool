#ifndef MERGEDEFAULTWORKERTHREAD_H
#define MERGEDEFAULTWORKERTHREAD_H

#include <istream>

#include <QThread>
#include <QFile>

#include <ZipFile.h>
// #include <ZipArchive.h>
// #include <ZipArchiveEntry.h>

#include "utils.h"

class MergeDefaultWorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit MergeDefaultWorkerThread(QObject *parent = nullptr);
    void setPtrs(ZipArchive::Ptr&, ZipArchive::Ptr&);
    void setPaths(const QString&, const QString&);
    void run() override {
        /* ... here is the expensive or blocking operation ...
         *
         *  -> prioritize pack 1 nodes in all cases
         *  -> use pack1 + pack2's + {[Merged by MCPackTool]} name
         *  -> export as zip
         *
         *  What info we need from call:
         *  -> Pack 1 ZipArchive::Ptr
         *  -> Pack 2 ZipArchive::Ptr
         *
         */

        std::vector<ZipArchiveEntry::Ptr>merged;
        QSet<QString> mergedNames;

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

        //first check if it already exists
        QString outputName=Utils::extractName(p1path)+"+"+Utils::extractName(p2path)+"[Merged by MCPackTool]";

        if(QFile::exists(".mctmp/"+outputName+".zip"))
            for(int i=1;true;i++){
                if(!QFile::exists(".mctmp/"+outputName+"("+QString::number(i)+").zip")){
                    outputName += "("+QString::number(i)+").zip";
                    break;
                }
            }
        else
            outputName += ".zip";

        //open the output zip file
        output = ZipFile::Open(".mctmp/"+outputName.toStdString());

        int i = 0, t = (int)merged.size();
        for(auto& entry :merged){
            i++;//keep track of iteration for progress
            QString fullName = QString::fromStdString(entry->GetFullName());
            if(fullName.back()!='/'){
                ZipArchiveEntry::Ptr newEntry = output->CreateEntry(entry->GetFullName());
                // newEntry->UseDataDescriptor();
                newEntry->SetCompressionStream(*entry->GetDecompressionStream(),
                                               DeflateMethod::Create(),
                                               ZipArchiveEntry::CompressionMode::Immediate);
            }else{
                output->CreateEntry(entry->GetFullName());
            }
            emit progress(20 + static_cast<int>((static_cast<double>(i) / t) * 80));
        }

        // ZipFile::SaveAndClose(output, ".mctmp/"+outputName.toStdString()); //oh no this was so terrible

        emit progress(100);

        emit doneMerging(".mctmp/"+outputName, output);
    }
signals:
    void doneMerging(QString,ZipArchive::Ptr);
    void progress(int);
private:
    ZipArchive::Ptr pack1;
    ZipArchive::Ptr pack2;
    ZipArchive::Ptr output;
    QString p1path, p2path;
};

#endif // MERGEDEFAULTWORKERTHREAD_H
