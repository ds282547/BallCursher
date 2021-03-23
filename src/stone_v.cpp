#include "stone_v.h"

stone_v::stone_v(Pixmaps *px, int clor):Stone(px,clor,1){
    isSpecialStone = true;
}
QVector<Bar*>* stone_v::getBars(){
    QVector<Bar*>* v = new QVector<Bar*>();
    v->push_back(new Bar(col,0,10,0));
    return v;
}
