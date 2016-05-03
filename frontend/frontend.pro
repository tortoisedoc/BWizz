# Add more folders to ship with the application, here

win32{
    folder_01.path = qml/BookmarkWizard
    folder_01.source = qml/BookmarkWizard
    folder_01.target = qml
    message(ADDING WINHOST TO DEFINES)
    DEFINES += WINHOST
}
else:{
    folder_01.path = qml/BookmarkWizard/globals.js
    folder_01.source = qml/BookmarkWizard/globals.js
    folder_01.target = qml

    folder_02.path = qml/BookmarkWizard/*.qml
    folder_02.source = qml/BookmarkWizard/*.qml
    folder_02.target = qml

    folder_03.path = images/qml/*.png
    folder_03.source = images/qml/pin.png
    folder_03.target = qml

    DEPLOYMENTFOLDERS += folder_01
    DEPLOYMENTFOLDERS += folder_02
    DEPLOYMENTFOLDERS += folder_03




    folder_04.files += images/bwizz_icon80.png
    folder_04.path = /usr/share/icons/hicolor/80x80/apps/
    folder_04.target = /usr/share/icons/hicolor/80x80/apps/

    folder_05.files += images/bwizz_icon64.png
    folder_05.path = /usr/share/icons/hicolor/80x80/apps/
    folder_05.target = /usr/share/icons/hicolor/80x80/apps/

    INSTALLS += folder_04
    INSTALLS += folder_05
#    DEPLOYMENTFOLDERS += folder_03
#    DEPLOYMENTFOLDERS += folder_04

}
DEPLOYMENTFOLDERS += folder_01


symbian:TARGET.UID3 = 0xE36585A6

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# this CRAAAAA_AAA_SHES!
#CONFIG += mobility
#MOBILITY += gallery

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable
QT += quick

# Add dependency to Symbian components
# CONFIG += qt-components

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp

OTHER_FILES += \
    frontend_harmattan.desktop \
    ../plugins/qmldir \
    qml/BookmarkWizard/globals.js \
    qml/BookmarkWizard/bwizz_icon.gif \
    qml/BookmarkWizard/main.qml \
    qml/BookmarkWizard/NoBookmarks.qml \
    qml/BookmarkWizard/Gallery.qml \
    qml/BookmarkWizard/EditBMPage.qml \
    qml/BookmarkWizard/ConfirmationBox.qml

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()


