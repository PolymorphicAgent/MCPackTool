#ifndef GUITHREADSLOTTER_H
#define GUITHREADSLOTTER_H

#include <QObject>
#include <QWidget>

class GUIThreadSlotter : public QWidget
{
    Q_OBJECT
public:
    explicit GUIThreadSlotter(QWidget *parent = nullptr);
public slots:
    void conflictPrompter(const QString&, const QString&, const QString&,
                          const QString&, const QString&, const QString&);
signals:
    void conflictResult(int);
};

#endif // GUITHREADSLOTTER_H
