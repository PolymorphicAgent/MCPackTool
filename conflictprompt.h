#ifndef CONFLICTPROMPT_H
#define CONFLICTPROMPT_H


#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QFrame>
#include <QImage>
#include <QIcon>

class ConflictPrompt : public QDialog
{
    //Q_OBJECT
public:
    ConflictPrompt(const QString&, const QString&, const QString&, const QString&, const QString&, const QString&);
    int result();
public slots:
    void updateR(bool);
    void updateL(bool);
    void accept() override;
private:
    int choice = 0;
    QCheckBox *selL, *selR;
    QString p1name, p1asset, p2name, p2asset;
    QImage *p1image, *p2image;
};

#endif // CONFLICTPROMPT_H
