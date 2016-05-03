#include "bookmarkmodelplugin_plugin.h"
#include "bookmarkmodel.h"
#include "Bookmark/cbookmark.h"
#include <QDeclarativeEngine>

#include <QtDeclarative/qdeclarative.h>

#ifndef DEBUG
#include "logger/filelogger.h"
#else
#include "logger/debuglogger.h"
#endif

void BookmarkModelPluginPlugin::registerTypes(const char *uri)
{
    // @uri com.giulietta.bookmarktools
    int res = qmlRegisterType<Bookmarkmodel>(uri, 1, 0, "Bookmarkmodel");
    LOG_DEBUG(" Bookmarkmodel registering result %d", res);
    res = qmlRegisterType<CBookmark>(uri, 1, 0, "Bookmark");
    LOG_DEBUG(" CBookmark registering result %d", res);
}
void BookmarkModelPluginPlugin::initializeEngine(QDeclarativeEngine *engine, const char *uri){
    LOG_DEBUG(" ADDING IMAGE PROVIDER %s", "");
    engine->addImageProvider(QLatin1String(BM_IMGPROV_NAME), CImageProvider::Instance());
    LOG_DEBUG(" IMAGE PROVIDER ADDED! %s", "");
}

Q_EXPORT_PLUGIN2(BookmarkModelPlugin, BookmarkModelPluginPlugin)


