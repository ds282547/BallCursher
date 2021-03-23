#ifndef STONE_STAR_H
#define STONE_STAR_H

#include "stone.h"
#include <cstdlib>
class stone_star : public Stone
{
public:
    stone_star(Pixmaps *px,int clor);
    virtual QVector<Bar*>* getBars();
};
#endif // STONE_STAR_H
