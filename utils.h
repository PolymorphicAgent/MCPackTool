#ifndef UTILS_H
#define UTILS_H

#include <vector>

#include <KZip>

#include <QGuiApplication>
#include <QStyleHints>
#include <QStringList>
#include <QEventLoop>
#include <QProcess>
#include <QString>
#include <QDebug>
#include <QIcon>
#include <QSet>
#include <QDir>

#ifdef Q_OS_WIN
// #define NOMINMAX //for warnings' sake
#include <windows.h>
#include <Lmcons.h>
#endif

#include "MCResourcePack.h"

class Utils : public QObject
{
    Q_OBJECT
public:

    static QString getUser();
    static void updateUser();

    //Qt::ColorScheme::Dark or Qt::ColorScheme::Light
    static Qt::ColorScheme getSystemTheme();

    static bool isSquare(QString path);

    static const QString inline PROGRAM_VERSION = "1.0 ALPHA";

    static inline QString MC_PATH;

    static bool compareQIcon(const QIcon&, const QIcon&);

    static QStringList inline formatToVersion(int format){
        QStringList version = {};
        switch(format){
        case 1:
            version.push_back("1.6.1");
            version.push_back("1.6.2");
            version.push_back("1.6.4");
            version.push_back("1.7.2");
            version.push_back("1.7.4");
            version.push_back("1.7.5");
            version.push_back("1.7.6");
            version.push_back("1.7.7");
            version.push_back("1.7.8");
            version.push_back("1.7.9");
            version.push_back("1.7.10");
            version.push_back("1.8");
            version.push_back("1.8.1");
            version.push_back("1.8.2");
            version.push_back("1.8.3");
            version.push_back("1.8.4");
            version.push_back("1.8.5");
            version.push_back("1.8.6");
            version.push_back("1.8.7");
            version.push_back("1.8.8");
            version.push_back("1.8.9");
            return version;
        case 2:
            version.push_back("1.9");
            version.push_back("1.9.1");
            version.push_back("1.9.2");
            version.push_back("1.9.3");
            version.push_back("1.9.4");
            version.push_back("1.10");
            version.push_back("1.10.1");
            version.push_back("1.10.2");
            return version;
        case 3:
            version.push_back("1.11");
            version.push_back("1.11.1");
            version.push_back("1.11.2");
            version.push_back("1.12");
            version.push_back("1.12.1");
            version.push_back("1.12.2");
            return version;
        case 4:
            version.push_back("1.13");
            version.push_back("1.13.1");
            version.push_back("1.13.2");
            version.push_back("1.14");
            version.push_back("1.14.1");
            version.push_back("1.14.2");
            version.push_back("1.14.3");
            version.push_back("1.14.4");
            return version;
        case 5:
            version.push_back("1.15");
            version.push_back("1.15.1");
            version.push_back("1.15.2");
            version.push_back("1.16");
            version.push_back("1.16.1");
            return version;
        case 6:
            version.push_back("1.16.2");
            version.push_back("1.16.3");
            version.push_back("1.16.4");
            version.push_back("1.16.5");
            return version;
        case 7:
            version.push_back("1.17");
            version.push_back("1.17.1");
            return version;
        case 8:
            version.push_back("1.18");
            version.push_back("1.18.1");
            version.push_back("1.18.2");
            return version;
        case 9:
            version.push_back("1.19");
            version.push_back("1.19.1");
            version.push_back("1.19.2");
            return version;
        case 10:
            version.push_back("INVALID PACK FORMAT");
            return version;
        case 11:
            version.push_back("22w42a");
            version.push_back("22w43a");
            version.push_back("22w44a");
            return version;
        case 12:
            version.push_back("1.19.3");
            return version;
        case 13:
            version.push_back("1.19.4");
            return version;
        case 14:
            version.push_back("23w14a");
            version.push_back("23w15a");
            version.push_back("23w16a");
            return version;
        case 15:
            version.push_back("1.20");
            version.push_back("1.20.1");
            return version;
        case 16:
            version.push_back("23w31a");
            return version;
        case 17:
            version.push_back("23w32a");
            return version;
        case 18:
            version.push_back("1.20.2");
            return version;
        default:
            version.push_back("INVALID PACK FORMAT");
            return version;
        }
    }

