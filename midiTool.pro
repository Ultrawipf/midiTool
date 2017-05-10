#-------------------------------------------------
#
# Project created by QtCreator 2017-01-23T14:15:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = midiTool
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    midiParser/Binasc.cpp \
    midiParser/MidiEvent.cpp \
    midiParser/MidiEventList.cpp \
    midiParser/MidiFile.cpp \
    midiParser/MidiMessage.cpp \
    midiParser/Options.cpp \
    factoriogenerator.cpp \
    Blueprinter/base64.cpp \
    Blueprinter/blueprint.cpp \
    Blueprinter/deflate_wrapper.cpp \
    Blueprinter/zlib/adler32.c \
    Blueprinter/zlib/compress.c \
    Blueprinter/zlib/crc32.c \
    Blueprinter/zlib/deflate.c \
    Blueprinter/zlib/gzclose.c \
    Blueprinter/zlib/gzlib.c \
    Blueprinter/zlib/gzread.c \
    Blueprinter/zlib/gzwrite.c \
    Blueprinter/zlib/infback.c \
    Blueprinter/zlib/inffast.c \
    Blueprinter/zlib/inflate.c \
    Blueprinter/zlib/inftrees.c \
    Blueprinter/zlib/trees.c \
    Blueprinter/zlib/uncompr.c \
    Blueprinter/zlib/zutil.c

HEADERS  += mainwindow.h \
    midiParser/Binasc.h \
    midiParser/MidiEvent.h \
    midiParser/MidiEventList.h \
    midiParser/MidiFile.h \
    midiParser/MidiMessage.h \
    midiParser/Options.h \
    factoriogenerator.h \
    Blueprinter/zlib/crc32.h \
    Blueprinter/zlib/deflate.h \
    Blueprinter/zlib/gzguts.h \
    Blueprinter/zlib/inffast.h \
    Blueprinter/zlib/inffixed.h \
    Blueprinter/zlib/inflate.h \
    Blueprinter/zlib/inftrees.h \
    Blueprinter/zlib/trees.h \
    Blueprinter/zlib/zconf.h \
    Blueprinter/zlib/zlib.h \
    Blueprinter/zlib/zutil.h \
    Blueprinter/base64.h \
    Blueprinter/blueprint.h \
    Blueprinter/deflate_wrapper.h \
    Blueprinter/util.h

FORMS    += mainwindow.ui
