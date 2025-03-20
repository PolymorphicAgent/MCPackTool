#bzip2 library qmake project file

TEMPLATE = lib

CONFIG += staticlib
CONFIG -= qt

CONFIG += c99
# CONFIG += c++17

DESTDIR = ../../../../../libs

SOURCES += blocksort.c \
           bzerror.c \
           bzlib.c \
           compress.c \
           crctable.c \
           decompress.c \
           huffman.c \
           randtable.c

HEADERS += bzlib.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

TARGET = bzip2
