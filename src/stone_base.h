#ifndef STONE_BASE_H
#define STONE_BASE_H

#include "stone.h"
#include <cstdlib>
class stone_base : public Stone
{
public:
    stone_base(Pixmaps *px,int clor);
    virtual QVector<Bar*>* getBars();
};

#endif // STONE_BASE_H
