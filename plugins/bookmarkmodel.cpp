#include <QtDeclarative/qdeclarative.h>
#include <QDeclarativeEngine>
#include <QTime>
#include <QSettings>
#include <QDir>
#include <QFile>

#include "bookmarkmodel.h"

#include "Bookmark/coperabookmark.h"
#include "Bookmark/cfirefoxbookmark.h"
#include "Bookmark/cbookmark.h"
#include "tools/image_provider/imageProvider.h"
#include "tools/ctoolbox.h"

#ifndef DEBUG
#include "logger/filelogger.h"
#else
#include "logger/debuglogger.h"
#endif

Bookmarkmodel::Bookmarkmodel(CBookmark& prototype, QObject *parent):
    QAbstractListModel(parent), m_bookmarks()
{
    Init(prototype.roleNames());
}

Bookmarkmodel::Bookmarkmodel(QObject *parent):
    QAbstractListModel(parent), m_bookmarks(){
    CBookmark cb;
    Init(cb.roleNames());
}

Bookmarkmodel::Bookmarkmodel(CBookmark& prototype, const QString &dir, QObject *parent): QAbstractListModel(parent), m_bookmarks(), m_bookmarksHome(dir){
    Init(prototype.roleNames());
}

Bookmarkmodel::~Bookmarkmodel(){
}

void Bookmarkmodel::Init(const QHash<int,QByteArray> &roleNames){
    setRoleNames(roleNames);
    m_bookmarksHome = getHomePath();
    m_fennecHome = getFFHomePath();
    LOG_DEBUG(" -----> Inbuild browser link home : %s", m_bookmarksHome);
    LOG_DEBUG(" -----> Fennec home : %s", m_fennecHome);
}

QString Bookmarkmodel::getHomePath(){
#ifdef DEBUG
    #ifndef WINDEBUG
        return QString("/home/user/.local/share/applications/");
    #else
        return QString(QDir::homePath() + "/.local/share/applications/");
    #endif
#else
    return QString(QDir::homePath() + "/.local/share/applications/");
#endif
}

bool Bookmarkmodel::isFireFoxAvailable(){
    QDir tempDir;
    return tempDir.exists(getFFHomePath());
}

bool Bookmarkmodel::isOperaAvailable(){
    QDir tempDir;
    return tempDir.exists(getOperaHomePath());
}

QString Bookmarkmodel::getFFHomePath(){
#ifdef DEBUG
    return QString ("/home/user/").append(DEFAULT_FENNEC_HOME);
#else
    return QDir::homePath() + DEFAULT_FENNEC_HOME;
#endif
}

QString Bookmarkmodel::getOperaHomePath(){
#ifdef DEBUG
    #ifndef WINDEBUG
        return QString ("/home/user/").append(DEFAULT_OPERA_HOME);
    #else
        return QString ("E:/n9/BWizz/testdata/wintest");
    #endif
#else   
    return QDir::homePath() + DEFAULT_OPERA_HOME;
#endif
}


bool Bookmarkmodel::removeRows(int position, int rows, const QModelIndex &index){
    position = 0;
    rows = 0;
    return true;
}
CBookmark* Bookmarkmodel::getBookmark(int index) {
    if (index >= m_bookmarks.size() || index < 0)
        return NULL;
    else return m_bookmarks[index];
}


int Bookmarkmodel::rowCount(const QModelIndex &parent) const
{
    return m_bookmarks.count();
}

QVariant Bookmarkmodel::data(const QModelIndex &index, int role) const
{
    int index_ = index.row();
    const CBookmark* tempBm = ((Bookmarkmodel*)this)->getBookmark(index_);
    if (tempBm)
        return tempBm->data(role);
    else return QVariant();
}



QVariant Bookmarkmodel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

/*    if (orientation == Qt::Horizontal)
        return QString("Column %1").arg(section);
    else
        return QString("Row %1").arg(section);
*/
    return QString ("Bookmarks");
}


Qt::ItemFlags Bookmarkmodel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}


void Bookmarkmodel::classBegin(){
}

void Bookmarkmodel::componentComplete(){
    this->LoadBookmarks(m_bookmarksHome);
    //Search for Firefox Bookmarks
    this->LoadFirefoxBookmarks(getFFHomePath());
    //..and Opera Bookmarks
    this->LoadOperaBookmarks(getOperaHomePath());
}

bool Bookmarkmodel::setData(const int &index, const QVariant &value, int role){

    CBookmark * tempBm = getBookmark(index);
    if (tempBm){
        bool ret = tempBm->setData(index, value, role);
        return ret;
    }
    else return false;
}

void Bookmarkmodel::saveCurrentEdit(const int &index){
    CBookmark* tempBm = ((Bookmarkmodel*)this)->getBookmark(index);
    if (tempBm){
        tempBm->save();
        //readd to image list
        CImageProvider::Instance()->addBMToMap(tempBm);
        QModelIndex _index = this->index(index, 0);
        this->dataChanged(_index, _index);
    }
}


