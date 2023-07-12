#ifndef RANDOMMODE_H
#define RANDOMMODE_H

#include <QMainWindow>
#include <QVector>
#include <QStack>
#include <QList>
#include <QQueue>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QKeyEvent>
#include <QRandomGenerator>

namespace Ui {
class randomMode;
}

class randomMode : public QMainWindow
{
    Q_OBJECT

public:
    explicit randomMode(QWidget *parent = nullptr);
    ~randomMode();
    void paintEvent(QPaintEvent *);  // 重写绘图事件
    void keyPressEvent(QKeyEvent *event);  // 重写键盘事件
    void bfs();
    void generateMaze();  // 随机生成迷宫

private slots:
    void on_returnButton_clicked();

    void on_startButton_clicked();

    void updateCountdown();  // 更新倒计时

    void on_showButton_clicked();

private:
    Ui::randomMode *ui;
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

    int len;  // 地图规模
    int size;  // 图片的大小
    int countdownTimer;  // 倒计时剩余时间
    bool flag;  // 控制加载图片
    bool gameRunning;  // 游戏是否进行
    bool showBfs;  // 是否显示路径
    Position entrance;  // 入口位置
    Position exit;  // 出口位置
    QPoint mousePos;  // 记录小鼠当前位置
    QVector<QVector<int>> bfsMaze;  // 存储bfs路径
    QVector<QVector<int>> randomMaze;  // 存储随机生成出的迷宫
};

#endif // RANDOMMODE_H
