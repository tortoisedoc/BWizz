#include "coperaplacesdb.h"
#include <QFile>
#include <QIODevice>
#include <QBuffer>

COperaPlacesDB::COperaPlacesDB(){
    m_loaded = false;
    m_filename = "";
    m_pRootFolder = NULL;
}

std::list<OperaBookmark*>& COperaPlacesDB::GetBookMarkList(){
    return m_allBookmarks;
}

typedef unsigned char* uchar_ptr;

void COperaPlacesDB::LoadLinkStringList (USHORT count, QDataStream &inFile, std::list<QString>& parentlist){
    if (count > 0){
        USHORT cntr = 0;
        while (cntr < count){
            unsigned char * temp = NULL;
            LoadWString(inFile, &temp);
            parentlist.push_back(QString((char*)temp));
            free (temp);
            cntr++;
        }
    }
}

void COperaPlacesDB::SaveLinkStringList (USHORT count, QDataStream &outFile, std::list<QString> &parentlist){
    if (parentlist.size() > 0){
        foreach (QString tempStr, parentlist){
            SaveWString(outFile, (unsigned char*) tempStr.toStdString().c_str());
        }
    }
}



void COperaPlacesDB::LoadLinkHeader(QDataStream &inFile, OperaLink * linkHeader){
    inFile >> linkHeader->link_state;
    unsigned char linkStrCount = 0;
    inFile >> linkStrCount;
    LoadLinkStringList(linkStrCount, inFile, linkHeader->linkStrings);
    LoadUUID(inFile, &linkHeader->UUID);
    LoadWString(inFile, &linkHeader->title);
    inFile >> linkHeader->locked;
}

void COperaPlacesDB::SaveLinkHeader(QDataStream& outStream, OperaLink * linkHeader){
    outStream << linkHeader->link_state;
    unsigned char count = linkHeader->linkStrings.size() & 0xFF;
    outStream << count;
    SaveLinkStringList(count, outStream, linkHeader->linkStrings);
    SaveWString(outStream, linkHeader->UUID);
    SaveWString(outStream, linkHeader->title);
    outStream << linkHeader->locked;
}

void COperaPlacesDB::LoadUUID (QDataStream &inFile, unsigned char ** uuid){
    //Cartelles en pröma!
    LoadWString (inFile, uuid);
}

void COperaPlacesDB::LoadWString (QDataStream &inFile, unsigned char ** wstr){
    USHORT wStrLen;
    inFile >> wStrLen;
    *wstr = NULL;
    if (wStrLen > 0){
        *wstr = new unsigned char[wStrLen + 2];
        memset(*wstr, 0, wStrLen + 2);
        inFile.readRawData((char*)*wstr, wStrLen);
    }
}

void COperaPlacesDB::SaveWString (QDataStream &outFile, unsigned char * wstr){
    QString tempStr((char*)wstr);

    USHORT wStrLen = tempStr.length();
    outFile << wStrLen;
    if (wStrLen > 0){
        outFile.writeRawData((char*)wstr, wStrLen);
    }
}

void COperaPlacesDB::LoadString (QDataStream &inFile, unsigned char ** str){

    unsigned char strLen;
    inFile >> strLen;
    *str = NULL;
    if (strLen > 0){
        *str = new unsigned char[strLen + 1];
        memset(*str, 0, (strLen + 1));
        inFile.readRawData((char*)*str, strLen);
        //inFile >> strLen; //Null-term
    }

}

void COperaPlacesDB::LoadFolder (QDataStream &inFile, OperaFolder*parent){

    if (parent){
        LoadLinkHeader(inFile, parent);
        inFile >> parent->target;
        USHORT subFolderCount = 0;
        inFile >> subFolderCount;
        //Sotcartelles
        USHORT cntr = 0;
        while (cntr < subFolderCount){
            OperaFolder * tempFolder = new OperaFolder();
            LoadFolder(inFile, tempFolder);
            parent->subfolderList.push_back(tempFolder);
            cntr++;
        }
        cntr = 0;
        USHORT bookmarkCount;
        inFile >> bookmarkCount;
        while (cntr < bookmarkCount){
            OperaBookmark* tempSegno = new OperaBookmark();
            LoadBookmark(inFile, tempSegno);
            parent->bookmarkList.push_back(tempSegno);
            m_allBookmarks.push_back(tempSegno);
            cntr++;
        }
        //Lii ete la lista de ordine
        USHORT orderIDCount = 0;
        inFile >> orderIDCount;
        LoadLinkStringList(orderIDCount, inFile, parent->orderIDStrings);
    }
}

