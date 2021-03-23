#ifndef STONE_BOOM_H
#define STONE_BOOM_H

#include "stone.h"
#include <cstdlib>
class stone_boom : public Stone
{
public:
    stone_boom(Pixmaps *px,int clor);
    virtual QVector<Bar*>* getBars();
};
#endif // STONE_BOOM_H
