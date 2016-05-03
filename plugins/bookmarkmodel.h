#ifndef BOOKMARKMODEL_H
#define BOOKMARKMODEL_H

#include <QtDeclarative/QDeclarativeItem>
#include <QAbstractListModel>
#include <QDir>
#include "Bookmark/cbookmark.h"
#include "tools/image_provider/imageprovider.h"
#include "tools/firefox_db/cfirefoxplacesdb.h"
#include "tools/opera_db/coperaplacesdb.h"

#define DEFAULT_BROWSER_CAT_ICON "/usr/share/themes/blanco/meegotouch/icons/icon-l-browser.png"
#define DEFAULT_FENNEC_CAT_ICON "/usr/share/themes/blanco/meegotouch/icons/icon-l-fennec.png"
#define DEFAULT_OPERA_CAT_ICON "/usr/share/icons/hicolor/80x80/apps/operamobile-labs.png"

#define DEFAULT_FENNEC_HOME "/.mozilla/fennec/"
#define DEFAULT_OPERA_HOME "/.config/operamobile-labs/ds"

class Bookmarkmodel : public QAbstractListModel, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(int count READ count)
    Q_DISABLE_COPY(Bookmarkmodel)
private:
    void Init(const QHash<int,QByteArray> &roleNames);
public:
    int count() const { return rowCount(QModelIndex()); }
    virtual void classBegin();
    virtual void componentComplete();

public:
    explicit Bookmarkmodel(QObject *parent = 0);
    explicit Bookmarkmodel(CBookmark& prototype, QObject *parent = 0);
    explicit Bookmarkmodel(CBookmark& prototype, const QString &dir, QObject*parent = 0);
    void InitModel();
    ~Bookmarkmodel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex &index) const;
    Q_INVOKABLE bool setData(const int &index, const QVariant &value, int role = Qt::EditRole);
    Q_INVOKABLE void saveCurrentEdit(const int &index);
    Q_INVOKABLE void deleteCurrent(const int &index);
    Q_INVOKABLE void exportCurrentBookMarkToHomeScreen(const int &index);
    Q_INVOKABLE void exportCurrentBookMarkToFirefox(const int &index);
    Q_INVOKABLE void exportCurrentBookMarkToOpera(const int &index);
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()){ return false;}
    Q_INVOKABLE bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());    

    Q_INVOKABLE bool isFireFoxAvailable();
    Q_INVOKABLE bool isOperaAvailable();
private:
    CBookmark* getBookmark(int index);
    QString m_bookmarksHome;
    QString m_fennecHome;
    QMap<QString, CFirefoxPlacesDB*> m_FFProfileDBs;
    QList<CBookmark*> m_bookmarks;
    COperaPlacesDB m_operaBMDB;
    int m_FFIDMobile;

    void LoadBookmarks(const QDir &dir);
    void LoadFirefoxBookmarksFromDb(CFirefoxPlacesDB * db);
    void LoadFirefoxBookmarks(QString fennecHome);
    void LoadOperaBookmarks(QString operaHome);
    void addToHomeScreen(CBookmark* bm, QString home, QString iconsHome);
    void addToFireFox(CBookmark* bm, CFirefoxPlacesDB * fireFoxDb);
    void addToOpera(CBookmark* bm);
    QString getHomePath();
    QString getFFHomePath();
    QString getOperaHomePath();
};

QML_DECLARE_TYPE(Bookmarkmodel)

#endif // BOOKMARKMODEL_H

