#include "mergenondefaultworkerthread.h"
#include "MCResourcePack.h"

MergeNonDefaultWorkerThread::MergeNonDefaultWorkerThread(QObject *parent)
    : QThread{parent}
{}

void MergeNonDefaultWorkerThread::setPaths(const QString& p1, const QString& p2){
    this->p1path = p1;this->p2path = p2;
}

void MergeNonDefaultWorkerThread::setOpts(int modeR, int descriptionR, int nameR, int imageR,
                                          bool toMinecraftR, bool exportAsR,
                                          const QString& customDescriptionR, const QString& customNameR, const QString& customImageR, const QString& customPathR)
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
}

void MergeNonDefaultWorkerThread::setGUIThreadSlotter(GUIThreadSlotter& i){
    this->slotter = &i;
}

void MergeNonDefaultWorkerThread::acceptConflictResult(int i){
    this->conflictResult = i;
}

void MergeNonDefaultWorkerThread::run() {
    // 1) Parse both packs
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

    // 2) Mode-based merge logic on elements
    switch (mode) {
    case 0: // prioritize p1
        merged = elems1;
        for (auto *e2 : elems2)
            if (!Utils::elementExists(e2->getBaseName(), merged))
                merged.append(e2);
        break;

    case 1: // prioritize p2
        merged = elems2;
        for (auto *e1 : elems1)
            if (!Utils::elementExists(e1->getBaseName(), merged))
                merged.append(e1);
        break;

    case 2: // remove conflicts
        merged = elems1;
        for (auto *e2 : elems2) {
            if (Utils::elementExists(e2->getBaseName(), merged))
                Utils::removeElement(e2->getBaseName(), merged);
            else
                merged.append(e2);
        }
        break;

    case 3: // prompt for each conflict (simplified)
        merged = elems1;
        for (auto *e2 : elems2) {
            if (Utils::elementExists(e2->getBaseName(), merged)) {
                // In real code: signal GUI, wait, then either replace or keep.
                bool useP2 = Utils::promptConflict(e2->getBaseName());
                if (useP2) {
                    Utils::removeElement(e2->getBaseName(), merged);
                    merged.append(e2);
                }
            } else {
                merged.append(e2);
            }
        }
        break;
    }

    // 3) Export merged pack
    QString outPath = Utils::ExportMergedPack(merged,
                                              Utils::extractName(path1),
                                              Utils::extractName(path2),
                                              /* options per this thread’s settings */);
    emit doneMerging(outPath);
}
