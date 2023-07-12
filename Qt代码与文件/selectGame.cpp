#include "selectGame.h"
#include "ui_selectGame.h"

SelectGame::SelectGame(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::SelectGame)
{
    ui->setupUi(this);

    // 设置窗口标题
    this->setWindowTitle("选择关卡");
}

SelectGame::~SelectGame()
{
    delete ui;
}

void SelectGame::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/new/prefix1/pictures/background.jpg");
    painter.drawPixmap(0, 0, 851, 601, pix);
}

void SelectGame::initGame(int index)
{
    // 进入关卡
    selectMode *selectmode = new selectMode;
    this->hide();
    selectmode->show();
    selectmode->initMaze(index - 1);
}

void selectMode::on_returnButton_clicked()
{
    // 返回选关界面
    SelectGame *selectGame = new SelectGame;
    this->hide();
    selectGame->show();
}

void SelectGame::on_maze1_clicked()
{
    this->initGame(1);
}

void SelectGame::on_maze2_clicked()
{
    this->initGame(2);
}

void SelectGame::on_maze3_clicked()
{
    this->initGame(3);
}

void SelectGame::on_maze4_clicked()
{
    this->initGame(4);
}

void SelectGame::on_maze5_clicked()
{
    this->initGame(5);
}

void SelectGame::on_maze6_clicked()
{
    this->initGame(6);
}

void SelectGame::on_maze7_clicked()
{
    this->initGame(7);
}

void SelectGame::on_maze8_clicked()
{
    this->initGame(8);
}

void SelectGame::on_maze9_clicked()
{
    this->initGame(9);
}

void SelectGame::on_maze10_clicked()
{
    this->initGame(10);
}

void SelectGame::on_maze11_clicked()
{
    this->initGame(11);
}

void SelectGame::on_maze12_clicked()
{
    this->initGame(12);
}

void SelectGame::on_maze13_clicked()
{
    this->initGame(13);
}

void SelectGame::on_maze14_clicked()
{
    this->initGame(14);
}

void SelectGame::on_maze15_clicked()
{
    this->initGame(15);
}

void SelectGame::on_maze16_clicked()
{
    this->initGame(16);
}

void SelectGame::on_maze17_clicked()
{
    this->initGame(17);
}

void SelectGame::on_maze18_clicked()
{
    this->initGame(18);
}

void SelectGame::on_maze19_clicked()
{
    this->initGame(19);
}

void SelectGame::on_maze20_clicked()
{
    this->initGame(20);
}
