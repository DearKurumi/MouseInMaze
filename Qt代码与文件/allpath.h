#ifndef ALLPATH_H
#define ALLPATH_H

#include "qstackedwidget.h"
#include <QMainWindow>
#include <QVector>
#include <QStack>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QLabel>

namespace Ui {
class allPath;
}

class allPath : public QMainWindow
{
    Q_OBJECT

public:
    explicit allPath(QWidget *parent = nullptr);
    ~allPath();
    void paintEvent(QPaintEvent *);  // 重写绘图事件
    struct Position {  // 位置元素
        int x;  // 横坐标(x)
        int y;  // 纵坐标(y)
        int dis;  // 指示下一个方向
    };
    void dfs(Position pos);
    void setMaze(const QVector<QVector<int>>& mazeData, const QVector<QVector<int>>& bfsData, int startX, int startY, int endX, int endY, int TIME);  // 获取上一级界面的数据

private slots:
    void on_nextButton_clicked();

    void on_returnButton_clicked();

private:
    Ui::allPath *ui;

    int len;  // 地图规模
    int size;  // 图片的大小
    int index;  // 路径下标
    int time;  // 存储上一级的time
    Position entrance;  // 入口位置
    Position exit;  // 出口位置
    QStack<Position> s;  // 用来进行dfs的栈
    QVector<QVector<int>> maze;  // 存储迷宫数据
    QVector<QVector<int>> visited;  // 访问标记数组
    QVector<QVector<int>> bfsMaze;  // 存储上一级的bfs结果
    QVector<QVector<Position>> dfsMaze;  // 存储dfs结果
};

#endif // ALLPATH_H
