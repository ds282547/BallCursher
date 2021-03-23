#include "stone_h.h"

stone_h::stone_h(Pixmaps *px, int clor):Stone(px,clor,2){
    isSpecialStone = true;
}
QVector<Bar*>* stone_h::getBars(){
    QVector<Bar*>* v = new QVector<Bar*>();
    v->push_back(new Bar(row*10,0,0,10));
    return v;
}
