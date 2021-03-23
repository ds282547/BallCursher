#ifndef PIXMAPS_H
#define PIXMAPS_H

#include <QPixmap>
#include <QList>

class Pixmaps
{
public:
    Pixmaps();
    QPixmap qp[20];
    QPixmap getPixmap(int color,int type);
};

#endif // PIXMAPS_H
