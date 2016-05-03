#include "imageProvider.h"

QT_BEGIN_NAMESPACE

CImageProvider::CImageProvider() :
    QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap), m_bmMap(){
}

void CImageProvider::addBMToMap(CBookmark *bm){
    if (bm){
        QString iconStr = bm->getIconUrl();
        QString id = iconStr.mid(iconStr.indexOf("#"));
        m_bmMap.remove(id);
        m_bmMap[id] = bm;
    }
}

CBookmark* CImageProvider::getBMById(QString id){
    CBookmark* bm = m_bmMap[id];
    if (bm != NULL)
        return bm;
    else
        return NULL;
}

QPixmap CImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize){
    CBookmark*tempBm = Instance()->getBMById(id);
    if (tempBm == NULL)
        return QPixmap();
    else {
        if (size){
            size->setWidth(tempBm->getFavIcon().width());
            size->setHeight(tempBm->getFavIcon().height());
        }
        if(requestedSize.width() > 0){
/*            QImage image(tempBm->getFavIcon().toImage());
            QImage out(image.width(), image.height(), QImage::Format_ARGB32);
            out.fill(Qt::transparent);

            QBrush brush(image);

            QPen pen;
            pen.setColor(Qt::transparent);
            pen.setJoinStyle(Qt::RoundJoin);

            QPainter painter(&out);
            painter.setBrush(brush);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setPen(pen);
            painter.drawRoundedRect(0, 0, requestedSize.width(), requestedSize.height(), 40.0, 40.0);

            if (!out.isNull())
                return QPixmap::fromImage(out);
            else
                return QPixmap();
*/
            QPixmap temp = tempBm->getFavIcon().scaled(requestedSize, Qt::IgnoreAspectRatio);
            if (!temp.isNull())
                return temp;
            else return QPixmap();

        }
        else return tempBm->getFavIcon();
    }
}

CImageProvider* CImageProvider::Instance(){
    static CImageProvider s_SegnoImageProvider;
    return &s_SegnoImageProvider;
}

QT_END_NAMESPACE