    static int inline versionToFormat(const QString& version){
        if(version == "1.6.1"){
            return 1;
        }
        else if(version == "1.6.2"){
            return 1;
        }
        else if(version == "1.6.4"){
            return 1;
        }
        else if(version == "1.7.2"){
            return 1;
        }
        else if(version == "1.7.4"){
            return 1;
        }
        else if(version == "1.7.5"){
            return 1;
        }
        else if(version == "1.7.6"){
            return 1;
        }
        else if(version == "1.7.7"){
            return 1;
        }
        else if(version == "1.7.8"){
            return 1;
        }
        else if(version == "1.7.9"){
            return 1;
        }
        else if(version == "1.7.10"){
            return 1;
        }
        else if(version == "1.8"){
            return 1;
        }
        else if(version == "1.8.1"){
            return 1;
        }
        else if(version == "1.8.2"){
            return 1;
        }
        else if(version == "1.8.3"){
            return 1;
        }
        else if(version == "1.8.4"){
            return 1;
        }
        else if(version == "1.8.5"){
            return 1;
        }
        else if(version == "1.8.6"){
            return 1;
        }
        else if(version == "1.8.7"){
            return 1;
        }
        else if(version == "1.8.8"){
            return 1;
        }
        else if(version == "1.8.9"){
            return 1;
        }
        else if(version == "1.9"){
            return 2;
        }
        else if(version == "1.9.1"){
            return 2;
        }
        else if(version == "1.9.2"){
            return 2;
        }
        else if(version == "1.9.3"){
            return 2;
        }
        else if(version == "1.9.4"){
            return 2;
        }
        else if(version == "1.10"){
            return 2;
        }
        else if(version == "1.10.1"){
            return 2;
        }
        else if(version == "1.10.2"){
            return 2;
        }
        else if(version == "1.11"){
            return 3;
        }
        else if(version == "1.11.1"){
            return 3;
        }
        else if(version == "1.11.2"){
            return 3;
        }
        else if(version == "1.12"){
            return 3;
        }
        else if(version == "1.12.1"){
            return 3;
        }
        else if(version == "1.12.2"){
            return 3;
        }
        else if(version == "1.13"){
            return 4;
        }
        else if(version == "1.13.1"){
            return 4;
        }
        else if(version == "1.13.2"){
            return 4;
        }
        else if(version == "1.14"){
            return 4;
        }
        else if(version == "1.14.1"){
            return 4;
        }
        else if(version == "1.14.2"){
            return 4;
        }
        else if(version == "1.14.3"){
            return 4;
        }
        else if(version == "1.14.4"){
            return 4;
        }
        else if(version == "1.15"){
            return 5;
        }
        else if(version == "1.15.1"){
            return 5;
        }
        else if(version == "1.15.2"){
            return 5;
        }
        else if(version == "1.16"){
            return 5;
        }
        else if(version == "1.16.1"){
            return 5;
        }
        else if(version == "1.16.2"){
            return 6;
        }
        else if(version == "1.16.3"){
            return 6;
        }
        else if(version == "1.16.4"){
            return 6;
        }
        else if(version == "1.16.5"){
            return 6;
        }
        else if(version == "1.17"){
            return 7;
        }
        else if(version == "1.17.1"){
            return 7;
        }
        else if(version == "1.18"){
            return 8;
        }
        else if(version == "1.18.1"){
            return 8;
        }
        else if(version == "1.18.2"){
            return 8;
        }
        else if(version == "1.19"){
            return 9;
        }
        else if(version == "1.19.1"){
            return 9;
        }
        else if(version == "1.19.2"){
            return 9;
        }
        else if(version == "22w42a"){
            return 11;
        }
        else if(version == "22w43a"){
            return 11;
        }
        else if(version == "22w44a"){
            return 11;
        }
        else if(version == "1.19.3"){
            return 12;
        }
        else if(version == "1.19.4"){
            return 13;
        }
        else if(version == "23w14a"){
            return 14;
        }
        else if(version == "23w15a"){
            return 14;
        }
        else if(version == "23w16a"){
            return 14;
        }
        else if(version == "1.20"){
            return 15;
        }
        else if(version == "1.20.1"){
            return 15;
        }
        else if(version == "23w31a"){
            return 16;
        }
        else if(version == "23w32a"){
            return 17;
        }
        else if(version == "1.20.2"){
            return 18;
        }
        else return -1;
    }

    static QString inline stringify(const QStringList& v){
        QString result = "";
        for(const QString &i:v)
            result+=(", "+i);
        return result.removeFirst();
    }

    static QString inline stringify(const std::vector<KArchiveEntry*>& v) {
        QString result = "[";
        for (KArchiveEntry *i : v) {
            result += QString::fromStdString(i->name().toStdString()) + ", ";
        }
        if (!v.empty())
            result.chop(2); // remove trailing comma+space
        result += "]";
        return result;
    }

    static QStringList inline mergeLists(const QStringList& a, const QStringList& b){
        QStringList result;
        for(const QString &i:a)
            result.push_back(i);
        for(const QString &i:b)
            result.push_back(i);
        return result;
    }

    static bool isResourcepack(const QString&);

    //static bool unzip(QString, QString);

    // static bool mergeDefault(const QString&, const QString&);

    static bool merge(const QString&, const QString&, int, int, QString, int, QString, int, QString, int, QString, int);

    static int find(const QString&,const QStringList&);

    static bool find(const QString&, std::vector<KArchiveEntry*>&);

    static int iFind(const QString&, std::vector<KArchiveEntry*>&);

    static bool cp(const QString&, const QString&);

    static QString extractName(const QString&);

    static void mkdir(const QString&);

    static void rm(const QString&);

    static std::vector<std::string> split(const std::string&, char);

    static int easedValue(int, int, qreal);

    static QString convertToWindowsPath(const QString&);

    static QString subSpaces(QString& st);

    static Utils& instance() {
        static Utils instance;
        return instance;
    }

    static QImage upscaleImage(const QImage&, int, int);

    static QImage downscaleImage(const QImage&, int, int);

    static int maxValue(int, int);

    static int minValue(int, int);

    static bool isJson(QByteArray*);

    static bool isImage(std::istream*);

    static bool isImage(const QImage&);

    static void debaggerlogger(const QString&);

    static QString limitedLength(const QString&, int);

    // Checks if an element with baseName exists in list
    static bool elementExists(const QString &baseName,
                              const QList<MCResourcePackElement*> &list);

    // Removes and deletes the element with baseName from the list
    static void removeElement(const QString &baseName,
                              QList<MCResourcePackElement*> &list);

public slots:
    static void postMergeNonDefault(QString);

private:
    static QString inline USER;

    static QString inline NAME;
};

#endif // UTILS_H
