#ifndef GUITHREADSLOTTER_H
#define GUITHREADSLOTTER_H

#include "MCResourcePack.h"

#include <QObject>
#include <QWidget>

class GUIThreadSlotter : public QWidget
{
    Q_OBJECT
public:
    explicit GUIThreadSlotter(QWidget *parent = nullptr);
public slots:
    void conflictPrompter(const QString&, const QString&, MCResourcePackElement*, MCResourcePackElement*);
signals:
    void conflictResult(int);
};

#endif // GUITHREADSLOTTER_H
