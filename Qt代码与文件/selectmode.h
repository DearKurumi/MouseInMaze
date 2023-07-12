#ifndef SELECTMODE_H
#define SELECTMODE_H

#include <QMainWindow>
#include <QVector>
#include <QFile>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QKeyEvent>
#include <QList>
#include <QQueue>

namespace Ui {
class selectMode;
}

class selectMode : public QMainWindow
{
    Q_OBJECT

public:
    explicit selectMode(QWidget *parent = nullptr);
    ~selectMode();
    void loadMaze();  // 载入全部地图
    void initMaze(int index);  // 显示地图
    void paintEvent(QPaintEvent *);  // 重写绘图事件
    void keyPressEvent(QKeyEvent *event);  // 重写键盘事件
    void bfs();  // 广度优先搜索找迷宫路径

private slots:
    void on_returnButton_clicked();

    void on_startButton_clicked();

    void updateCountdown();  // 更新倒计时

    void on_showButton_clicked();

private:
    Ui::selectMode *ui;
    QTimer *timer;  // QTimer指针
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

    int index;  // 第几关
    int len;  // 地图规模
    int size;  // 图片的大小
    int countdownTimer;  // 倒计时剩余时间
    bool flag;  // 控制加载图片
    bool gameRunning;  // 游戏是否进行
    bool showBfs;  // 是否显示路径
    Position entrance;  // 入口位置
    Position exit;  // 出口位置
    QPoint mousePos;  // 小鼠当前位置
    QVector<QVector<int>> bfsMaze;  // 存储bfs路径
    QVector<QVector<QVector<int>>> selectMaze;  // 选关模式全部地图数据
};

#endif // SELECTMODE_H
