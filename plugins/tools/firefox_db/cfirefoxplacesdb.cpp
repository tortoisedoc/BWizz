#include "cfirefoxplacesdb.h"
#include "../ctoolbox.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QBuffer>
#include <QIODevice>

#ifdef DEBUG
#include <QDebug>
#endif

#define SQL_SELECT_FF_BM "SELECT mp.id mpid, mb.id bmid, mb.title title, mp.url url, mf.id idfavicon, mf.data data"\
    " FROM (moz_bookmarks mb JOIN moz_places mp ON mb.fk = mp.id)"\
    " LEFT JOIN moz_favicons mf ON mp.favicon_id=mf.id;"

#define SQL_SELECT_MOZ_PLACE_BY_URL "SELECT id from moz_places where url LIKE :url;"
#define SQL_INSERT_MOZ_PLACE "INSERT INTO moz_places (url, title, favicon_id)"\
    " VALUES(:url, :title, :idfavicon);"
#define SQL_INSERT_MOZ_BOOKMARK "INSERT INTO moz_bookmarks(type, fk, parent, position, "\
    "title, dateAdded, lastModified, guid)VALUES (1, :idplace, 6, 0, :title, :dateadd, :lastmod, :guid)"
#define SQL_INSERT_MOZ_FAVICONS "INSERT INTO moz_favicons(url, data, mime_type, guid)"\
    " VALUES(:url, :data, :mime, :guid)"
#define SQL_UPDATE_MOZ_BOOKMARK "UPDATE moz_bookmarks set title=:title where id=:id;"
#define SQL_UPDATE_MOZ_PLACES "UPDATE moz_places set url=:url where id=:id;"
#define SQL_UPDATE_MOZ_FAVICON "UPDATE moz_favicons set data= :data where id= :id ;"
#define SQL_DELETE_MOZ_BOOKMARK "DELETE FROM moz_bookmarks where id=:id;"
#define SQL_SELECT_MOZ_FAVICONID "SELECT favicon_id from moz_places where id=:id;"

CFirefoxPlacesDB::CFirefoxPlacesDB(QString filename):CDBSQLiteManager("FFDBMGR", filename){
}

int CFirefoxPlacesDB::addMozIcon(QString url, QString type, QPixmap& pmap){

    //Arjigna ca guid
    QString guid = CToolbox::generateUUID(FIREFOX_UUID_RADIX, FIREFOX_UUID_LEN);

    //Arjigna ca la pilora
    QByteArray ba;
    QBuffer buffer( &ba );
    buffer.open( QIODevice::WriteOnly );
    pmap.save( &buffer, "PNG" );
    buffer.close();

    tDBQueryParamters params;
    params["url"] = url;
    params["data"] = ba;
    params["mime"] = type;
    params["guid"] = guid;
    int result = executeQuery(QString(SQL_INSERT_MOZ_FAVICONS), &params, NULL, true);
    return result;
}


int CFirefoxPlacesDB::getListOfBookmarks(CResultSet& resSet){
    return executeQuery(QString(SQL_SELECT_FF_BM), NULL, &resSet);
}

int CFirefoxPlacesDB::getMozPlaceByUrl(QString url, CResultSet& resSet){
    tDBQueryParamters params;
    params["url"] = url;
    return executeQuery(QString(SQL_SELECT_MOZ_PLACE_BY_URL), &params, &resSet);
}


int CFirefoxPlacesDB::addMozPlace(QString url, QString title, int idFavIcon){
    if (url != ""){
        tDBQueryParamters params;
        params["url"] = url;
        params["title"] = title;
        params["idfavicon"] = idFavIcon;
        return executeQuery(QString(SQL_INSERT_MOZ_PLACE), &params);
    }else return FF_PLACES_DB_INVALID_PARAM;
}


int CFirefoxPlacesDB::addMozBookmark(int idPlace, QString name, qint64 dateTime){
    tDBQueryParamters params;
    params["idplace"] = idPlace;
    params["title"] = name;
    params["dateadd"] = dateTime;
    params["lastmod"] = dateTime;
    QString guid = CToolbox::generateUUID(FIREFOX_UUID_RADIX, FIREFOX_UUID_LEN);
    params["guid"] = guid;
    return executeQuery(QString(SQL_INSERT_MOZ_BOOKMARK), &params, NULL, true);
}



int CFirefoxPlacesDB::updateBookmark(QString title, int idBm, QString url, int idPlace){
    if (title != "" && url !=""){
        tDBQueryParamters params;
        params["title"] = title;
        params["id"] = idBm;
        executeQuery(QString(SQL_UPDATE_MOZ_BOOKMARK), &params, NULL, true);
        params.clear();
        params["url"] = url;
        params["id"] = idPlace;
        executeQuery(QString(SQL_UPDATE_MOZ_PLACES), &params, NULL, true);
        return 0;
    }else return FF_PLACES_DB_INVALID_PARAM;

}

void CFirefoxPlacesDB::removeBookmark(int idbm){
    if (idbm > 0){
        tDBQueryParamters params;
        params["id"] = idbm;
        executeQuery(QString(SQL_DELETE_MOZ_BOOKMARK), &params, NULL, true);
    }
}


int CFirefoxPlacesDB::getMozFavIconIdFromPlace(int placeId, CResultSet& res){
    int result = 0;
    if (placeId > 0){
        tDBQueryParamters params;
        params["id"] = placeId;
        result = executeQuery(QString(SQL_SELECT_MOZ_FAVICONID), &params, &res);
    }
    else result = FF_PLACES_DB_INVALID_PARAM;
    return result;
}

int CFirefoxPlacesDB::updateFavIcon(int favId, QPixmap &pmap){
    int result = 0;
    CResultSet tempSet;
    if (favId > 0){

        tDBQueryParamters params;
        params["id"] = favId;

        QByteArray ba;
        QBuffer buffer( &ba );
        buffer.open( QIODevice::WriteOnly );
        pmap.save( &buffer, "PNG" );
        buffer.close();

        params["data"] = ba;
        result = executeQuery(QString(SQL_UPDATE_MOZ_FAVICON), &params, NULL, true);

        //result = execSQL(QString(SQL_UPDATE_MOZ_FAVICON).arg(ba.data()).arg(favId), tempSet);
    }
    return result;
}
