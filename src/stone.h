#ifndef STONE_H
#define STONE_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QColor>

#include "pixmaps.h"
#include "bar.h"

class Stone : public QObject, public QGraphicsPixmapItem
{
     Q_OBJECT

public:
    Stone(Pixmaps *_pix,int clor,int tp);
    int type;
    int color;
    int row;
    int col;
    virtual QVector<Bar*>* getBars();
    bool isSpecialStone = false;
    void Pos(int r,int c);
    void toogleSel();
    void upd();
    void setColor(int clor);
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    Stone *operator + (const Stone& st);

signals:
    void cccc(int r,int c);
private:
    bool sel;

protected:
    Pixmaps *pix;
};

#endif // STONE_H
