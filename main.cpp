#include "mainwindow.h"
#include "mainmenu.h"
#include "quickmerge.h"
#include "utils.h"

#include <QSurfaceFormat>
#include <QApplication>
#include <QMessageBox>
// #include <QPushButton>
// #include <QLayout>
//#include <QProgressBar>
//#include <QSlider>
// #include "conflictprompt.h"
#include <QFile>
#include <QDir>

#include <kzip.h>

int main(int argc, char *argv[])
{

    // <- MUST come *before* QApplication is constructed:
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QSurfaceFormat fmt;
    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    // Request ES2 / GL2 compatibility:
    fmt.setVersion(2, 0);
    fmt.setDepthBufferSize(24);
    fmt.setStencilBufferSize(8);
    QSurfaceFormat::setDefaultFormat(fmt);

    QApplication a(argc, argv);

    //the most important 5 lines of code in this whole project
    bool thinksHesCool = true,isActuallyCool = false, IH8Redst0ne = true;
    if(IH8Redst0ne == thinksHesCool) qDebug()<<"IH8Redst0ne thinks he's cool!";
    else qDebug()<<"IH8Redst0ne doesn't think he's cool!";
    if(IH8Redst0ne == isActuallyCool) qDebug()<<"IH8Redst0ne is actually cool!";
    else qDebug()<<"IH8Redst0ne isn't actually cool!";

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

    // // Create a zip archive
    // KZip archive(QStringLiteral("hello.zip"));
    // qDebug()<<"Created archive!";

    // // Open our archive for writing
    // if (archive.open(QIODevice::WriteOnly)) {
    //     qDebug()<<"Opened archive!";
    //     // The archive is open, we can now write data
    //     archive.writeFile(QStringLiteral("world"), // File name
    //                       QByteArray("The whole world inside a hello."), // Data
    //                       0100644, // Permissions
    //                       QStringLiteral("owner"), // Owner
    //                       QStringLiteral("users")); // Group
    //     qDebug()<<"wrote to archive!";
    //     // Don't forget to close!
    //     archive.close();
    //     qDebug()<<"closed archive!";
    // }

    // if (archive.open(QIODevice::ReadOnly)) {
    //     const KArchiveDirectory *dir = archive.directory();

    //     const KArchiveEntry *e = dir->entry("world");
    //     if (!e) {
    //         qDebug() << "File not found!";
    //         return -1;
    //     }
    //     const KArchiveFile *f = static_cast<const KArchiveFile *>(e);
    //     QByteArray arr(f->data());
    //     qDebug() << arr; // the file contents

    //     // To avoid reading everything into memory in one go, we can use createDevice() instead
    //     QIODevice *dev = f->createDevice();
    //     while (!dev->atEnd()) {
    //         qDebug() << dev->readLine();
    //     }
    //     delete dev;
    // }

    return a.exec();
}
