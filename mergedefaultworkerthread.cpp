#include "mergedefaultworkerthread.h"

MergeDefaultWorkerThread::MergeDefaultWorkerThread(QObject *parent)
    : QThread{parent}
{}

void MergeDefaultWorkerThread::setPtrs(ZipArchive::Ptr& p1, ZipArchive::Ptr& p2){
    this->pack1 = p1;
    this->pack2 = p2;
}

void MergeDefaultWorkerThread::setPaths(const QString& p1, const QString& p2){
    this->p1path = p1;
    this->p2path = p2;
}
