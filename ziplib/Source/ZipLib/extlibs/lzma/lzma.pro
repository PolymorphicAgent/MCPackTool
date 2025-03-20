#lzma library qmake project file

TEMPLATE = lib

CONFIG += staticlib
CONFIG -= qt

CONFIG += c99
# CONFIG += c++17

DESTDIR = ../../../../../libs

SOURCES += ./7zAlloc.c \
           ./7zBuf.c \
           ./7zBuf2.c \
           ./7zCrc.c \
           ./7zCrcOpt.c \
           ./7zDec.c \
           ./7zFile.c \
           ./7zin.c \
           ./7zStream.c \
           ./Alloc.c \
           ./Bcj2.c \
           ./Bra.c \
           ./Bra86.c \
           ./BraIA64.c \
           ./CpuArch.c \
           ./Delta.c \
           ./LzFind.c \
           ./LzFindMt.c \
           ./Lzma2Dec.c \
           ./Lzma2Enc.c \
           ./LzmaDec.c \
           ./LzmaEnc.c \
           ./LzmaLib.c \
           ./MtCoder.c \
           ./Ppmd7.c \
           ./Ppmd7Dec.c \
           ./Ppmd7Enc.c \
           ./Sha256.c \
           ./Threads.c \
           ./Xz.c \
           ./XzCrc64.c \
           ./XzDec.c \
           ./XzEnc.c \
           ./Xzin.c
           # ./unix/7zBuf2.c \
           # ./unix/7zCrc.c \
           # ./unix/7zCrcOpt.c \
           # ./unix/7zStream.c \
           # ./unix/Aes.c \
           # ./unix/Alloc.c \
           # ./unix/Bra.c \
           # ./unix/Bra86.c \
           # ./unix/BraIA64.c \
           # ./unix/BwtSort.c \
           # ./unix/CpuArch.c \
           # ./unix/Delta.c \
           # ./unix/HuffEnc.c \
           # ./unix/LzFind.c \
           # ./unix/LzFindMt.c \
           # ./unix/Lzma2Dec.c \
           # ./unix/Lzma2Enc.c \
           # ./unix/LzmaDec.c \
           # ./unix/LzmaEnc.c \
           # ./unix/LzmaUtil/Lzma86Dec.c \
           # ./unix/LzmaUtil/Lzma86Enc.c \
           # ./unix/MtCoder.c \
           # ./unix/Ppmd7.c \
           # ./unix/Ppmd7Dec.c \
           # ./unix/Ppmd7Enc.c \
           # ./unix/Ppmd8.c \
           # ./unix/Ppmd8Dec.c \
           # ./unix/Ppmd8Enc.c \
           # ./unix/Sha256.c \
           # ./unix/Sort.c \
           # ./unix/Threads.c \
           # ./unix/Xz.c \
           # ./unix/XzCrc64.c \
           # ./unix/XzDec.c \
           # ./unix/XzEnc.c \
           # ./unix/XzIn.c

HEADERS += ./7z.h \
           ./7zAlloc.h \
           ./7zBuf.h \
           ./7zCrc.h \
           ./7zFile.h \
           ./7zVersion.h \
           ./Alloc.h \
           ./Bcj2.h \
           ./Bra.h \
           ./CpuArch.h \
           ./Delta.h \
           ./LzFind.h \
           ./LzFindMt.h \
           ./LzHash.h \
           ./Lzma2Dec.h \
           ./Lzma2Enc.h \
           ./Lzma86.h \
           ./LzmaDec.h \
           ./LzmaEnc.h \
           ./LzmaLib.h \
           ./MtCoder.h \
           ./Ppmd.h \
           ./Ppmd7.h \
           ./RotateDefs.h \
           ./Sha256.h \
           ./Threads.h \
           ./Types.h \
           # ./unix/7zBuf.h \
           # ./unix/7zCrc.h \
           # ./unix/7zVersion.h \
           # ./unix/Aes.h \
           # ./unix/Alloc.h \
           # ./unix/Bra.h \
           # ./unix/BwtSort.h \
           # ./unix/CpuArch.h \
           # ./unix/Delta.h \
           # ./unix/HuffEnc.h \
           # ./unix/LzFind.h \
           # ./unix/LzFindMt.h \
           # ./unix/LzHash.h \
           # ./unix/Lzma2Dec.h \
           # ./unix/Lzma2Enc.h \
           # ./unix/LzmaDec.h \
           # ./unix/LzmaEnc.h \
           # ./unix/LzmaUtil/Lzma86Dec.h \
           # ./unix/LzmaUtil/Lzma86Enc.h \
           # ./unix/MtCoder.h \
           # ./unix/Ppmd.h \
           # ./unix/Ppmd7.h \
           # ./unix/Ppmd8.h \
           # ./unix/RotateDefs.h \
           # ./unix/Sha256.h \
           # ./unix/Sort.h \
           # ./unix/Threads.h \
           # ./unix/Types.h \
           # ./unix/Xz.h \
           # ./unix/XzCrc64.h \
           # ./unix/XzEnc.h \
           ./Xz.h \
           ./XzCrc64.h \
           ./XzEnc.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

TARGET = lzma
