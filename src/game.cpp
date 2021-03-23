#include "game.h"
#include "ui_game.h"
#include "gamedata.h"
#include <QDebug>
#include <ctime>


extern const qreal stoneWidth = 50;
extern const qreal stoneHeight= 50;

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    //init screen
    gpScreen = new QGraphicsScene();
    pixs = new Pixmaps();

    ui->graphicsView->setScene(gpScreen);
    initStones();
    setStonesPos();
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    //selected stone
    seled = -1;
    //combo & -1 --> no animaiton played
    Combo = -1;
    maxCombo = 0;
    scores = 0;
    _scr = 0;
    //timer
    gametimer = new QTimer(this);
    gametimer->start(100);
    gt=120;
    connect(gametimer,SIGNAL(timeout()),this,SLOT(gametimer_timeout()));
}
Game::~Game()
{
    int scr = scores;
    int str;
    if(scr<1200000){
        str=0;
    }else if(scr<1500000){
        str=1;
    }else if(scr<1900000){
        str=2;
    }else{
        str=3;
    }
    emit quit(str,scr);
    delete ui;
}
void Game::gametimer_timeout(){
    gt-=0.1;
    ui->label_5->setText(QString::number(gt));
    if(gt<=0){
        disconnect(gametimer,SIGNAL(timeout()),this,SLOT(gametimer_timeout()));
        ui->label_5->setText("0");
        endGame();
    }
    //scores
    _scr = (scores-_scr)*0.8+_scr;
    int scr = _scr;
    QString s = QString("%1").arg(scr).rightJustified(10, '0');
    ui->label_3->setText(s);
}
void Game::endGame(){
    int scr = scores;
    int str;
    if(scr<1200000){
        str=0;
    }else if(scr<1500000){
        str=1;
    }else if(scr<1900000){
        str=2;
    }else{
        str=3;
    }
    emit quit(str,scr);
    this->close();
}
void Game::updateCombo(){
    if(Combo>maxCombo){
        maxCombo = Combo;
        ui->label_6->setText(QString::number(maxCombo));
    }
}
void Game::addScores(qreal sc){
    scores+=sc;
}

void Game::setStonesPos(){
    for(int i=0;i<GD::stoneCount;i++){
        for(int j=0;j<GD::stoneCount;j++){
            stones[i][j]->setPos(i*GD::stoneSize,j*GD::stoneSize);
        }
    }
}
void Game::initStones(){
    QVector<int> k;
    srand(time(NULL));
    for(int i=0;i<GD::stoneCount;i++){

        for(int j=0;j<GD::stoneCount;j++){
            k.clear();
            k << 0 << 1 << 2 << 3;
            if(i>1){
                if(stones[i-1][j]->color == stones[i-2][j]->color){
                    k.removeAll(stones[i-1][j]->color);
                }
            }
            if(j>1){
                if(stones[i][j-1]->color == stones[i][j-2]->color){
                    k.removeAll(stones[i][j-1]->color);
                }
            }

            stones[i][j] = new stone_base(pixs, k.at(rand()%k.length()));
            //set position data
            stones[i][j]->Pos(i,j);
            //set signals
            connect(stones[i][j],SIGNAL(cccc(int,int)),this,SLOT(selectStone(int,int)));
            gpScreen->addItem(stones[i][j]);
        }
    }

    //animation
}

