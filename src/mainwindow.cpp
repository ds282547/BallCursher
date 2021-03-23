#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(StartClick()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::StartClick()
{

    game=new Game(this);

    this->setCentralWidget(game);
}
