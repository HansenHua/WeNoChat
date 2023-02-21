#-------------------------------------------------
#
# Project created by QtCreator 2022-08-18T21:50:15
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WeNoChat
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_FILE = icon/icon.rc

SOURCES += main.cpp \
    socket/basesock.cpp \
    socket/datasock.cpp \
    socket/filesock.cpp \
    UI/chatmessagewidget.cpp \
    UI/friendlist.cpp \
    UI/login.cpp \
    UI/mainwindow.cpp \
    UI/password.cpp \
    UI/question.cpp \
    UI/regist.cpp \
    UI/searchfriends.cpp \
    UI/user.cpp \
    UI/viewfullmessage.cpp \
    utils/communicate_utils.cpp \
    utils/tools.cpp \
    messagerecord.cpp \
    constants.cpp \
    utils/jsoncpp.cpp \
    UI/picturecut.cpp \
    UI/mydelegate.cpp \
    UI/group.cpp \
    socket/cmd.cpp \
    utils/resourcemanager.cpp \
    UI/basewidget.cpp \
    UI/ui_windows.cpp \
    UI/netset.cpp \
    socket/sockinfo.cpp

HEADERS  += \
    socket/basesock.h \
    socket/datasock.h \
    socket/filesock.h \
    UI/chatmessagewidget.h \
    UI/friendlist.h \
    UI/login.h \
    UI/mainwindow.h \
    UI/password.h \
    UI/question.h \
    UI/regist.h \
    UI/searchfriends.h \
    UI/user.h \
    UI/viewfullmessage.h \
    utils/communicate_utils.h \
    utils/tools.h \
    utils/json/json.h \
    utils/json/json-forwards.h \
    messagerecord.h \
    constants.h \
    UI/picturecut.h \
    UI/mydelegate.h \
    UI/group.h \
    socket/cmd.h \
    utils/resourcemanager.h \
    UI/basewidget.h \
    UI/ui_windows.h \
    UI/netset.h \
    socket/sockinfo.h

FORMS    += \
    UI/login.ui \
    UI/mainwindow.ui \
    UI/password.ui \
    UI/question.ui \
    UI/regist.ui \
    UI/searchfriends.ui \
    UI/user.ui \
    UI/viewfullmessage.ui \
    UI/picturecut.ui \
    UI/group.ui \
    UI/netset.ui

RESOURCES += \
    uires.qrc \

DISTFILES += icon/icon.rc
