# NOTA POR WINDOWS (MEEGO):
#   TE MESSES COPIE LA DLL DAL PLUGIN A MAN TAL
#      E:\QtSDK\Simulator\Qt\mingw\imports\com\giulietta\bookmarktools
#   STE OS ADORE LE SIMULATORE!

win32:{
    DEPLOYMENTFOLDERS=    
    message(ADDING WINHOST TO DEFINES)
    DEFINES+=WINHOST
    CONFIG(debug, debug|release){
        DEFINES+=WINDEBUG
    }
}

CONFIG(debug, debug|release){
	DEFINES += DEBUG
	configuration = debug
}else{
	#DEFINES += LOGGER_ON
	configuration = release
}

TEMPLATE = lib
TARGET = BookmarkModelPlugin
QT += declarative sql
CONFIG += qt
CONFIG += plugin
CONFIG += mobility
MOBILITY += gallery

INCLUDEPATH += ../../qtapi/

uri = com.giulietta.bookmarktools


message(OUT_PWD IS $$OUT_PWD)

# Input
SOURCES += \
	bookmarkmodelplugin_plugin.cpp \
	bookmarkmodel.cpp \
	Bookmark/cbookmark.cpp \
	Bookmark/cfirefoxbookmark.cpp \
	Bookmark/coperabookmark.cpp \
	tools/firefox_db/cfirefoxplacesdb.cpp \
	tools/opera_db/coperaplacesdb.cpp \
    tools/image_provider/imageprovider.cpp \
    tools/ctoolbox.cpp

HEADERS += \
    bookmarkmodelplugin_plugin.h \
    bookmarkmodel.h \
    Bookmark/cbookmark.h \
    Bookmark/cfirefoxbookmark.h \
    tools/firefox_db/cfirefoxplacesdb.h \
    Bookmark/coperabookmark.h \
    tools/opera_db/coperaplacesdb.h \
    tools/image_provider/imageprovider.h \
    tools/ctoolbox.h

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    message(SETUP EXTRA MAKE TARGER)
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    message(qmldir.commands is $$copy_qmldir.commands)
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

win32{
    qmldir.files = qmldir_win
    qmldir.target = qmldir
}else{
    qmldir.files = qmldir
}
target.files = libBookmarkModelPlugin.so


symbian {
    message(SYMBIAN)
    TARGET.EPOCALLOWDLLDATA = 1

} else:unix {
    message(UNIX)
    maemo5 | !isEmpty(MEEGO_VERSION_MAJOR) {
        installPath = /opt/frontend/plugins/$$replace(uri, \\., /)
    } else {
        installPath = $$[QT_INSTALL_IMPORTS]/$$replace(uri, \\., /)
    }
    qmldir.path = $$installPath
    target.path = $$installPath
    message (TARGER PATH A IS $$target.path)
    INSTALLS += target qmldir
} else:win32{
    message (QT_INSTAL_IMPORTS IS $$[QT_INSTALL_IMPORTS])
    #orPath = $$[QT_INSTALL_IMPORTS]\\$$replace(uri, \\., \\)
    orPath = $$OUT_PWD\\$$replace(uri, \\., \\)
    pluginPath = $$replace(orPath, \\\, /)
    installPath = $$pluginPath
    message(WINDOWS - install Path IS $$installPath)
    qmldir.path = $$installPath
    #target.path = $$OUT_PWD"/debug/BookmarkModelPlugin.dll"
    message(WINDOWS - target.source IS $$target.source)
    #target.target = debug/com/giulietta/bookmarktools
    #DEPLOYMENTFOLDERS += target qmldir
    #DEPLOYMENTFOLDERS += target
    message ($$qmldir.path)
    message ($$qmldir.source)
    message ($$qmldir.target)

 #   DEPLOYMENTFOLDERS += qmldir
}


win32:{
	PRE_TARGETDEPS += $$PWD/../../qtapi/dbmanager/$$configuration/libdbmanager.a
	PRE_TARGETDEPS += $$PWD/../../qtapi/logger/$$configuration/liblogger.a

	LIBS += -L$$PWD/../../qtapi/logger/$$configuration/ -ldbmanager
	LIBS += -L$$PWD/../../qtapi/dbmanager/$$configuration/ -ldbmanager
}
else:unix:!macx:!symbian {
	PRE_TARGETDEPS += $$PWD/../../qtapi/dbmanager/libdbmanager.a
	PRE_TARGETDEPS += $$PWD/../../qtapi/logger/liblogger.a
	LIBS += -L$$PWD/../../qtapi/dbmanager/ -ldbmanager
	LIBS += -L$$PWD/../../qtapi/logger/ -llogger
}

DEPENDPATH += $$PWD/../../../qtapi/logger/
DEPENDPATH += $$PWD/../../../qtapi/dbmanager/

# Please do not modify the following two lines. Required for deployment.
!win32:{
    include(deployment.pri)
    qtcAddDeployment()
}

OTHER_FILES += \
    libBookmarkModelPlugin.so

