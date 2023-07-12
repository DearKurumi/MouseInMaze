#ifndef DIYMODE_H
#define DIYMODE_H

#include <QMainWindow>
#include <QVector>
#include <QList>
#include <QQueue>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include "diygame.h"

namespace Ui {
class DIYMode;
}

class DIYMode : public QMainWindow
{
    Q_OBJECT

public:
    explicit DIYMode(QWidget *parent = nullptr);
    ~DIYMode();
    void paintEvent(QPaintEvent *);  // 重写绘图事件
    void mousePressEvent(QMouseEvent *event);  // 重写鼠标事件
    void updateMazeSize(int size);
    void LupdateMaze(int x, int y);
    void RupdateMaze(int x, int y);
    void bfs();  // 广度优先搜索找迷宫路径

private slots:
    void on_returnButton_clicked();

    void on_startButton_clicked();

    void on_saveButton_clicked();

    void on_readButton_clicked();

    void on_addButton_clicked();

    void on_subButton_clicked();

    void on_showButton_clicked();

    void on_upButton_clicked();

    void on_downButton_clicked();

private:
    Ui::DIYMode *ui;
    DIYGame *diygame = nullptr;
    QLabel *countDownLabel;  // 倒计时显示标签

    struct Position {  // 位置元素
        int x;  // 横坐标(x)
        int y;  // 纵坐标(y)
        int dis;  // 指示下一个方向
    };

    struct node {  // 路径节点
        Position pos;
        QList<Position> l;
    };

    int len;  // 地图规模
    int size;  // 图片的大小
    int time;  // 倒计时时间
    bool showBfs;  // 是否显示路径
    bool haveEntrance;  // 是否已有入口
    bool haveExit;  // 是否已有出口
    Position entrance;  // 入口位置
    Position exit;  // 出口位置
    QQueue<node> q;  // 存储bfs路径
    QVector<QVector<int>> DIYMaze;  // 存储自定义迷宫
    QVector<QVector<int>> bfsMaze;  // 存储bfs路径
};

#endif // DIYMODE_H
