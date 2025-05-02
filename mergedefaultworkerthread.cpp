#include "mergedefaultworkerthread.h"
#include "MCResourcePack.h"
#include "utils.h"

MergeDefaultWorkerThread::MergeDefaultWorkerThread(QObject *parent)
    : QThread{parent}
{}

void MergeDefaultWorkerThread::setPaths(const QString& p1, const QString& p2){
    this->p1path = p1;
    this->p2path = p2;
}

void MergeDefaultWorkerThread::run() {
    // 1) Load both packs
    MCResourcePack pack1(p1path), pack2(p2path);
    connect(&pack1, &MCResourcePack::progressUpdated, this, [this](int p){
        emit progress(0, p);
    });
    connect(&pack2, &MCResourcePack::progressUpdated, this, [this](int p){
        emit progress(1, p);
    });

    pack1.parse();  // emits progress 0–100
    pack2.parse();  // emits progress 0–100

    // 2) Get element lists
    auto elems1 = pack1.getElements();
    auto elems2 = pack2.getElements();

    // 3) Merge default: all elems1, then any elems2 not in elems1 by baseName
    QList<MCResourcePackElement*> merged = elems1;
    QSet<QString> seen;
    for (auto *e : elems1) seen.insert(e->getBaseName());

    int total = elems2.size();
    for (int i = 0; i < total; ++i) {
        auto *e2 = elems2[i];
        if (!seen.contains(e2->getBaseName())) {
            merged.append(e2);
        }
        emit progress(2, (i * 100) / total );
    }

    // 4) Export via Utils helper (creates ZIP, pack.mcmeta, pack.png)
    QString outPath = Utils::ExportMergedPack(merged,
                                              Utils::extractName(path1),
                                              Utils::extractName(path2),
                                              /* default options */);
    emit doneMerging(outPath);
}

// // A recursive helper to collect entries that start with a given prefix.
// // Note: For simplicity, we build the full path by concatenating parentPath and the entry’s name.
// void MergeDefaultWorkerThread::collectEntries(const KArchiveDirectory *dir,
//                                               QVector<const KArchiveEntry*>& outEntries,
//                                               QSet<QString>& outNames,
//                                               const QString &prefix,
//                                               const QString &parentPath)
// {
//     // Get the names of all entries directly under this directory.
//     QStringList names = dir->entries();
//     for (const QString &name : names) {
//         // Build the full path (using '/' as a separator)
//         QString fullPath = parentPath.isEmpty() ? name : parentPath + "/" + name;
//         const KArchiveEntry *entry = dir->entry(name);
//         // Check if the full path begins with the required prefix.
//         if (fullPath.startsWith(prefix)) {
//             if (!outNames.contains(fullPath)) {
//                 outEntries.append(entry);
//                 outNames.insert(fullPath);
//             }
//         }
//         // If the entry is a directory, recurse into it.
//         if (entry->isDirectory()) {
//             const KArchiveDirectory *subDir = static_cast<const KArchiveDirectory*>(entry);
//             collectEntries(subDir, outEntries, outNames, prefix, fullPath);
//         }
//     }
// }
