#include "cbookmark.h"
#include <QDir>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include "../tools/image_provider/imageprovider.h"

CBookmark::CBookmark(QDeclarativeItem* parent):QDeclarativeItem(parent)
{
    init();
}

void CBookmark::init(){
    m_bmType = TYPE_BM_BROWSER;
    m_favIconSize.setHeight(80);
    m_favIconSize.setWidth(80);
}

CBookmark::CBookmark(QString filename, QString uuid, QString catIcon, QDeclarativeItem* parent):QDeclarativeItem(parent)
{
    init();
    m_uuid = uuid;
    m_iconCategory = catIcon;
    m_imgIdRnd = qrand();
#ifdef _DEBUG
    qDebug() << QString("Loading ") + filename;
#endif
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.indexOf("Desktop Entry") == -1){
                QStringList tempList = line.split("=");
                if (tempList[0] == "Encoding")
                    m_encoding = tempList[1];
                else if (tempList[0] == "Type")
                    m_type = tempList[1];
                else if (tempList[0] == "Name"){
                    m_name = tempList[1];
                }
                else if (tempList[0] == "URL")
                    m_url = tempList[1];
                else if (tempList[0] == "Icon"){
                    m_iconFile = tempList[1];
                    QFile tempFile;
                    QString tempPath = m_iconFile.replace("//", "/");
                    if (tempFile.exists(tempPath)){
                        m_favIcon.load(tempPath);
                    }
                }
            }
        }
        m_filename = filename;
    }else m_filename = "";
}

CBookmark::CBookmark(QString filename, QString uuid, QString catIcon, QString url, QString name, QDeclarativeItem* parent):QDeclarativeItem(parent){
    m_uuid = uuid;
    m_filename = filename;
    m_url = url;
    m_imgIdRnd = qrand();
    m_name = name;
    m_iconFile = "";
    m_iconCategory = catIcon;
    init();
}



CBookmark::~CBookmark(){
}

QString CBookmark::id() const{
    return m_filename;
}

QHash<int, QByteArray>CBookmark::roleNames() const{
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[IconURLRole] = "iconurl";
    names[IconFileRole] = "iconfile";
    names[URLRole] = "url";
    names[CatIconRole] ="cat_icon";
    names[BMType]="type";
    return names;
}


QVariant CBookmark::data(int role) const{
    QString tempStr;
    switch(role) {
      case NameRole:
        return m_name;
      case IconFileRole:
        return m_iconFile;
      case IconURLRole:
        tempStr = getIconUrl();
        return tempStr;
      case URLRole:
        return m_url;
      case CatIconRole:
        return m_iconCategory;
      case BMType:
        return m_bmType;
      default:
        return QVariant();
    }
}

QString CBookmark::getName() {
    return m_name;
}
void CBookmark::setName(const QString name){
    m_name = name;
}
QString CBookmark::getUrl(){
    return m_url;
}
void CBookmark::setUrl(const QString url){
    m_url = url;
}
QString CBookmark::getIcon() {
    return m_iconFile;
}

QString CBookmark::getIconUrl() const{
    return "image://" + QString(BM_IMGPROV_NAME) + "/#" + m_iconFile + "-" + QString(m_imgIdRnd);
}

void CBookmark::setIcon(const QString iconPath){
    QString tempPath;
    QPixmap pmap;
    int fileURLIndex = iconPath.indexOf("file://");
    m_favIconchange = false;
    if (fileURLIndex >= 0){
        m_iconFile = iconPath.right(iconPath.length() - 7);
        pmap.load(m_iconFile);
        m_favIconchange = true;
    }else{
        if (iconPath.indexOf("image://") == -1){
            m_iconFile = iconPath;
            pmap.load(iconPath);
            m_favIconchange = true;
        }
    }
    if (m_favIconchange){
        m_imgIdRnd = qrand();
        setFavIcon(pmap);
    }
}
QPixmap& CBookmark::getFavIcon()
{
    return m_favIcon;
}
void CBookmark::setFavIcon(QPixmap& tempPixMap){
    m_favIcon = tempPixMap;
}
QString CBookmark::getCatIcon() {
    return m_iconCategory;
}
void CBookmark::setCatIcon(const QString icon){
    m_iconCategory = icon;
}

void CBookmark::setFileName(const QString filename){
    m_filename = filename;
}

bool CBookmark::setData(const int &index, const QVariant &value, int role){
    QPixmap pmap;
    switch(role) {
      case NameRole:        
        setName(value.toString());
        break;
      case IconFileRole:
        setIcon(value.toString());
        break;
      case URLRole:
        setUrl(value.toString());
        break;
      default:
        return false;
    }
    return true;
}


void CBookmark::save(){
    QFile file(m_filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        QTextStream out(&file);
        //out.setCodec("UTF-8");
        out << QString("[Desktop Entry]\n");
        out << QString("Encoding=" + m_encoding + "\n");
        QString temp("Type=Link\nName=" + m_name + "\nURL=" + m_url + "\nIcon=" + m_iconFile);
        out << temp;
        file.close();
    }
}

bool CBookmark::remove(){
    QFile file(m_filename);
    if (file.exists()){        
#ifdef WINDEBUG
        return true;
#else
        return file.remove();
#endif
    }
    return false;
}

