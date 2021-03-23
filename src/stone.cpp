#include "stone.h"
#include <QDebug>

Stone::Stone(Pixmaps *_pix,int clor,int tp)
{
    pix = _pix;
    color = clor;
    type = tp;
    sel = true;
    this->setPixmap(pix->getPixmap(clor,tp));
}

void Stone::mousePressEvent(QGraphicsSceneMouseEvent * event){
    emit cccc(row,col);
}


void Stone::toogleSel(){
    if(sel){
        QGraphicsDropShadowEffect *qe = new QGraphicsDropShadowEffect();
        qe->setColor(QColor(255,230,208));
        qe->setBlurRadius(28);
        qe->setXOffset(0);
        qe->setYOffset(0);
        this->setGraphicsEffect(qe);

    }else{

        this->setGraphicsEffect(0);
    }
    sel = !sel;
}
void Stone::Pos(int r,int c){
    this->row=r;
    this->col=c;
}
void Stone::upd(){
    this->setPixmap(pix->getPixmap(color,type));
}
void Stone::setColor(int clor){
    this->color = clor;
    this->upd();
}

Stone *Stone::operator + (const Stone& st){
    this->type = st.type;
    this->color = st.color;
    this->setX(st.x());
    this->setY(st.y());
    this->upd();
    return this;
}

QVector<Bar*>* Stone::getBars(){

}
