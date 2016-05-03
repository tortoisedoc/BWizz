#ifndef CTOOLBOX_H
#define CTOOLBOX_H
#include <QString>

class CToolbox
{
public:
    CToolbox();
    static QString generateUUID(const char* radix, unsigned char len);
};

#endif // CTOOLBOX_H
