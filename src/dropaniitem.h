#ifndef DROPANIITEM_H
#define DROPANIITEM_H
#include <stone.h>

class dropAniItem
{
public:
    Stone *st;
    int tx;
    int ty;
    int ox;
    int oy;
    dropAniItem(Stone *_st,int _tx,int _ty);
};

#endif // DROPANIITEM_H
