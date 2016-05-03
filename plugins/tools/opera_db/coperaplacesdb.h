#ifndef COPERAPLACESDB_H
#define COPERAPLACESDB_H

#include <QPixmap>
#include <list>
#include <QFile>

#define UINT unsigned int
#define USHORT unsigned short

struct OperaLink{
    int link_state;
    std::list<QString>linkStrings;
    unsigned char *UUID;
    unsigned char *title;
    bool locked;
};

struct OperaBookmark : public OperaLink{
    unsigned char* url;
    QPixmap favIcon;
};

struct OperaFolder :public OperaLink{
    unsigned char target;
//    USHORT subFolderCount;
//    USHORT bookmarkCount;
    std::list<QString> orderIDStrings;
    std::list<OperaBookmark*> bookmarkList;
    std::list<OperaFolder*>subfolderList;
};

class COperaPlacesDB
{
private:
    bool m_loaded;
    QString m_filename;
    QFile m_file;
    unsigned char m_verByte;
    OperaFolder* m_pRootFolder;
    std::list<OperaBookmark*> m_allBookmarks;

    void LoadLinkHeader(QDataStream &inFile, OperaLink * linkHeader);
    void SaveLinkHeader(QDataStream& outStream, OperaLink * parent);

    void LoadLinkStringList (USHORT count, QDataStream &inFile, std::list<QString> &parentlist);
    void SaveLinkStringList (USHORT count, QDataStream &outFile, std::list<QString> &parentlist);
    void LoadUUID (QDataStream &inFile, unsigned char ** uuid);

    void LoadWString (QDataStream &inFile, unsigned char ** wstr);
    void SaveWString (QDataStream &outFile, unsigned char * wstr);

    void LoadString (QDataStream &inFile, unsigned char ** str);

    void LoadFolder (QDataStream &inFile, OperaFolder*parent);
    void SaveFolder(QDataStream& outStream, OperaFolder* parent);

    void LoadBookmark (QDataStream &inFile, OperaBookmark*parent);
    void SaveBookmark (QDataStream& outStream, OperaBookmark* parent);

    void LoadFavIcon (QDataStream &inFile, QPixmap& tempPixmap);
    void SaveFavIcon (QDataStream &inFile, QPixmap& tempPixmap);    
public:
    std::list<OperaBookmark*>& GetBookMarkList();
    COperaPlacesDB();
    bool IsLoaded();
    void Load(QString dbFile);
    void Save(QString saveFile="");
    bool Remove(OperaFolder*parent, OperaBookmark* tempBm);
    void AddBookmark (OperaBookmark* tempBM);
    OperaBookmark* CreateNewBookmark();
};

#endif // COPERAPLACESDB_H