void Bookmarkmodel::deleteCurrent(const int &index){
    CBookmark* tempBm = ((Bookmarkmodel*)this)->getBookmark(index);
    if (tempBm){        
        if (tempBm->remove()){
            m_bookmarks.removeAt(index);
            QModelIndex _index = this->index(index, 0);
            this->beginRemoveRows(QModelIndex(), index, index);
            this->removeRow(index);
            this->endRemoveRows();
            free(tempBm);
        }
    }
}

// LOADERS -----------------------------------------------

void Bookmarkmodel::LoadFirefoxBookmarks(QString fennecHome){
    QFile fennecProf;    
    QString fennecProfIni (fennecHome + "/profiles.ini");
    if (fennecProf.exists(fennecProfIni)){
        LOG_DEBUG("Loading Fennec profiles from %s", fennecProfIni);
        QSettings profiles(fennecProfIni, QSettings::IniFormat);
        foreach(QString sectName, profiles.childGroups()){
            if (sectName.indexOf("Profile") > -1){ //Profile!
                QString tempDBPath (fennecHome  + profiles.value(sectName + "/Path").toString() + "/places.sqlite");
                LOG_DEBUG("Loading Fennec bookmarks for profile %s", sectName);
                if (fennecProf.exists(tempDBPath)){
                    CFirefoxPlacesDB* tempDB = new CFirefoxPlacesDB(tempDBPath);

                    m_FFProfileDBs[profiles.value(sectName + "/Name").toString()] = tempDB;
                    this->LoadFirefoxBookmarksFromDb(tempDB);
                }
            }
        }
    }
}


void Bookmarkmodel::LoadFirefoxBookmarksFromDb(CFirefoxPlacesDB* db){
    LOG_DEBUG("loading firefox bookmarks from %s", db->getFileName());
    if (db && db->Open() == DB_CONN_MGR_OK){
        CResultSet tempResSet;
        db->getListOfBookmarks(tempResSet);
        for (int i = 0; i < tempResSet.getRowCount(); i ++){
            QList<QVariant> tempList = tempResSet.getResult()["data"];
            if (((QList<QVariant>&)tempResSet.getResult()["url"])[i].toString().indexOf("about") != 0){
                QPixmap tempPM;
                tempPM.loadFromData(tempList[i].toByteArray());
                CFirefoxBookmark *tempBm = new CFirefoxBookmark (db->getFileName(),
                     DEFAULT_FENNEC_CAT_ICON,
                     tempPM,
                     ((QList<QVariant>&)tempResSet.getResult()["title"])[i].toString(),
                     ((QList<QVariant>&)tempResSet.getResult()["url"])[i].toString(),
                     ((QList<QVariant>&)tempResSet.getResult()["mpid"])[i].toInt(),
                     ((QList<QVariant>&)tempResSet.getResult()["bmid"])[i].toInt(),
                     ((QList<QVariant>&)tempResSet.getResult()["idfavicon"])[i].toInt(),
                     db);
                m_bookmarks.push_front(tempBm);
                //Add to picture loader
                CImageProvider::Instance()->addBMToMap(tempBm);
            }
        }
        db->Close();
    }
}

void Bookmarkmodel::LoadOperaBookmarks(QString operaHome){
    QFile operaBMDB;
    QString operaDBFile(operaHome);
    operaDBFile.append("/2_all");
    if (operaBMDB.exists(operaDBFile)){
        LOG_DEBUG("loading Opera browser bookmarks from %s", operaHome);
        m_operaBMDB.Load(operaDBFile);
        foreach (OperaBookmark *segno, m_operaBMDB.GetBookMarkList()){
            COperaBookmark * tempSegno = new COperaBookmark(segno, QString(DEFAULT_OPERA_CAT_ICON),NULL, &m_operaBMDB);
            m_bookmarks.push_front(tempSegno);
            CImageProvider::Instance()->addBMToMap(tempSegno);
        }
#if 0       //mat por test
        QString operaDBResFile(operaHome);
        operaDBResFile.append("/2_noeall");
        m_operaBMDB.Save(operaDBResFile);
#endif
    }
}

void Bookmarkmodel::LoadBookmarks(const QDir &dir){
    LOG_DEBUG("loading inbuilt browser bookmarks from %s", dir.absolutePath());
    QStringList filters;
    filters << "browser-*.desktop";
    QStringList files;
    files = dir.entryList(filters);	// filter only desktop files

    foreach (QString filename, files){
        QString fullName = dir.absolutePath() + "/" + filename;
        CBookmark *tempBm = new CBookmark(fullName, "", DEFAULT_BROWSER_CAT_ICON);
        m_bookmarks.push_front(tempBm);
        //Add to picture loader
        CImageProvider::Instance()->addBMToMap(tempBm);
    }
}


