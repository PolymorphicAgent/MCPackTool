#ifndef GUITHREADSLOTTER_H
#define GUITHREADSLOTTER_H

#include "MCResourcePack.h"
#include "conflictprompt.h"

#include <QObject>
#include <QWidget>

class GUIThreadSlotter : public QWidget
{
    Q_OBJECT
public:
    explicit GUIThreadSlotter(QWidget *parent = nullptr);
public slots:
    void conflictPrompter(const QString&, const QString&, MCResourcePackElement*, MCResourcePackElement*);
    void resetDialog();
signals:
    void conflictResult(int);
private:
    ConflictPrompt *pro;
};

#endif // GUITHREADSLOTTER_H
