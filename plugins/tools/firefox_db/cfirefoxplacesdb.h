#ifndef CFIREFOXPLACESDB_H
#define CFIREFOXPLACESDB_H

#include "../../Bookmark/cbookmark.h"
#include "dbmanager/cdbsqlitemanager.h"
#include "../ctoolbox.h"

#include <QMap>
#include <QList>
#include <QVariant>

#define FIREFOX_UUID_RADIX "0123456789abcdefghijklmnopqrstuvwxzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define FIREFOX_UUID_LEN 12

enum {
    FF_PLACES_DB_INVALID_PARAM = -40001
};

class CFirefoxPlacesDB : public CDBSQLiteManager {
private:
public:
    CFirefoxPlacesDB(QString filename);
    int getListOfBookmarks(CResultSet& resSet);
    int getMozPlaceByUrl(QString url, CResultSet& resSet);
    int getMozFavIconIdFromPlace(int placeId, CResultSet& res);
    int addMozPlace(QString url, QString title, int idFavIcon);
    int addMozBookmark(int idPlace, QString name, qint64 dateTime);
    int addMozIcon(QString url, QString type, QPixmap& pmap);
    int updateFavIcon(int placeId, QPixmap &pmap);
    int updateBookmark(QString title, int idBm, QString url, int idPlace);
    void removeBookmark(int idbm);
};

#endif // CFIREFOXPLACESDB_H
