# ─────────────────────────────────────────────────────────────────────────────
# euro2012.pro – QMake project (optional; prefer CMake for portfolio builds)
#
# Layered layout:
#   src/logic/  – pure C++17 domain (no Qt)
#   src/gui/    – Qt Widgets front-end
# ─────────────────────────────────────────────────────────────────────────────

QT       += core gui widgets
CONFIG   += c++17 warn_on
TARGET    = Euro2012Simulator
TEMPLATE  = app

INCLUDEPATH += $$PWD/src/logic $$PWD/src/gui

SOURCES += \
    src/logic/Zawodnik.cpp \
    src/logic/ZdarzenieMeczowe.cpp \
    src/logic/CzlonekSztabu.cpp \
    src/logic/Druzyna.cpp \
    src/logic/Mecz.cpp \
    src/logic/Turniej.cpp \
    src/main.cpp \
    src/gui/MainWindow.cpp

HEADERS += \
    src/logic/Exceptions.h \
    src/logic/Zawodnik.h \
    src/logic/ZdarzenieMeczowe.h \
    src/logic/CzlonekSztabu.h \
    src/logic/Druzyna.h \
    src/logic/Mecz.h \
    src/logic/Turniej.h \
    src/gui/MainWindow.h

FORMS += \
    src/gui/MainWindow.ui
