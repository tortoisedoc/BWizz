#include <QtGui/QApplication>
#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QDeclarativeExtensionPlugin>
#include <QDeclarativeView>
#include <QMainWindow>

#include "qmlapplicationviewer.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));    
    QmlApplicationViewer view;
    view.setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);

#ifdef WINHOST
    QUrl fullUrl(QCoreApplication::applicationDirPath() + "/../qml/BookmarkWizard/main.qml");
    view.addImportPath(QString( QCoreApplication::applicationDirPath() + "/../../plugins/debug"));
    //view.addImportPath(QString( "E:/n9/bookmarkwizard-build-simulator-simulator_qt_for_mingw_4_4__qt_sdk__debug/../../plugins/debug"));
#else
    QUrl fullUrl(QCoreApplication::applicationDirPath() + "/../qml/main.qml");
    view.addImportPath(QString( QCoreApplication::applicationDirPath() + "/../plugins"));    
#endif    
    //Tentative Fix for QtMobility.Gallery
    view.addImportPath(QString( "/usr/lib/qt4/plugins"));
    view.setSource(fullUrl);
    view.showFullScreen();
    return app->exec();
}
