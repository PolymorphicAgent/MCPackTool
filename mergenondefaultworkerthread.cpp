#include "mergenondefaultworkerthread.h"

MergeNonDefaultWorkerThread::MergeNonDefaultWorkerThread(QObject *parent)
    : QThread{parent}
{}

void MergeNonDefaultWorkerThread::setPtrs(ZipArchive::Ptr& p1, ZipArchive::Ptr& p2){
    this->pack1 = p1;
    this->pack2 = p2;
}

void MergeNonDefaultWorkerThread::setPaths(const QString& p1, const QString& p2){
    this->p1path = p1;
    this->p2path = p2;
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
