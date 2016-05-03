#ifndef CImageProvider_H
#define CImageProvider_H

#include <QDeclarativeImageProvider>
#include "Bookmark/cbookmark.h"
#include <QMap>
#include <QPixmap>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Declarative)

#define BM_IMGPROV_NAME "bmfavicon"

class CImageProvider : public QDeclarativeImageProvider
{    
public:
    CImageProvider();
    virtual ~CImageProvider(){
        m_bmMap.clear();
    }
    void addBMToMap(CBookmark*bm);
    virtual QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
    virtual QImage requestImage(const QString &id, QSize *size, const QSize& requestedSize){
        return QImage();
    }
    static CImageProvider* Instance();
    CBookmark* getBMById(QString id);
private:
    QMap<QString, CBookmark*> m_bmMap;
};

QT_END_NAMESPACE
QT_END_HEADER
#endif // CImageProvider_H
