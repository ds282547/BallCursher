#ifndef STONE_V_H
#define STONE_V_H

#include "stone.h"
#include <cstdlib>
class stone_v: public Stone
{
public:
    stone_v(Pixmaps *px,int clor);
    virtual QVector<Bar*>* getBars();
};

#endif // STONE_V_H
