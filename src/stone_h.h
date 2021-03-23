#ifndef STONE_H_H
#define STONE_H_H

#include "stone.h"
#include <cstdlib>
class stone_h : public Stone
{
public:
    stone_h(Pixmaps *px,int clor);
    virtual QVector<Bar*>* getBars();
};
#endif // STONE_H_H
