TEMPLATE = subdirs

uri = com.giulietta.bookmarktools
# Additional import path used to resolve QML modules in Creator's code model

CONFIG(debug, debug|release){
    DEFINES +=DEBUG
	message(BOOKMARKWIZARD CONFIGURATION DEBUG)
}else{
#	DEFINES +=LOGGER_ON
	message(BOOKMARKWIZARD CONFIGURATION RELEASE )
}

win32:{
    QT_INSTAL_IMPORTS=frontend
    QML_IMPORT_PATH = $$[QT_INSTALL_IMPORTS]/$$replace(uri, \\., \\)
    message( WIN32 )
}else:{
    message (HARMATTAN)

    QML_IMPORT_PATH = ./plugins/$$replace(uri, \\., /)
}
message(QML_IMPORT_PATH is $$QML_IMPORT_PATH)
SUBDIRS += frontend
SUBDIRS += ../qtapi/logger
SUBDIRS += ../qtapi/dbmanager
SUBDIRS += plugins

#CONFIG(debug, debug|release){
#    OTHER_FILES += \
#        qtc_packaging/debian_harmattan/rules_debug
#}else{
    OTHER_FILES += \
        qtc_packaging/debian_harmattan/rules
#}

OTHER_FILES += \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog
