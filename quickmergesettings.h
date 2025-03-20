#ifndef QUICKMERGESETTINGS_H
#define QUICKMERGESETTINGS_H

#include <QDir>
#include <QFrame>
#include <QLabel>
#include <QToolTip>
#include <QWidget>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QCloseEvent>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

class QuickMergeSettings : public QWidget
{
    Q_OBJECT
public:
    explicit QuickMergeSettings(QWidget *parent = nullptr);
    void reset();
private slots:
    void modeIclicked();
    void infoIclicked();
    void infoPIIclicked();
    void infoPISclicked();
    void infoCDindexChanged(int);
    void infoCNindexChanged(int);
//    void infoCPIindexChanged(int);
    void exportIclicked();
    void exportIMclicked();
    void exportITclicked();
    void exportMclicked();
    void exportCindexChanged(int);
    void applyClicked();
    void backClicked();
    void closeEvent(QCloseEvent*) override;
private:
    QFrame      *modeF  , *infoF  , *exportF ;
    QLabel      *modeL  , *infoL  , *exportL ;
    QLabel      *modeD  , *infoD  , *exportD ;
    QLabel      *infoN  , *infoPI , *exportT ;
    QPushButton *modeI  , *infoI  , *exportI , *exportIT, *apply;
    QPushButton *infoPII, *infoPIS, *exportIM, *exportM , *back ;
    QComboBox   *modeC  , *infoCD , *exportC ;
    QComboBox   *infoCN , *infoCPI, *exportCT;
    QIcon       *info   , *folder , *check   ;
    QString customDescription, customName, customImage, customPath, dirSlash;
    QRegularExpression *noSpace;
    QFileDialog *fileDialog;
    QDir *mcDir;
    int mode, description, name, image, toMinecraft, exportAs;
    bool piis, exs, applyEvent, backEvent;
signals:
    void quickMergeSettngsChanged(int, int, QString, int, QString, int, QString, int, QString, int);
protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // QUICKMERGESETTINGS_H
