#ZipLib library qmake project file

TEMPLATE = lib

CONFIG += staticlib
CONFIG -= qt

# CONFIG += c99
CONFIG += c++17

DESTDIR = ../libs

SOURCES += Source/ZipLib/ZipArchive.cpp \
           Source/ZipLib/ZipArchiveEntry.cpp \
           Source/ZipLib/ZipFile.cpp \
           Source/ZipLib/detail/EndOfCentralDirectoryBlock.cpp \
           Source/ZipLib/detail/ZipCentralDirectoryFileHeader.cpp \
           Source/ZipLib/detail/ZipGenericExtraField.cpp \
           Source/ZipLib/detail/ZipLocalFileHeader.cpp

HEADERS += Source/ZipLib/compression/bzip2/bzip2_decoder.h \
           Source/ZipLib/compression/bzip2/bzip2_decoder_properties.h \
           Source/ZipLib/compression/bzip2/bzip2_encoder.h \
           Source/ZipLib/compression/bzip2/bzip2_encoder_properties.h \
           Source/ZipLib/compression/compression_interface.h \
           Source/ZipLib/compression/deflate/deflate_decoder.h \
           Source/ZipLib/compression/deflate/deflate_decoder_properties.h \
           Source/ZipLib/compression/deflate/deflate_encoder.h \
           Source/ZipLib/compression/deflate/deflate_encoder_properties.h \
           Source/ZipLib/compression/lzma/detail/lzma_alloc.h \
           Source/ZipLib/compression/lzma/detail/lzma_handle.h \
           Source/ZipLib/compression/lzma/detail/lzma_header.h \
           Source/ZipLib/compression/lzma/detail/lzma_in_stream.h \
           Source/ZipLib/compression/lzma/detail/lzma_out_stream.h \
           Source/ZipLib/compression/lzma/lzma_decoder.h \
           Source/ZipLib/compression/lzma/lzma_decoder_properties.h \
           Source/ZipLib/compression/lzma/lzma_encoder.h \
           Source/ZipLib/compression/lzma/lzma_encoder_properties.h \
           Source/ZipLib/compression/store/store_decoder.h \
           Source/ZipLib/compression/store/store_decoder_properties.h \
           Source/ZipLib/compression/store/store_encoder.h \
           Source/ZipLib/compression/store/store_encoder_properties.h \
           Source/ZipLib/detail/EndOfCentralDirectoryBlock.h \
           Source/ZipLib/detail/ZipCentralDirectoryFileHeader.h \
           Source/ZipLib/detail/ZipGenericExtraField.h \
           Source/ZipLib/detail/ZipLocalFileHeader.h \
           Source/ZipLib/methods/Bzip2Method.h \
           Source/ZipLib/methods/DeflateMethod.h \
           Source/ZipLib/methods/ICompressionMethod.h \
           Source/ZipLib/methods/LzmaMethod.h \
           Source/ZipLib/methods/StoreMethod.h \
           Source/ZipLib/methods/ZipMethodResolver.h \
           Source/ZipLib/streams/compression_decoder_stream.h \
           Source/ZipLib/streams/compression_encoder_stream.h \
           Source/ZipLib/streams/crc32stream.h \
           Source/ZipLib/streams/memstream.h \
           Source/ZipLib/streams/nullstream.h \
           Source/ZipLib/streams/serialization.h \
           Source/ZipLib/streams/streambuffs/compression_decoder_streambuf.h \
           Source/ZipLib/streams/streambuffs/compression_encoder_streambuf.h \
           Source/ZipLib/streams/streambuffs/crc32_streambuf.h \
           Source/ZipLib/streams/streambuffs/mem_streambuf.h \
           Source/ZipLib/streams/streambuffs/null_streambuf.h \
           Source/ZipLib/streams/streambuffs/sub_streambuf.h \
           Source/ZipLib/streams/streambuffs/tee_streambuff.h \
           Source/ZipLib/streams/streambuffs/zip_crypto_streambuf.h \
           Source/ZipLib/streams/substream.h \
           Source/ZipLib/streams/teestream.h \
           Source/ZipLib/streams/zip_cryptostream.h \
           Source/ZipLib/utils/enum_utils.h \
           Source/ZipLib/utils/stream_utils.h \
           Source/ZipLib/utils/time_utils.h \
           Source/ZipLib/ZipArchive.h \
           Source/ZipLib/ZipArchiveEntry.h \
           Source/ZipLib/ZipFile.h

TARGET = ZipLib

#bzip2
win32: LIBS += -L$$PWD/../libs/ -lbzip2

INCLUDEPATH += $$PWD/Source/ZipLib/extlibs/bzip2
DEPENDPATH += $$PWD/Source/ZipLib/extlibs/bzip2

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../libs/bzip2.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../libs/libbzip2.a

#lzma
win32: LIBS += -L$$PWD/../libs/ -llzma

INCLUDEPATH += $$PWD/Source/ZipLib/extlibs/lzma
DEPENDPATH += $$PWD/Source/ZipLib/extlibs/lzma

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../libs/lzma.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../libs/liblzma.a

#zlib
win32: LIBS += -L$$PWD/../libs/ -lzlib

INCLUDEPATH += $$PWD/../libs
DEPENDPATH += $$PWD/../libs

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../libs/zlib.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../libs/libzlib.a