void Game::selectStone(int r,int c){
    if(!(r*GD::stoneCount+c==seled)&&Combo==-1){
        int dist = (r*GD::stoneCount+c)-seled;
        if(seled>-1){
            if(dist == 1 || dist == -1 || dist == GD::stoneCount || dist == -GD::stoneCount){
                stones[seled/GD::stoneCount][seled%GD::stoneCount]->toogleSel();

                seled2 = r*GD::stoneCount+c;

                //prepare animation

                change_1 = new Stone(pixs,0,0);
                change_1 = *change_1 + *stones[seled/GD::stoneCount][seled%GD::stoneCount];
                change_2 = new Stone(pixs,0,0);
                change_2 = *change_2 + *stones[seled2/GD::stoneCount][seled2%GD::stoneCount];
                stones[seled/GD::stoneCount][seled%GD::stoneCount]->setOpacity(0);
                stones[seled2/GD::stoneCount][seled2%GD::stoneCount]->setOpacity(0);
                tx1 = change_1->x();
                ty1 = change_1->y();
                tx2 = change_2->x();
                ty2 = change_2->y();
                gpScreen->addItem(change_1);
                gpScreen->addItem(change_2);

                //calc the move
                //combo
                Combo = 0;
                //swap
                int temp,temp2;
                temp = stones[seled/GD::stoneCount][seled%GD::stoneCount]->color;
                temp2 = stones[seled/GD::stoneCount][seled%GD::stoneCount]->type;
                stones[seled/GD::stoneCount][seled%GD::stoneCount]->type = stones[seled2/GD::stoneCount][seled2%GD::stoneCount]->type;
                stones[seled/GD::stoneCount][seled%GD::stoneCount]->setColor(stones[seled2/GD::stoneCount][seled2%GD::stoneCount]->color);
                stones[seled2/GD::stoneCount][seled2%GD::stoneCount]->type = temp2;
                stones[seled2/GD::stoneCount][seled2%GD::stoneCount]->setColor(temp);


                checkMove();

                tl1 = new QTimeLine(500);
                tl1->setFrameRange(0,500);
                connect(tl1,SIGNAL(frameChanged(int)),this,SLOT(change_ani1_timer(int)));
                connect(tl1,SIGNAL(finished()),this,SLOT(change_ani1_finish()));
                tl1->start();

            }else{
                stones[seled/GD::stoneCount][seled%GD::stoneCount]->toogleSel();
                seled = r*GD::stoneCount+c;
                stones[r][c]->toogleSel();
            }
        }else{
            seled = r*GD::stoneCount+c;
            stones[r][c]->toogleSel();
        }
    }
}
void Game::checkMove(){
    int i,j;

    int barhead,barbody;
    resetSpecialStone();
    bars.clear();
    for(i=0;i<GD::stoneCount;i++){
        barhead = -1;
        barbody = 1;
        for(j=0;j<GD::stoneCount;j++){
            if(barhead == stones[i][j]->color){
                barbody++;
            }else{
                if(barbody>2)
                    bars.push_back(new Bar(i*GD::stoneCount+j-1,barhead,0,barbody));
                barhead = stones[i][j]->color;
                barbody = 1;
            }

            //special effect
        }
        if(barbody>2)
            bars.push_back(new Bar(i*GD::stoneCount+j-1,barhead,0,barbody));
    }
    for(i=0;i<GD::stoneCount;i++){
        barhead = -1;
        barbody = 1;
        for(j=0;j<GD::stoneCount;j++){
            if(barhead == stones[j][i]->color){
                barbody++;
            }else{
                if(barbody>2)
                    bars.push_back(new Bar((j-1)*GD::stoneCount+i,barhead,barbody,0));
                barhead = stones[j][i]->color;
                barbody = 1;
            }
        }
        if(barbody>2)
            bars.push_back(new Bar((j-1)*GD::stoneCount+i,barhead,barbody,0));
    }
    if(bars.length()>0){
        foreach(Bar *b,bars){
            //special stones couter
            //does not special effect
            if(!b->speciEFF){
                if((b->xlong)>3){
                     specials.push_back(1*4+b->barhead); //vectical + color

                }
                if((b->ylong)>3){
                     specials.push_back(2*4+b->barhead); //horizon + color
                }
                if(b->xlong==0){
                    int x,y,ccc;
                    bool f = true;
                    x = b->pos/GD::stoneCount;
                    y = b->pos%GD::stoneCount;

                    if(x>1){
                        for(int p=0;p<b->ylong;p++){

                            ccc = stones[x][y-p]->color;

                            if(f&stones[x-1][y-p]->color==ccc&&stones[x-2][y-p]->color==ccc&&!(b->ylong==3&&p==1)){
                                specials.push_back(3*4+b->barhead); //boom
                                f = false;
                            }
                        }
                    }
                    if(x<GD::stoneCount-2&&f){
                        for(int p=0;p<b->ylong;p++){
                            ccc = stones[x][y-p]->color;
                            if(f&stones[x+1][y-p]->color==ccc&&stones[x+2][y-p]->color==ccc&&!(b->ylong==3&&p==1)){
                                specials.push_back(3*4+b->barhead); //boom
                                f = false;
                            }
                        }
                    }
                }
                if((b->xlong)>4||(b->ylong)>4){
                     specials.push_back(4*4+b->barhead); //star
                }
            }

            clearStones.push_back(b->pos);
            for(i=1;i<b->xlong;i++){
                clearStones.push_back(b->pos-i*10);
            }
            for(j=1;j<b->ylong;j++){
                clearStones.push_back(b->pos-j);
            }

        }
        //specil
        QVector<Bar*> sp;
        foreach(int k,clearStones){
            i = k/GD::stoneCount;
            j = k%GD::stoneCount;
            if(stones[i][j]->isSpecialStone){
                if(stones[i][j]->type<4){
                    QVector<Bar*> k;
                    k = *stones[i][j]->getBars();
                    for(int q=0;q<k.length();q++)
                        sp.push_back(k.at(q));
                }else{
                    //star
                    for(int d=0;d<GD::stoneCount;d++){
                        for(int e=0;e<GD::stoneCount;e++){
                            if(stones[d][e]->color==stones[i][j]->color)
                                clearStones.push_back(d*GD::stoneCount+e);
                        }
                    }
                }
            }
        }

        foreach(Bar *b,sp){
            clearStones.push_back(b->pos);
            for(i=1;i<b->xlong;i++){
                clearStones.push_back(b->pos+i*10);
            }
            for(j=1;j<b->ylong;j++){
                clearStones.push_back(b->pos+j);
            }
        }

        Combo++;
        updateCombo();
    }
}
void Game::change_ani1_timer(int frame){
    qreal rate = frame / 500.0;

    change_1->setX(tx1+(tx2-tx1)*rate);
    change_1->setY(ty1+(ty2-ty1)*rate);
    change_2->setX(tx2+(tx1-tx2)*rate);
    change_2->setY(ty2+(ty1-ty2)*rate);
}
void Game::change_ani1_finish(){
    if(Combo>0){
        disconnect(tl1,SIGNAL(frameChanged(int)),this,SLOT(change_ani1_timer(int)));
        disconnect(tl1,SIGNAL(finished()),this,SLOT(change_ani1_finish()));
        gpScreen->removeItem(change_1);
        gpScreen->removeItem(change_2);
        delete tl1,change_1,change_2;
        //vision & selection
        stones[seled/GD::stoneCount][seled%GD::stoneCount]->setOpacity(1);
        stones[seled2/GD::stoneCount][seled2%GD::stoneCount]->setOpacity(1);
        seled = -1;


        //start to clear stone
        startClearAni();



    }else{
        //swap back
        tl1 = new QTimeLine(300);
        tl1->setFrameRange(0,500);
        connect(tl1,SIGNAL(frameChanged(int)),this,SLOT(change_ani2_timer(int)));
        connect(tl1,SIGNAL(finished()),this,SLOT(change_ani2_finish()));
        tl1->start();
    }

}
void Game::change_ani2_timer(int frame){
    qreal rate = frame / 500.0;

    change_2->setX(tx1+(tx2-tx1)*rate);
    change_2->setY(ty1+(ty2-ty1)*rate);
    change_1->setX(tx2+(tx1-tx2)*rate);
    change_1->setY(ty2+(ty1-ty2)*rate);

}
void Game::change_ani2_finish(){
    disconnect(tl1,SIGNAL(frameChanged(int)),this,SLOT(change_ani2_timer(int)));
    disconnect(tl1,SIGNAL(finished()),this,SLOT(change_ani2_finish()));
    gpScreen->removeItem(change_1);
    gpScreen->removeItem(change_2);
    delete tl1,change_1,change_2;
    //swap back
    int temp,temp2;
    temp = stones[seled/GD::stoneCount][seled%GD::stoneCount]->color;
    temp2 = stones[seled/GD::stoneCount][seled%GD::stoneCount]->type;
    stones[seled/GD::stoneCount][seled%GD::stoneCount]->type = stones[seled2/GD::stoneCount][seled2%GD::stoneCount]->type;
    stones[seled/GD::stoneCount][seled%GD::stoneCount]->setColor(stones[seled2/GD::stoneCount][seled2%GD::stoneCount]->color);
    stones[seled2/GD::stoneCount][seled2%GD::stoneCount]->type = temp2;
    stones[seled2/GD::stoneCount][seled2%GD::stoneCount]->setColor(temp);

    //vision & selection
    stones[seled/GD::stoneCount][seled%GD::stoneCount]->setOpacity(1);
    stones[seled2/GD::stoneCount][seled2%GD::stoneCount]->setOpacity(1);

    resetStone();
    seled = -1;
    //combo back to -1
    Combo = -1;
}
void Game::startClearAni(){
    tl1 = new QTimeLine(300);
    tl1->setFrameRange(0,500);
    connect(tl1,SIGNAL(frameChanged(int)),this,SLOT(clear_ani_timer(int)));
    connect(tl1,SIGNAL(finished()),this,SLOT(clear_ani_finish()));
    tl1->start();
}
void Game::clear_ani_timer(int frame){
    int x,y;
    qreal op;
    foreach(int p,clearStones){
        op = 1 - frame / 500.0;
        x=p/GD::stoneCount;
        y=p%GD::stoneCount;
        stones[x][y]->setOpacity(op);
    }
}

