#ifndef CFIREFOXBOOKMARK_H
#define CFIREFOXBOOKMARK_H
#include "cbookmark.h"
#include "../tools/firefox_db/cfirefoxplacesdb.h"

class CFirefoxBookmark : public CBookmark{
private:
    int m_idPlace;
    int m_idBookmark;
    int m_idFavIcon;    
    CFirefoxPlacesDB* m_ffDb;
protected:
    virtual void init();
public:
    CFirefoxBookmark(QString dbFile, QString catIcon, QPixmap& favIconData, QString title, QString url, int placeid, int bookmarkId, int idfavIcon, CFirefoxPlacesDB*db, QDeclarativeItem* parent=NULL);
    virtual QString getIconUrl() const;
    virtual void save();
    virtual bool remove();
    void setIDBookmark(int _id);
    void setIDPlace(int _id);
    Q_INVOKABLE virtual void setFavIcon(QPixmap& tempPixMap);
};

#endif // CFIREFOXBOOKMARK_H
