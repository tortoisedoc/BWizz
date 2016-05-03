#include "coperabookmark.h"
#include "../tools/image_provider/imageprovider.h"

void COperaBookmark::init(){
    m_bmType = TYPE_BM_FF;
    m_favIconSize.setHeight(64);
    m_favIconSize.setWidth(64);
}

COperaBookmark::COperaBookmark(OperaBookmark * segno, QString catIcon, QDeclarativeItem* parent, COperaPlacesDB * db)
    :CBookmark(QString(""), QString(""), catIcon, QString((char*)segno->url), QString((char*)segno->title), parent){
    m_db = db;
    m_operaSegno = segno;
    m_uuid = QString((char*)segno->UUID);
    setFavIcon(m_operaSegno->favIcon);
    m_encoding= ""; //Ne adorun nia pa Opera DB!
    m_bmType = TYPE_BM_OPERA;
}

QString COperaBookmark::getIconUrl() const {
    return "image://" + QString(BM_IMGPROV_NAME) + "/#" + m_uuid + "-" + QString(m_imgIdRnd);
}

void COperaBookmark::save(){
    free(m_operaSegno->title);
    m_operaSegno->title = (unsigned char*)new char[m_name.length() + 1];
    memset(m_operaSegno->title, 0 , m_name.length() + 1);
    strcpy ((char*)m_operaSegno->title, m_name.toStdString().c_str());

    free(m_operaSegno->url);
    m_operaSegno->url = (unsigned char*)new char[m_url.length() + 1];
    memset(m_operaSegno->url, 0 , m_url.length() + 1);
    strcpy ((char*)m_operaSegno->url, m_url.toStdString().c_str());
    m_operaSegno->favIcon = m_favIcon;
    m_db->Save();
}

bool COperaBookmark::remove(){
    if (m_db && m_db->IsLoaded()){
        m_db->Remove(NULL, m_operaSegno);
        m_db->Save();
    }
}

OperaBookmark* COperaBookmark::getOperaBookmarkData(){
    return m_operaSegno;
}


void COperaBookmark::setName(const QString name){
    CBookmark::setName(name);
    if(m_operaSegno->title)
        free (m_operaSegno->title);
    m_operaSegno->title = new unsigned char [name.length() + 1];
    strcpy ((char*)m_operaSegno->title, name.toStdString().c_str());
}

void COperaBookmark::setUrl(const QString url){
    CBookmark::setUrl(url);
    if(m_operaSegno->url)
        free (m_operaSegno->url);
    m_operaSegno->url = new unsigned char [url.length() + 1];
    strcpy ((char*)m_operaSegno->url, url.toStdString().c_str());
}

void COperaBookmark::setFavIcon(QPixmap& tempPixMap){
    m_favIcon = tempPixMap.scaled(m_favIconSize, Qt::KeepAspectRatio);
}
