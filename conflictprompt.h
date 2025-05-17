#ifndef CONFLICTPROMPT_H
#define CONFLICTPROMPT_H

#include "MCResourcePack.h"

#include <QOpenGLWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QDialog>

class ModelPreviewWidget;

class ConflictPrompt : public QDialog
{
    //Q_OBJECT
public:
    ConflictPrompt(const QString&, const QString&, MCResourcePackElement*, MCResourcePackElement*);
    ~ConflictPrompt();
    int result();
public slots:
    void updateR(bool);
    void updateL(bool);
    void accept() override;
    void flush();
private:
    int choice = 0;
    QString p1name, p2name;
    QCheckBox *selL, *selR;
    QPushButton *okButton, *cancelButton;
    ModelPreviewWidget *leftPreview, *rightPreview;
};

#endif // CONFLICTPROMPT_H
