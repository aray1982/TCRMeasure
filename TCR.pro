#-------------------------------------------------
#
# Project created by QtCreator 2018-08-10T19:17:57
#
#-------------------------------------------------

QT       += serialbus serialport core gui axcontainer sql xml printsupport charts
QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include  (./Dataop/dataop.pri)
include  (./NIop/NIop.pri)
include  (./Probe/Probe.pri)
include  (./TCR/TCR.pri)
include  (./Dataquery/Dataquery.pri)
include  (./Calibrate/Calibrate.pri)
include  (./Mainwindow/Mainwindow.pri)
include  (./Other/Other.pri)



VERSION = 1.2.0.3
RC_ICONS = SunteckICON.ico
QMAKE_TARGET_COMPANY = "Sunteck"
QMAKE_TARGET_PRODUCT = "TCR Measure System"
QMAKE_TARGET_COPYRIGHT = "Copyright 2001-2018 The Sunteck Company Ltd. All rights reserved."
RC_LANG = 0x0004
DEFINES += APP_VERSION=\\\"$$VERSION\\\"



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






HEADERS += \
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
    gloabaldefine.h \



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

# V1.0.0 修正：
 # 追加R1~10全选选项
 # 外部测量追加低阻小于10Ω优化选项
 # 调整所有报表格式。
 # 调整查询显示结果界面，使查询结果横向不使用图框
 # 调整外部测量界面，使显示结果全部显示
 # 追加UAC选项
#/
/#V1.0.1 修正:
 # 追加数据查询界面显示最近两天数据自动选项
 #
#/
/#V1.1.0 修正:
 # 修正基片名称显示颜色不正确问题
 # 修正运行过程中TCR测量界面可进入的问题
 # 追加报表可变温度显示
 # 刷新外部测量时，低阻优化和补偿测量取消选中状态
 # 增加修正计算补充，仅对实施修正且高温超过100℃实施修正
 # 增加TCR计算计算值选项
 # 对TCR测试过程中的外部测量禁用提示做出优化

#/
/# V1.1.1:
 # 增加版本显示标示,d表示调试版本呢,b表示修正版本
#/
/# V1.1.2:
 #  增加临时报表单项选择按钮
 #  增加按钮电阻选择图标
#/
/# V1.1.3:
 # 外部测量小数位调整
 # 计算TCR参数错误调整
 # 安定时间显示,自动打印开启
 #/
/#
 # V1.1.4
 # 修正方式改变
 # 修正报表变更
 # 修正外部测量异常中止(延时启用测试相关按钮)
 # V1.1.5
 # 优化查询结果,按时间顺序排序,调整显示宽度
 # 废除原有TCR设定计算值函数
 # 添加正式版本控制系统
 # V1.2.0.0
 # 调整代码结构，增加PRI文件
 # 修正外部测量报表格式
 # V1.2.0.1
 # 修正TCR报表,维持原有修正,对小于2000000以下维持实际值
 # 版本正式发布
 # V1.2.0.2
 # 增加基片选中后,基板名称获取焦点
 # V1.2.0.3
 # 增加ATCR计算功能,修正对应报表.
#/
