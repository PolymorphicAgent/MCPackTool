#include "mainwindow.h"
#include "mainmenu.h"
#include "quickmerge.h"
#include "utils.h"

#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QLayout>
//#include <QProgressBar>
//#include <QSlider>
#include "conflictprompt.h"
// #include <istream>
// #include <fstream>
#include <ZipFile.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef Q_OS_WIN
    //check for dependencies
    if(!QFile::exists("copy.bat")||!QFile::exists("copyA.bat")){
        QMessageBox::critical(NULL, " Minecraft Pack Tool v"+Utils::PROGRAM_VERSION, "Failed to find dependencies.\t\nTry reinstalling the program.", QMessageBox::Ok);
        a.quit();
        return -1;
    }
#else
#endif

    Utils::updateUser();
    // qDebug()<<(Utils::getSystemTheme() == Qt::ColorScheme::Dark);
    QuickMerge *q = new QuickMerge;
    MainMenu *menu = new MainMenu;
    MainWindow w(q,menu);
    w.setFixedSize(800,600);
    w.setCentralWidget(menu);
    w.setWindowTitle(" Minecraft Pack Tool v"+Utils::PROGRAM_VERSION);
    w.show();

    // std::ifstream f;
    // f.open(".mctmp/16x16.png", std::ios::binary);

    // qDebug()<<"should be true: "<<Utils::isImage(&f);

    // f.close();
    // f.open(".mctmp/redstone_ore.tmp", std::ios::binary);

    // qDebug()<<"should be true: "<<Utils::isImage(&f);

    // f.close();

    // Utils::upscaleImage(*new QImage(".mctmp/images/mcicon.png"), 2048, 2048).save(".mctmp/images/mciconUP.png", "PNG", 100);

    // QImage(".mctmp/images/mcicon.png").scaled(2048, 2048, Qt::KeepAspectRatio, Qt::SmoothTransformation).save(".mctmp/images/mciconUP.png", "PNG", 100);

    // ConflictPrompt *pro = new ConflictPrompt("pack1name", "p1asset", ".mctmp/16.16.png",
    //                                          "pack2name", "p2asset", ".mctmp/64.64.png");

    // pro->exec();

    // QImage t = Utils::upscaleImage(*new QImage(".mctmp/16.16.png"),32,32);

    // qDebug()<<"Output Width: "<<t.width()<<"Output Height: "<<t.height()<<"Output Format: "<<t.format();

    // qDebug()<<"16.16 format:"<<QImage(".mctmp/16.16.png").format();

    // qDebug()<<"64.64 format:"<<QImage(".mctmp/64.64.png").format();

    // QImage o = Utils::upscaleImage(*new QImage(".mctmp/64.64.png"),128,128);

    // qDebug()<<"Output Width: "<<o.width()<<"Output Height: "<<o.height()<<"Output Format: "<<o.format();

    return a.exec();
}
