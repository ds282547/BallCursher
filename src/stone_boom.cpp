#include "stone_boom.h"
#include <QDebug>

stone_boom::stone_boom(Pixmaps *px, int clor):Stone(px,clor,3){
    isSpecialStone = true;
}
QVector<Bar*>* stone_boom::getBars(){
    QVector<Bar*>* v = new QVector<Bar*>();
    int x=row;
    int y=col-1;
    int l=3;
    if(y<0){
        y=0;
        l=2;
    }
    if(y>7){
        l=2;
    }

    if(x>0){
            v->push_back(new Bar((x-1)*10+y,0,0,l));
    }
    if(x<9){
            v->push_back(new Bar((x+1)*10+y,0,0,l));
    }
    v->push_back(new Bar(x*10+y,0,0,l));
    return v;
}
