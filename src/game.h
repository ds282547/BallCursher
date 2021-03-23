#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QVector>
#include <QGraphicsScene>
#include <QTimer>

#include "stone.h"
#include "stone_base.h"
#include "stone_v.h"
#include "stone_h.h"
#include "stone_star.h"
#include "stone_boom.h"

#include "pixmaps.h"
#include "bar.h"
#include "dropaniitem.h"


#include <QTimeLine>

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT

public:

    explicit Game(QWidget *parent = 0);
    QGraphicsScene *gpScreen;
    Stone *stones[10][10];
    void setStonesPos();
    void initStones();
    //gametime
    QTimer *gametimer;
    //change
    Stone *change_1;
    Stone *change_2;
    //timeline and animation
    QTimeLine *tl1;
    int seled;
    int seled2;
    qreal tx1,ty1,tx2,ty2;
    ~Game();
    // swap
    void checkMove();
    //clear ani
    void startClearAni();
    //drop ani
    void startDropAni();
    QVector<Stone*> extra_stones;
    QVector<dropAniItem*> drops;
    // clear combo
    int Combo;
    int maxCombo;
    void updateCombo();
    QVector<Bar*> bars;
    QVector<int> clearStones;
    bool changeable = false;
    void resetStone();
    // special stone
    void resetSpecialStone();
    QVector<int> specials;
    // scores
    qreal scores;
    qreal _scr;
    void addScores(qreal sc);
    void endGame();
    qreal gt;
    void test();
public slots:
    void selectStone(int r,int c);

    void change_ani1_timer(int frame);
    void change_ani1_finish();
    void change_ani2_timer(int frame);
    void change_ani2_finish();
    void clear_ani_timer(int frame);
    void clear_ani_finish();
    void drop_ani_timer(int frame);
    void drop_ani_finish();
    void gametimer_timeout();
private:
    Ui::Game *ui;
    Pixmaps *pixs;
signals:
    void quit(int star,int scores);
};

#endif // GAME_H
