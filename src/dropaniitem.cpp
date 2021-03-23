#include "dropaniitem.h"

dropAniItem::dropAniItem(Stone *_st,int _tx,int _ty){
    st = _st;
    tx = _tx;
    ty = _ty;
    ox = st->x();
    oy = st->y();
}