void COperaPlacesDB::SaveFolder(QDataStream& outStream, OperaFolder* parent){
    if (parent != NULL){
        SaveLinkHeader(outStream, parent);

        outStream << parent->target;
        USHORT foldercount = parent->subfolderList.size() & 0xFFFF;
        outStream << foldercount;
        //Sotcartelles
        USHORT cntr = 0;
        foreach (OperaFolder* tempFolder, parent->subfolderList){
            SaveFolder(outStream, tempFolder);
            cntr++;
        }
        cntr = parent->bookmarkList.size() & 0xFFFF;
        outStream << cntr;
        cntr = 0;
        foreach (OperaBookmark* tempSegno, parent->bookmarkList){
            SaveBookmark(outStream, tempSegno);
            cntr++;
        }
        //Lii ete la lista de ordine
        USHORT count = parent->orderIDStrings.size() & 0xFFFF;
        outStream << count;
        SaveLinkStringList(count, outStream, parent->orderIDStrings);
    }
}

void COperaPlacesDB::LoadFavIcon (QDataStream &inFile, QPixmap& tempPixmap){
    USHORT len;
    inFile >> len;
    if (len > 0){
        unsigned char * tempBuf = new unsigned char[len + 1];
        memset (tempBuf, 0x0, len + 1);
        inFile.readRawData((char*)tempBuf, len);
        QByteArray tempArr((char*)tempBuf, len);
        tempPixmap.loadFromData(tempArr, "PNG");
        tempArr.detach();
        delete tempBuf;
        tempBuf = NULL;
    }
}

void COperaPlacesDB::SaveFavIcon (QDataStream &outStream, QPixmap& tempPixmap){

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    tempPixmap.save(&buffer, "PNG");
    USHORT tempSize = buffer.size() & 0xFFFF;
    outStream << (USHORT)(tempSize);
    outStream.writeRawData(bytes.constData(), bytes.length() );
    buffer.close();
}

void COperaPlacesDB::LoadBookmark (QDataStream &inFile, OperaBookmark* parent){
    if (parent != NULL){
        LoadLinkHeader(inFile, parent);
        //Cartelles en pröma!
        LoadWString(inFile, &parent->url);
        LoadFavIcon(inFile, parent->favIcon);
    }
}

void COperaPlacesDB::SaveBookmark (QDataStream& outStream, OperaBookmark* parent){
    if (parent != NULL){
        SaveLinkHeader(outStream, parent);
        //Cartelles en pröma!
        SaveWString(outStream, parent->url);
        SaveFavIcon(outStream, parent->favIcon);
    }
}

bool COperaPlacesDB::IsLoaded(){
    return m_loaded;
}

void COperaPlacesDB::Load(QString dbFile){
    m_file.setFileName(dbFile);
    if (m_file.open(QIODevice::ReadOnly)){
        QDataStream in(&m_file);

        //get header
        in >> m_verByte;

#ifdef WINHOST
        in.setVersion(QDataStream::Qt_4_7);
        in.setByteOrder(QDataStream::BigEndian);
#endif
        m_pRootFolder = new OperaFolder();
        LoadFolder(in, m_pRootFolder);
        m_file.close();
        m_filename = dbFile;
        m_loaded = true;
    }
}

void COperaPlacesDB::Save(QString saveFile){
    if (saveFile == "")
        m_file.setFileName(m_filename);
    else
        m_file.setFileName(saveFile);

    if (m_file.open(QIODevice::WriteOnly)){
        QDataStream out(&m_file);
#ifdef WINHOST
        out.setVersion(QDataStream::Qt_4_7);
        out.setByteOrder(QDataStream::BigEndian);
#endif
        out << m_verByte;
        SaveFolder(out, m_pRootFolder);
        //trailer bytes
        unsigned char nuller = 0;
        out << nuller;
        out << nuller;
        out << nuller;
        out << nuller;
        out << nuller;
        out << nuller;
        out << nuller;
        m_file.close();
    }
}

bool COperaPlacesDB::Remove(OperaFolder*parent, OperaBookmark* tempBm){
    if (parent == NULL)
        parent = m_pRootFolder;
    if (parent != NULL){
        std::list<OperaBookmark*>::iterator it = parent->bookmarkList.begin();
        while (it != parent->bookmarkList.end() && (*it) != tempBm){
            it++;
        }
        if (it != parent->bookmarkList.end()){ //Ciafe!
            tempBm->linkStrings.clear();
            parent->orderIDStrings.remove(QString((char*)tempBm->UUID));

            parent->bookmarkList.remove(tempBm);
            tempBm->linkStrings.clear();
            if (tempBm->url != NULL)
                free (tempBm->url);
            if (tempBm->title != NULL)
                free (tempBm->title);
            if (tempBm->UUID != NULL)
                free (tempBm->UUID);
            return true;
        }else { // plü SOT tal som!
            foreach (OperaFolder* tempFolder, parent->subfolderList){
                if (Remove(tempFolder, tempBm))
                    return true;
            }
        }
        return false;
    }else return false;
}


void COperaPlacesDB::AddBookmark (OperaBookmark* tempBM){
    if (m_loaded && m_pRootFolder){
            m_pRootFolder->bookmarkList.push_back(tempBM);
            m_allBookmarks.push_back(tempBM);
    }
}

OperaBookmark* COperaPlacesDB::CreateNewBookmark(){
    OperaBookmark* tempSegno = new OperaBookmark();
    tempSegno->link_state = 0;
    tempSegno->locked = false;
    return tempSegno;
}
