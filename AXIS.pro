QT += core gui widgets network sql
CONFIG += c++17

TARGET = AXIS
TEMPLATE = app

SOURCES += \
    charts.cpp \
    database.cpp \
    main.cpp \
    axis.cpp \
    connexion.cpp \
    employe.cpp \
    conge.cpp \
    materielpage.cpp \
    smtp.cpp

HEADERS += \
    axis.h \
    charts.h \
    connexion.h \
    database.h \
    employe.h \
    conge.h \
    materielpage.h \
    smtp.h \
    style.h

FORMS += \
    axis.ui \
    materielpage.ui

RESOURCES += \
    ressources.qrc

DISTFILES += \
    ../../AXIS_Materiels (3).zip \
    style.qss