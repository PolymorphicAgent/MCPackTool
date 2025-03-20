#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainmenu.h"
#include "quickmerge.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QuickMerge* const qOther, MainMenu* const mOther);
    ~MainWindow();
private slots:
    void quickmerge();
    void toMain();
private:
    Ui::MainWindow *ui;
    QuickMerge *q;
    MainMenu *m;
protected:
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // MAINWINDOW_H