void Game::clear_ani_finish(){
    //clear stones base data
    //addScroes
    int x,y;
    qreal sc=0;
    foreach(int p,clearStones){
        x=p/GD::stoneCount;
        y=p%GD::stoneCount;
        if(stones[x][y]->color>-1)
            sc += 1000+rand()%2000;
        stones[x][y]->color = -1; // maker empty
        stones[x][y]->type = 0;
    }
    sc = sc*(qreal(Combo)+10)/10;
    addScores(sc);
    disconnect(tl1,SIGNAL(frameChanged(int)),this,SLOT(clear_ani_timer(int)));
    disconnect(tl1,SIGNAL(finished()),this,SLOT(clear_ani_finish()));
    delete tl1;
    startDropAni();
}
void Game::test(){
    for(int i=0;i<10;i++){
        qDebug() << stones[0][i]->type<< stones[1][i]->type<< stones[2][i]->type<< stones[3][i]->type<< stones[4][i]->type<< stones[5][i]->type<< stones[6][i]->type<< stones[7][i]->type<< stones[8][i]->type << stones[9][i]->type;

    }
}

void Game::startDropAni(){
    int i,j,k,c,max_empty;
    int all;
    int counter;
    int ept_num[10];
    max_empty = 0;
    all=0;
    for(i=0;i<GD::stoneCount;i++){
        c=0;
        for(j=GD::stoneCount-1;j>=0;j--){ // bottom to top
            if(stones[i][j]->color==-1){
                c++;
            }else if(c>0){
                drops.push_back(new dropAniItem(stones[i][j],i*GD::stoneSize,(j+c)*GD::stoneSize));
                stones[i][j+c]->color = stones[i][j]->color;
                stones[i][j+c]->type = stones[i][j]->type;
                stones[i][j]->type = 0;
            }
        }
        ept_num[i]=c;
        if(c>max_empty){
            max_empty = c;
        }
        all += c;
    }
    //special stones
    QVector<int> nums;
    QVector<int> pos;
    for(i=0;i<all;i++){
        nums.push_back(i);
    }
    for(i=0;i<specials.length();i++){
        int l=nums.at(rand()%nums.length());
        pos.push_back(l);
        nums.removeOne(l);
    }
    counter = 0;
    for(i=0;i<10;i++){
        for(j=0;j<ept_num[i];j++){
            stones[i][j]->color = rand()%4;
            if(pos.contains(counter)){
                stones[i][j]->type=specials.last()/4;
                stones[i][j]->color=specials.last()%4;
                specials.removeLast();
            }
            extra_stones.push_back(new stone_base(pixs,0));
            extra_stones.last()->type = stones[i][j]->type;
            extra_stones.last()->setColor(stones[i][j]->color);
            extra_stones.last()->setPos(i*GD::stoneSize,(j-ept_num[i])*GD::stoneSize);
            gpScreen->addItem(extra_stones.last());
            drops.push_back(new dropAniItem(extra_stones.last(),i*GD::stoneSize,j*GD::stoneSize));
            counter++;
        }
    }
    tl1 = new QTimeLine(200*max_empty);
    tl1->setFrameRange(0,GD::stoneSize*max_empty);
    connect(tl1,SIGNAL(frameChanged(int)),this,SLOT(drop_ani_timer(int)));
    connect(tl1,SIGNAL(finished()),this,SLOT(drop_ani_finish()));
    tl1->start();
}
void Game::drop_ani_timer(int frame){
    qreal x,y,m;
    m = frame;
    foreach(dropAniItem *dp,drops){
        x = dp->ox;
        y = dp->oy + m;
        if(y>dp->ty){
            dp->st->setY(dp->ty);
            drops.removeOne(dp);
        }else{
            dp->st->setPos(x,y);
        }

    }

}
void Game::resetStone(){
    int temp;
    for(int i=0;i<GD::stoneCount;i++){
        for(int j=0;j<GD::stoneCount;j++){
            if(stones[i][j]->type>0&&!(stones[i][j]->isSpecialStone)){
                temp = stones[i][j]->color;
                gpScreen->removeItem(stones[i][j]);
                switch(stones[i][j]->type){
                    case 1:
                        delete stones[i][j];
                        stones[i][j] = new stone_v(pixs,temp);
                        break;
                    case 2:
                        delete stones[i][j];
                        stones[i][j] = new stone_h(pixs,temp);
                        break;
                    case 3:
                        delete stones[i][j];
                        stones[i][j] = new stone_boom(pixs,temp);
                        break;
                    case 4:
                        delete stones[i][j];
                        stones[i][j] = new stone_star(pixs,temp);
                        break;
                }
                connect(stones[i][j],SIGNAL(cccc(int,int)),this,SLOT(selectStone(int,int)));
                gpScreen->addItem(stones[i][j]);
            }
            if(stones[i][j]->type==0&&stones[i][j]->isSpecialStone){
                temp = stones[i][j]->color;
                gpScreen->removeItem(stones[i][j]);
                delete stones[i][j];
                stones[i][j] = new stone_base(pixs,temp);
                connect(stones[i][j],SIGNAL(cccc(int,int)),this,SLOT(selectStone(int,int)));
                gpScreen->addItem(stones[i][j]);
            }
            stones[i][j]->row = i;
            stones[i][j]->col =j;
            stones[i][j]->setPos(i*GD::stoneSize,j*GD::stoneSize);
            stones[i][j]->upd();

        }
    }
}
void Game::resetSpecialStone(){
    int temp;
    qreal op;
    for(int i=0;i<GD::stoneCount;i++){
        for(int j=0;j<GD::stoneCount;j++){
            op = stones[i][j]->opacity();

            if(stones[i][j]->type>0&&!(stones[i][j]->isSpecialStone)){
                temp = stones[i][j]->color;

                gpScreen->removeItem(stones[i][j]);
                switch(stones[i][j]->type){
                    case 1:
                        delete stones[i][j];
                        stones[i][j] = new stone_v(pixs,temp);
                        break;
                    case 2:
                        delete stones[i][j];
                        stones[i][j] = new stone_h(pixs,temp);
                        break;
                    case 3:
                        delete stones[i][j];
                        stones[i][j] = new stone_boom(pixs,temp);
                        break;
                    case 4:
                        delete stones[i][j];
                        stones[i][j] = new stone_star(pixs,temp);
                        break;
                }
                connect(stones[i][j],SIGNAL(cccc(int,int)),this,SLOT(selectStone(int,int)));
                gpScreen->addItem(stones[i][j]);

                stones[i][j]->setPos(i*GD::stoneSize,j*GD::stoneSize);

            }
            if(stones[i][j]->type==0&&stones[i][j]->isSpecialStone){
                temp = stones[i][j]->color;
                gpScreen->removeItem(stones[i][j]);
                delete stones[i][j];
                stones[i][j] = new stone_base(pixs,temp);
                connect(stones[i][j],SIGNAL(cccc(int,int)),this,SLOT(selectStone(int,int)));
                gpScreen->addItem(stones[i][j]);


                stones[i][j]->setPos(i*GD::stoneSize,j*GD::stoneSize);
            }
            stones[i][j]->row = i;
            stones[i][j]->col =j;

            stones[i][j]->setOpacity(op);
            stones[i][j]->upd();

        }
    }
}

void Game::drop_ani_finish(){
    disconnect(tl1,SIGNAL(frameChanged(int)),this,SLOT(drop_ani_timer(int)));
    disconnect(tl1,SIGNAL(finished()),this,SLOT(drop_ani_finish()));
    delete tl1;
    resetStone();
    foreach(Stone *st,extra_stones){
        gpScreen->removeItem(st);
    }
    foreach(int i,clearStones){
        stones[i/GD::stoneCount][i%GD::stoneCount]->setOpacity(1.0);
    }
    extra_stones.clear();
    clearStones.clear();
    drops.clear();
    specials.clear();

    checkMove();
    if(bars.length()>0){
        startClearAni();
    }else{
        Combo = -1;
        seled = -1;
    }

}

