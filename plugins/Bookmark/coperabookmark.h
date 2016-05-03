#ifndef COPERABOOKMARK_H
#define COPERABOOKMARK_H

#include "cbookmark.h"
#include "tools/opera_db/coperaplacesdb.h"

#define OPERA_UUID_RADIX "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define OPERA_UUID_LEN  32

class COperaBookmark : public CBookmark
{
private:
    OperaBookmark* m_operaSegno;
    COperaPlacesDB * m_db;
protected:
    virtual void init();
public:
    COperaBookmark(OperaBookmark * segno, QString catIcon, QDeclarativeItem* parent=NULL, COperaPlacesDB*db = NULL);
    CBookmark* addToHomeScreen(QString home, QString iconsHome);
    OperaBookmark* getOperaBookmarkData();
    virtual QString getIconUrl() const;
    virtual void save();
    virtual bool remove();
    Q_INVOKABLE virtual void setName(const QString name);
    Q_INVOKABLE virtual void setUrl(const QString url);
    Q_INVOKABLE virtual void setFavIcon(QPixmap& tempPixMap);
};

#endif // COPERABOOKMARK_H
