QT       += core gui quick qml quickwidgets svg opengl widgets openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#include project sources
SOURCES += \
    combomessagebox.cpp \
    conflictprompt.cpp \
    guithreadslotter.cpp \
    importmenu.cpp \
    main.cpp \
    mainmenu.cpp \
    mainwindow.cpp \
    mcresourcepack.cpp \
    # mergedefaultworkerthread.cpp \
    mergeworkerthread.cpp \
    modelpreviewwidget.cpp \
    newworkspace.cpp \
    progressmessagebox.cpp \
    quickmerge.cpp \
    quickmergesettings.cpp \
    utils.cpp

#include project headers
HEADERS += \
    combomessagebox.h \
    conflictprompt.h \
    guithreadslotter.h \
    importmenu.h \
    mainmenu.h \
    mainwindow.h \
    mcresourcepack.h \
    # mergedefaultworkerthread.h \
    mergeworkerthread.h \
    modelpreviewwidget.h \
    newworkspace.h \
    progressmessagebox.h \
    quickmerge.h \
    quickmergesettings.h \
    utils.h

#for windows GetUsernameW
LIBS += -lAdvapi32

# LIBS += -lZipLib
# LIBS += -ljsoncpp

FORMS += \
   mainwindow.ui

RC_ICONS = icon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    mergenondefault.txt \
    quickmergesettings.txt \
    scratchpad \
    todo.txt \
    README.md

RESOURCES += \
    icons.qrc

#ziplib
# win32: LIBS += -L$$PWD/libs/ -lZipLib

# INCLUDEPATH += $$PWD/ziplib/Source/ZipLib
# DEPENDPATH += $$PWD/ziplib/Source/ZipLib


# win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/ZipLib.lib
# else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/libZipLib.a

#jsoncpp
# win32: LIBS += -L$$PWD/libs/ -ljsoncpp

# INCLUDEPATH += $$PWD/jsoncpp/include/json
# DEPENDPATH += $$PWD/jsoncpp/include/json

# win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/jsoncpp.lib
# else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/libjsoncpp.a



# #karchive - Release
# win32: LIBS += -L$$PWD/libs/msvc2022_64/Release/karchive-static/lib/ -lKF6Archive

# INCLUDEPATH += $$PWD/libs/msvc2022_64/Release/karchive-static/include/KF6/KArchive
# DEPENDPATH += $$PWD/libs/msvc2022_64/Release/karchive-static/include/KF6/KArchive

# win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/msvc2022_64/Release/karchive-static/lib/KF6Archive.lib

# #zlib - Release
# win32: LIBS += -L$$PWD/libs/msvc2022_64/Release/zlib/lib/ -lzlibstatic

# INCLUDEPATH += $$PWD/libs/msvc2022_64/Release/zlib/include
# DEPENDPATH += $$PWD/libs/msvc2022_64/Release/zlib/include

# win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/msvc2022_64/Release/zlib/lib/zlibstatic.lib


#karchive - Debug
win32: LIBS += -L$$PWD/libs/msvc2022_64/Debug/karchive-static/lib/ -lKF6Archive

INCLUDEPATH += $$PWD/libs/msvc2022_64/Debug/karchive-static/include/KF6/KArchive
DEPENDPATH += $$PWD/libs/msvc2022_64/Debug/karchive-static/include/KF6/KArchive

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/msvc2022_64/Debug/karchive-static/lib/KF6Archive.lib

#zlib - Debug
win32: LIBS += -L$$PWD/libs/msvc2022_64/Debug/zlib/lib/ -lzlibstatic

INCLUDEPATH += $$PWD/libs/msvc2022_64/Debug/zlib/include
DEPENDPATH += $$PWD/libs/msvc2022_64/Debug/zlib/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/msvc2022_64/Debug/zlib/lib/zlibstatic.lib



#bzip2
# win32: LIBS += -L$$PWD/libs/ -lbzip2

# INCLUDEPATH += $$PWD/ziplib/Source/ZipLib/extlibs/bzip2
# DEPENDPATH += $$PWD/ziplib/Source/ZipLib/extlibs/bzip2

# win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/bzip2.lib
# else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/libbzip2.a

#lzma
# win32: LIBS += -L$$PWD/libs/ -llzma

# INCLUDEPATH += $$PWD/libs
# DEPENDPATH += $$PWD/libs

# win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/lzma.lib
# else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/liblzma.a

#zlib

# win32: LIBS += -L$$PWD/libs/ -lzlib

# INCLUDEPATH += $$PWD/libs
# DEPENDPATH += $$PWD/libs

# win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/zlib.lib
# else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/libzlib.a

#eof
