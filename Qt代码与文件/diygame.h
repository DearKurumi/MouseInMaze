#ifndef DIYGAME_H
#define DIYGAME_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>
#include "allpath.h"

namespace Ui {
class DIYGame;
}

class DIYGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit DIYGame(QWidget *parent = nullptr);
    ~DIYGame();
    void paintEvent(QPaintEvent *);  // 重写绘图事件
    void keyPressEvent(QKeyEvent *event);  // 重写键盘事件
    void setMaze(const QVector<QVector<int>>& mazeData, const QVector<QVector<int>>& bfsData, int startX, int startY, int endX, int endY, int TIME);  // 获取上一级界面的数据

private slots:
    void on_returnButton_clicked();

    void on_showButton_clicked();

    void updateCountdown();  // 更新倒计时

    void on_startButton_clicked();

    void on_allButton_clicked();

private:
    Ui::DIYGame *ui;
    QTimer *timer;  // QTimer指针
    QLabel *countDownLabel;  // 倒计时显示标签
    allPath *allpath = nullptr;

    struct Position {  // 位置元素
        int x;  // 横坐标(x)
        int y;  // 纵坐标(y)
        int dis;  // 指示下一个方向
    };

    int len;  // 地图规模
    int size;  // 图片的大小
    int time;  // 倒计时时间
    int countdownTimer;  // 倒计时剩余时间
    bool flag;  // 控制加载图片
    bool gameRunning;  // 游戏是否进行
    bool showBfs;  // 是否显示路径
    Position entrance;  // 入口位置
    Position exit;  // 出口位置
    QPoint mousePos;  // 小鼠当前位置
    QVector<QVector<int>> maze;  // 存储迷宫数据
    QVector<QVector<int>> bfsMaze;  // 存储bfs路径
};

#endif // DIYGAME_H
