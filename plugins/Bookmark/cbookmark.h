#ifndef CBOOKMARK_H
#define CBOOKMARK_H

#include <QString>
#include <QVariant>
#include <QDeclarativeItem>

enum Roles {
    NameRole = Qt::UserRole+1,
    IconFileRole, //34
    IconURLRole,
    URLRole,
    CatIconRole,
    BMType
};

enum BMTypes{
    TYPE_BM_BROWSER = 0,
    TYPE_BM_FF,
    TYPE_BM_OPERA,
};

class CBookmark : public QDeclarativeItem{
    Q_OBJECT
    /*Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QString url READ getUrl WRITE setUrl)
    Q_PROPERTY(QString icon READ getIcon WRITE setIcon)
    */
protected:
    QString m_filename;    
    QString m_encoding;
    QString m_type;
    QString m_name;
    QString m_url;
    QString m_iconFile;
    QString m_iconCategory;    
    int m_bmType;
    QSize m_favIconSize;
    QPixmap m_favIcon;
    QString m_uuid;
    bool m_favIconchange;
    int m_imgIdRnd;
    virtual void init();
public:
    CBookmark(QDeclarativeItem* parent = 0);
    CBookmark(QString filename, QString uuid, QString catIcon, QDeclarativeItem* parent = 0);
    CBookmark(QString filename, QString uuid, QString catIcon, QString url, QString name, QDeclarativeItem* parent = 0);

    ~CBookmark();
    QString id() const;
    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;
    bool setData(const int &index, const QVariant &value, int role = Qt::EditRole);

public: //Getters & setters
    QPixmap& getFavIcon();
    //void setFavIcon(QPixmap& tempPixMap);
    QString getName();
    Q_INVOKABLE virtual void setName(const QString name);
    QString getUrl();
    Q_INVOKABLE virtual void setUrl(const QString url);
    QString getIcon();
    Q_INVOKABLE virtual void setIcon(const QString iconPath);
    virtual QString getIconUrl() const;
    void setFileName(const QString filename);
    QString getCatIcon();
    Q_INVOKABLE void setCatIcon(const QString icon);
    Q_INVOKABLE virtual void setFavIcon(QPixmap& tempPixMap);
    int getType(){
        return m_bmType;
    }
    void setType(int type){
        m_bmType = type;
    }
    virtual void save();
    virtual bool remove();
};

#endif // CBOOKMARK_H
