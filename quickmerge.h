#ifndef QUICKMERGE_H
#define QUICKMERGE_H

#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QFrame>
#include <QWidget>
#include "quickmergesettings.h"

class QuickMerge : public QWidget
{
    Q_OBJECT
public:
    explicit QuickMerge(QWidget *parent = nullptr);
private slots:
    void selectL();
    void selectR();
    void settingss();
    void recievedSettingsUpdate(int, int, QString, int, QString, int, QString, int, QString, int);
    void init();
private:
    void reset();
    QPushButton         *pack1,             *pack2,             *merge,      *settings, *back;
    QLabel              *pack1L,            *pack2L,            *title;
    QIcon               *check,             *folder,            *info;
    QLabel              *pack1ButtonLabel,  *pack2ButtonLabel;
    QFrame              *Ldiv,              *Rdiv;
    QFileDialog         *fileDialog;
    QDir                *mcDir;
    QuickMergeSettings  *qs;
    int                 mode,              description,         name,        image,     toMinecraft, exportAs;
    QString             customDescription, customName,          customImage,            customPath;
    QString             p1Path,            p2Path,              dirSlash;
    bool                settingsSet;
signals:
    void toMain();
protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // QUICKMERGE_H