// EXPORTERS ------------------------------------

void Bookmarkmodel::exportCurrentBookMarkToHomeScreen(const int &index){
    CBookmark* tempBm = ((Bookmarkmodel*)this)->getBookmark(index);
    if (tempBm){
        switch (tempBm->getType()){
            case TYPE_BM_FF:
            case TYPE_BM_OPERA:
                addToHomeScreen(tempBm, m_bookmarksHome, QDir::homePath() + "/.bwizz/images");
            break;
        }
        this->reset();
    }
}

void Bookmarkmodel::exportCurrentBookMarkToFirefox(const int &index){
    if (isFireFoxAvailable()){
        CBookmark* tempBm = ((Bookmarkmodel*)this)->getBookmark(index);
        if (tempBm){
            switch (tempBm->getType()){
                case TYPE_BM_OPERA:
                case TYPE_BM_BROWSER:
                    addToFireFox(tempBm, m_FFProfileDBs["default"]);
                break;
            }
            this->reset();
        }
    }
}

void Bookmarkmodel::exportCurrentBookMarkToOpera(const int &index){
    if (isOperaAvailable()){
        CBookmark* tempBm = ((Bookmarkmodel*)this)->getBookmark(index);
        if (tempBm){
            switch (tempBm->getType()){
                case TYPE_BM_FF:
                case TYPE_BM_BROWSER:
                    addToOpera(tempBm);
                break;
            }
            this->reset();
        }
    }
}

void Bookmarkmodel::addToHomeScreen(CBookmark* bm, QString home, QString iconsHome){
    //export image

    if (bm){
        qsrand(QTime::currentTime().msec());
        unsigned int tempVal = qrand();
        QDir iconDir;

        if (!iconDir.exists(iconsHome))
            iconDir.mkpath(iconsHome);
        char tempName[300];
        sprintf(tempName, "/browser-bwizzimg-%d.png", tempVal);
        const QString targetImgFile(iconsHome + tempName);
        sprintf(tempName,"/browser-bwizzbookmark-%d.desktop", tempVal);
        const QString bmFile (home + tempName);
        bm->getFavIcon().save(targetImgFile, "PNG");
        //Export bookmark
        CBookmark * newBM = new CBookmark();
        if (newBM){
            newBM->setCatIcon(DEFAULT_BROWSER_CAT_ICON);
            newBM->setName(bm->getName());
            newBM->setUrl(bm->getUrl());
            newBM->setFileName(bmFile);
            newBM->setIcon(targetImgFile);
            newBM->setType(TYPE_BM_BROWSER);
            newBM->save();
            m_bookmarks.push_back(newBM);
            CImageProvider::Instance()->addBMToMap(newBM);
        }
    }
}

void Bookmarkmodel::addToFireFox(CBookmark* bm, CFirefoxPlacesDB * fireFoxDb){
    if (bm
        && (fireFoxDb->Open() == DB_CONN_MGR_OK)
        ){

        CFirefoxBookmark * nbm = new CFirefoxBookmark(fireFoxDb->getFileName(), DEFAULT_FENNEC_CAT_ICON, bm->getFavIcon(), bm->getName(), bm->getUrl(), 0, 0, 0, fireFoxDb );
        if (nbm){
            nbm->save();
            m_bookmarks.push_back(nbm);
            CImageProvider::Instance()->addBMToMap(nbm);
        }
        fireFoxDb->Close();
    }
}

void Bookmarkmodel::addToOpera(CBookmark* bm){
    if (m_operaBMDB.IsLoaded()){
        OperaBookmark * tempOBM = m_operaBMDB.CreateNewBookmark();
        QString tempStr = bm->getName();
        int tempLen = tempStr.length()+ 1;
        tempOBM->title = new unsigned char[2*tempLen];
        tempStr = bm->getUrl();
        tempLen = tempStr.length()+ 1;
        tempOBM->url = new unsigned char[2*tempLen];
        QString strUUID = CToolbox::generateUUID(OPERA_UUID_RADIX, OPERA_UUID_LEN);
        tempOBM->UUID = new unsigned char[OPERA_UUID_LEN];
        strcpy ((char*)tempOBM->UUID, strUUID.toStdString().c_str());
        COperaBookmark * tempBM = new COperaBookmark(tempOBM, DEFAULT_OPERA_CAT_ICON, NULL, &m_operaBMDB);
        tempBM->setUrl(bm->getUrl());
        tempBM->setName(bm->getName());
        tempBM->setFavIcon(bm->getFavIcon());
        m_operaBMDB.AddBookmark(tempBM->getOperaBookmarkData());
        tempBM->save();
        m_bookmarks.push_back(tempBM);
        //Add to picture loader
        CImageProvider::Instance()->addBMToMap(tempBM);

    }
}

