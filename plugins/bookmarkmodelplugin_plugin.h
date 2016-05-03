#ifndef BOOKMARKMODELPLUGIN_PLUGIN_H
#define BOOKMARKMODELPLUGIN_PLUGIN_H

#include <QtDeclarative/QDeclarativeExtensionPlugin>

class BookmarkModelPluginPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
    
public:
    void registerTypes(const char *uri);
    void initializeEngine(QDeclarativeEngine *engine, const char *uri);
};

#endif // BOOKMARKMODELPLUGIN_PLUGIN_H

