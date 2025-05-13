#ifndef MERGEDEFAULTWORKERTHREAD_H
#define MERGEDEFAULTWORKERTHREAD_H

#include <QFileInfo>
#include <QThread>
#include <QBuffer>
#include <QFile>
#include <QDir>

#include <KArchiveDirectory>
#include <KArchiveFile>
#include <KZip>

class MergeDefaultWorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit MergeDefaultWorkerThread(QObject *parent = nullptr);
    void setPaths(const QString&, const QString&);
    void run() override;
    // {
    //     /* ... here is the expensive or blocking operation ...
    //      *
    //      *  -> prioritize pack 1 nodes in all cases
    //      *  -> use pack1 + pack2's + {[Merged by MCPackTool]} name
    //      *  -> export as zip
    //      *
    //      *  What info we need from call:
    //      *  -> Pack 1 ZipArchive::Ptr
    //      *  -> Pack 2 ZipArchive::Ptr
    //      *
    //      */

    //     QVector<const KArchiveEntry*>mergedEntries;
    //     QSet<QString> mergedNames;

    //     // // Process pack1 entries and add assets entries to merged vector.
    //     // int p1count = static_cast<int>(pack1->GetEntriesCount());
    //     // for (int i = 0; i < p1count; ++i) {
    //     //     auto entry = pack1->GetEntry(i);
    //     //     QString fullName = QString::fromStdString(entry->GetFullName());
    //     //     if (fullName.startsWith("assets/")) {
    //     //         merged.push_back(entry);
    //     //         mergedNames.insert(fullName);
    //     //     }
    //     //     // Use floating-point division to update progress smoothly.
    //     //     emit progress(static_cast<int>((static_cast<double>(i) / p1count) * 10));
    //     // }

    //     // Collect from pack1
    //     {
    //         const KArchiveDirectory *root = pack1->directory();
    //         collectEntries(root, mergedEntries, mergedNames, "assets");
    //         int count = mergedEntries.size();
    //         // Update progress based on pack1 processing – here we mimic progress
    //         for (int i = 0; i < count; ++i) {
    //             emit progress(static_cast<int>((static_cast<double>(i) / count) * 10));
    //         }
    //     }


    //     // // Process pack2, add assets entries that aren't already in mergedNames.
    //     // int p2count = static_cast<int>(pack2->GetEntriesCount());
    //     // for (int i = 0; i < p2count; ++i) {
    //     //     auto entry = pack2->GetEntry(i);
    //     //     QString fullName = QString::fromStdString(entry->GetFullName());
    //     //     if (fullName.startsWith("assets/") && !mergedNames.contains(fullName)) {
    //     //         merged.push_back(entry);
    //     //         mergedNames.insert(fullName);
    //     //     }
    //     //     emit progress(10 + static_cast<int>((static_cast<double>(i) / p2count) * 10));
    //     // }

    //     // Collect from pack2 and only add those not already in mergedNames
    //     {
    //         const KArchiveDirectory *root = pack2->directory();
    //         QVector<const KArchiveEntry*> tempEntries;
    //         collectEntries(root, tempEntries, mergedNames, "assets");
    //         // Append the new entries from pack2
    //         mergedEntries += tempEntries;
    //         int count = tempEntries.size();
    //         for (int i = 0; i < count; ++i) {
    //             emit progress(10 + static_cast<int>((static_cast<double>(i) / count) * 10));
    //         }
    //     }

    //     // //first check if it already exists
    //     // QString outputName=Utils::extractName(p1path)+"+"+Utils::extractName(p2path)+"[Merged by MCPackTool]";

    //     // if(QFile::exists(".mctmp/"+outputName+".zip"))
    //     //     for(int i=1;true;i++){
    //     //         if(!QFile::exists(".mctmp/"+outputName+"("+QString::number(i)+").zip")){
    //     //             outputName += "("+QString::number(i)+").zip";
    //     //             break;
    //     //         }
    //     //     }
    //     // else
    //     //     outputName += ".zip";

    //     // Construct the output file name.
    //     QString baseName = Utils::extractName(p1path) + "+" + Utils::extractName(p2path) + "[Merged by MCPackTool]";
    //     QString outputFilePath = QDir::currentPath() + "/.mctmp/" + baseName + ".zip";
    //     QDir tmpDir(".mctmp");
    //     if (!tmpDir.exists())
    //         tmpDir.mkpath(".");

    //     // If a file with that name already exists, modify the name.
    //     if(QFile::exists(outputFilePath)) {
    //         int i = 1;
    //         while (QFile::exists(QDir(".mctmp").absoluteFilePath(baseName + "(" + QString::number(i) + ").zip"))) {
    //             ++i;
    //         }
    //         outputFilePath = QDir(".mctmp").absoluteFilePath(baseName + "(" + QString::number(i) + ").zip");
    //     }

    //     // //open the output zip file
    //     // output = ZipFile::Open(".mctmp/"+outputName.toStdString());

    //     // int i = 0, t = (int)merged.size();
    //     // for(auto& entry :merged){
    //     //     i++;//keep track of iteration for progress
    //     //     QString fullName = QString::fromStdString(entry->GetFullName());
    //     //     if(fullName.back()!='/'){
    //     //         ZipArchiveEntry::Ptr newEntry = output->CreateEntry(entry->GetFullName());
    //     //         // newEntry->UseDataDescriptor();
    //     //         newEntry->SetCompressionStream(*entry->GetDecompressionStream(),
    //     //                                        DeflateMethod::Create(),
    //     //                                        ZipArchiveEntry::CompressionMode::Immediate);
    //     //     }else{
    //     //         output->CreateEntry(entry->GetFullName());
    //     //     }
    //     //     emit progress(20 + static_cast<int>((static_cast<double>(i) / t) * 80));
    //     // }

    //     // // ZipFile::SaveAndClose(output, ".mctmp/"+outputName.toStdString()); //oh no this was so terrible

    //     // Create the output ZIP archive.
    //     output = new KZip(outputFilePath);
    //     if (!output->open(QIODevice::WriteOnly)) {
    //         qWarning() << "Failed to open output archive:" << outputFilePath;
    //         return;
    //     }

    //     // Write each collected entry into the output.
    //     int total = mergedEntries.size();
    //     for (int i = 0; i < total; ++i) {
    //         const KArchiveEntry *entry = mergedEntries.at(i);
    //         QString fullName = entry->name();
    //         // For the output, you may want to use the full (relative) path.
    //         if (!entry->isDirectory()) {
    //             // For file entries, cast to KArchiveFile and retrieve its data.
    //             const KArchiveFile *fileEntry = static_cast<const KArchiveFile*>(entry);
    //             QByteArray fileData = fileEntry->data();
    //             // Write the file into the output archive.
    //             // (permissions, owner, group are set as examples)
    //             if (!output->writeFile(fullName, fileData, 0100644, "owner", "users")) {
    //                 qWarning() << "Failed to write file entry:" << fullName;
    //             }
    //         } else {
    //             // Write a directory entry.
    //             if (!output->writeDir(fullName, "owner", "users", 040755)) {
    //                 qWarning() << "Failed to write directory entry:" << fullName;
    //             }
    //         }
    //         // Update progress over the 80% portion (from 20% to 100%).
    //         emit progress(20 + static_cast<int>((static_cast<double>(i + 1) / total) * 80));
    //     }
    //     output->close();

    //     emit progress(100);
    //     emit doneMerging(outputFilePath, output);
    // }
signals:
    void doneMerging(QString);
    void progress(int bar, int progress);
private:
    // Helper: Recursively add entries from a KArchiveDirectory that match a given prefix.
    // void collectEntries(const KArchiveDirectory *dir,
    //                     QVector<const KArchiveEntry*>& outEntries,
    //                     QSet<QString>& outNames,
    //                     const QString &prefix,
    //                     const QString &parentPath = QString());

    // KZip *pack1 = nullptr;
    // KZip *pack2 = nullptr;
    // KZip *output = nullptr;
    QString p1path, p2path;
};

#endif // MERGEDEFAULTWORKERTHREAD_H
