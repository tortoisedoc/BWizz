#include "ctoolbox.h"
#include <QTime>

CToolbox::CToolbox()
{
}


QString CToolbox::generateUUID(const char* radix, unsigned char len){
    QString guidstr;
    QString templateStr(radix);
    qsrand(QTime::currentTime().msec());
    for (int i = 0; i < len; i++){ //Generate random guid
        unsigned int tempVal = qrand()%templateStr.size();
        guidstr += templateStr[tempVal];
    }
    return guidstr;
}
