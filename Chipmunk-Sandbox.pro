#-------------------------------------------------
#
# Project created by QtCreator 2012-01-11T16:39:00
#
#-------------------------------------------------

QT       += core gui

TARGET = Chipmunk-Sandbox
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    canvas.cpp \
    primitive.cpp \
    scene.cpp \
    primitives/boxprimitive.cpp \
    tool.cpp \
    tools/boxtool.cpp \
    tools.cpp \
    command.cpp \
    history.cpp \
    commands/createprimitivecommand.cpp \
    tools/edittool.cpp \
    commands/moveprimitivecommand.cpp \
    primitivemarker.cpp \
    primitives/boxprimitivemarker.cpp \
    commands/movemarkercommand.cpp \
    primitives/rotateprimitivemarker.cpp \
    commands/rotateprimitivecommand.cpp

HEADERS  += mainwindow.h \
    canvas.h \
    primitive.h \
    scene.h \
    primitives/boxprimitive.h \
    tool.h \
    tools/boxtool.h \
    tools.h \
    command.h \
    history.h \
    commands/createprimitivecommand.h \
    tools/edittool.h \
    commands/moveprimitivecommand.h \
    primitivemarker.h \
    primitives/boxprimitivemarker.h \
    commands/movemarkercommand.h \
    primitives/rotateprimitivemarker.h \
    commands/rotateprimitivecommand.h

FORMS    += mainwindow.ui


RESOURCES += \
    icons.qrc
