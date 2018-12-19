#-------------------------------------------------
#
# Project created by QtCreator 2018-08-10T19:17:57
#
#-------------------------------------------------

QT       += serialbus serialport core gui axcontainer sql xml printsupport charts
QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_ICONS = SunteckICON.ico

TARGET = TCR
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    baseplate.cpp \
    displayop.cpp \
    dmm.cpp \
    main.cpp \
    mainwindow.cpp \
    rlamp.cpp \
    switch.cpp \
    tcrsettingdialog.cpp \
    tcrbase.cpp \
    tcrbase.cpp \
    baseplate.cpp \
    displayop.cpp \
    dmm.cpp \
    tcrseldialog.cpp \
    dataop.cpp \
    tcrdataop.cpp \
    tcrmeasuredialog.cpp \
    probetestdialog.cpp \
    progressview.cpp \
    tcrdatadialog.cpp \
    calanderdialog.cpp \
    probedata.cpp \
    calibratedialog.cpp \
    mymodbus.cpp \
    mymodbus.cpp


HEADERS += \
        mainwindow.h \
    NIinclude/ivi.h \
    NIinclude/IviACPwr_ni.h \
    NIinclude/IviCounter_ni.h \
    NIinclude/IviDCPwr.h \
    NIinclude/IviDCPwrObsolete.h \
    NIinclude/IviDigitizer_ni.h \
    NIinclude/IviDmm.h \
    NIinclude/IviDmmObsolete.h \
    NIinclude/IviDownconverter_ni.h \
    NIinclude/IviFgen.h \
    NIinclude/IviFgenObsolete.h \
    NIinclude/IviPwrMeter.h \
    NIinclude/IviRFSigGen.h \
    NIinclude/IviScope.h \
    NIinclude/IviScopeObsolete.h \
    NIinclude/IviSpecAn.h \
    NIinclude/IviSwtch.h \
    NIinclude/IviSwtchObsolete.h \
    NIinclude/IviUpconverter_ni.h \
    NIinclude/IviVisaType.h \
    NIinclude/nidmm.h \
    NIinclude/nidmmObsolete.h \
    NIinclude/niModInst.h \
    NIinclude/niswitch.h \
    NIinclude/niswitchTopologies.h \
    NIinclude/visa.h \
    NIinclude/visatype.h \
    NIinclude/vpptype.h \
    baseplate.h \
    displayop.h \
    dmm.h \
    gloabaldefine.h \
    mainwindow.h \
    rlamp.h \
    switch.h \
    tcrsettingdialog.h \
    tcrbase.h \
    tcrbase.h \
    tcrseldialog.h \
    dataop.h \
    tcrdataop.h \
    tcrmeasuredialog.h \
    probetestdialog.h \
    progressview.h \
    tcrdatadialog.h \
    calanderdialog.h \
    probedata.h \
    calibratedialog.h \
    mymodbus.h \
    mymodbus.h \
    mymodbus.h


FORMS += \
        mainwindow.ui \
    tcrsettingdialog.ui \
    mainwindow.ui \
    tcrsettingdialog.ui \
    tcrseldialog.ui \
    tcrmeasuredialog.ui \
    probetestdialog.ui \
    tcrseldialog.ui \
    tcrmeasuredialog.ui \
    probetestdialog.ui \
    tcrdatadialog.ui \
    calanderdialog.ui \
    calibratedialog.ui



RESOURCES += \
    m_resource.qrc




unix|win32: LIBS += -L$$PWD/Limereportlib/ -llimereport

INCLUDEPATH += $$PWD/Limereportlib/include
DEPENDPATH += $$PWD/Limereportlib/include



unix|win32: LIBS += -L$$PWD/Limereportlib/ -lQtZint

INCLUDEPATH += $$PWD/Limereportlib/include
DEPENDPATH += $$PWD/Limereportlib/include

unix|win32: LIBS += -L$$PWD/NIlib/ -lnidmm_32

INCLUDEPATH += $$PWD/NIlib
DEPENDPATH += $$PWD/NIlib

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/NIlib/nidmm_32.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/NIlib/libnidmm_32.a

unix|win32: LIBS += -L$$PWD/NIlib/ -lniswitch

INCLUDEPATH += $$PWD/NIlib
DEPENDPATH += $$PWD/NIlib

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/NIlib/niswitch.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/NIlib/libniswitch.a
