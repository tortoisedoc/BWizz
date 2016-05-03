#include "cfirefoxbookmark.h"
#include "../tools/image_provider/imageProvider.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>

// ====================================== CFirefoxBookmark

void CFirefoxBookmark::init(){
    m_bmType = TYPE_BM_FF;
    m_favIconSize.setHeight(64);
    m_favIconSize.setWidth(64);
}

QString CFirefoxBookmark::getIconUrl() const{
    return "image://" + QString(BM_IMGPROV_NAME) + "/#" + m_filename + "-" + QString(m_idPlace) + "@" + QString(m_idBookmark) + "-" + QString(m_imgIdRnd);  //Ne ejist nia te Firefox DB!
}

CFirefoxBookmark::CFirefoxBookmark(QString dbFile, QString catIcon, QPixmap& favIconData, QString title, QString url, int placeid, int bookmarkId, int idfavIcon, CFirefoxPlacesDB*db, QDeclarativeItem* parent)
    :CBookmark (dbFile, "", catIcon, url, title, parent){
    m_idFavIcon = idfavIcon;
    m_ffDb = db; //por i salve dopo
    setFavIcon(favIconData);
    m_encoding= ""; //Ne adorun nia pa Firefox DB!
    m_idPlace = placeid;
    m_idBookmark = bookmarkId;
    init();
}

void CFirefoxBookmark::save(){
    if (m_idFavIcon == 0){
        QString url = getUrl();
        if (url[url.length()-1] != '/')
            url.append("/");
        url.append("favicon.ico");
        m_idFavIcon = m_ffDb->addMozIcon( url, QString("image/png"), getFavIcon());
    }else{
        if (m_favIconchange)
            m_ffDb->updateFavIcon(m_idFavIcon, getFavIcon());
        m_favIconchange = false;
    }

    if (m_idPlace == 0){
        CResultSet tempSet;
        m_ffDb->getMozPlaceByUrl(getUrl(), tempSet);
        if (tempSet.getRowCount() > 0){
            m_idPlace = tempSet.getResult()["id"][0].toInt();
        }else { //Met le post
            m_idPlace = m_ffDb->addMozPlace(getUrl(), getName(), m_idFavIcon);
        }
    }
    if (m_idPlace > 0 && m_idBookmark == 0){
        qint64 dateTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        // Met l'icona
        m_idBookmark = m_ffDb->addMozBookmark(m_idPlace, getName(), dateTime);
    }else
        m_ffDb->updateBookmark(getName(), m_idBookmark, getUrl(), m_idPlace);
}

bool CFirefoxBookmark::remove(){
    m_ffDb->removeBookmark(m_idBookmark);
    return true;
}

void CFirefoxBookmark::setIDBookmark(int _id){
    m_idBookmark = _id;
}
void CFirefoxBookmark::setIDPlace(int _id){
    m_idPlace = _id;
}

void CFirefoxBookmark::setFavIcon(QPixmap& tempPixMap){
    m_favIcon = tempPixMap.scaled(m_favIconSize, Qt::KeepAspectRatio);
}

