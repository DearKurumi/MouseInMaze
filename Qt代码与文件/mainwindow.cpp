#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口标题
    this->setWindowTitle("老鼠走迷宫");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/new/prefix1/pictures/background.jpg");
    painter.drawPixmap(0, 0, 851, 601, pix);
}

void MainWindow::on_exitButton_clicked()
{
    // 退出游戏
    this->close();
}

void MainWindow::on_selectButton_clicked()
{
    // 进入选关界面
    selectGame = new SelectGame;
    this->hide();
    selectGame->show();
}

void SelectGame::on_returnButton_clicked()
{
    // 返回主界面
    MainWindow *mainWindow = new MainWindow;
    this->hide();
    mainWindow->show();
}

void MainWindow::on_randomButton_clicked()
{
    // 进入随机模式
    randommode = new randomMode;
    this->hide();
    randommode->show();
}

void randomMode::on_returnButton_clicked()
{
    // 返回主界面
    MainWindow *mainWindow = new MainWindow;
    this->hide();
    mainWindow->show();
}

void MainWindow::on_DIYButton_clicked()
{
    // 进入自定义模式
    diymode = new DIYMode;
    this->hide();
    diymode->show();
}

void DIYMode::on_returnButton_clicked()
{
    // 返回主界面
    MainWindow *mainWindow = new MainWindow;
    this->hide();
    mainWindow->show();
}
