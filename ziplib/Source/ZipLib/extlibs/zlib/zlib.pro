#zlib library qmake project file

TEMPLATE = lib

CONFIG += staticlib
CONFIG -= qt

CONFIG += c99
# CONFIG += c++17

DESTDIR = ../../../../../libs

SOURCES += ./adler32.c \
           ./compress.c \
           ./crc32.c \
           ./deflate.c \
           ./infback.c \
           ./inffast.c \
           ./inflate.c \
           ./inftrees.c \
           ./trees.c \
           ./uncompr.c \
           ./zutil.c

HEADERS += ./crc32.h \
           ./deflate.h \
           ./gzguts.h \
           ./inffast.h \
           ./inffixed.h \
           ./inflate.h \
           ./inftrees.h \
           ./trees.h \
           ./zconf.h \
           ./zlib.h \
           ./zutil.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

TARGET = zlib
