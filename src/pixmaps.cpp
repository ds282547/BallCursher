#include "pixmaps.h"
#include <QDebug>
#include "gamedata.h"

Pixmaps::Pixmaps()
{
    QStringList names,types;
    types << "" << "v" << "h" << "b" << "s";
    names << "b" << "g" << "o" << "r";
    for(int i=0;i<4;i++){
        for(int j=0;j<5;j++){
            qp[i+j*4] = QPixmap(":/balls/pic/"+types.at(j)+names.at(i)+"b.png").scaled(GD::stoneSize,GD::stoneSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        }
    }
}

QPixmap Pixmaps::getPixmap(int color,int type){
    return qp[color+type*4];
}
