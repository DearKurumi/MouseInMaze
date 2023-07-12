#include "allpath.h"
#include "ui_allpath.h"

allPath::allPath(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::allPath)
{
    ui->setupUi(this);

    len = 0;
    size = 550;
    index = 0;
}

allPath::~allPath()
{
    delete ui;
}

void allPath::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/new/prefix1/pictures/background.jpg");
    painter.drawPixmap(0, 0, 851, 601, pix);
    painter.translate(70, 20);
    pix.load(":/new/prefix1/pictures/grass.jpg");
    painter.drawPixmap(0, 0, size * len, size * len, pix);

    // 地图绘制
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            if (maze[i][j] == 0) {
                pix.load(":/new/prefix1/pictures/tree.png");
                painter.drawPixmap(i * size, j * size, size, size, pix);
            }
        }
    }
    pix.load(":/new/prefix1/pictures/mouse.png");
    painter.drawPixmap(entrance.x * size, entrance.y * size, size, size, pix);
    pix.load(":/new/prefix1/pictures/cheese.png");
    painter.drawPixmap(exit.x * size, exit.y * size, size, size, pix);

    // 路径绘制
    if (!dfsMaze.isEmpty()) {
        for(int i = 0; i < dfsMaze[index].size(); i++) {
            switch (dfsMaze[index][i].dis) {
            case 1:
                pix.load(":/new/prefix1/pictures/downArrow.png");
                painter.drawPixmap(dfsMaze[index][i].x * size, dfsMaze[index][i].y * size, size, size, pix);
                break;
            case 2:
                pix.load(":/new/prefix1/pictures/rightArrow.png");
                painter.drawPixmap(dfsMaze[index][i].x * size, dfsMaze[index][i].y * size, size, size, pix);
                break;
            case 3:
                pix.load(":/new/prefix1/pictures/upArrow.png");
                painter.drawPixmap(dfsMaze[index][i].x * size, dfsMaze[index][i].y * size, size, size, pix);
                break;
            case 4:
                pix.load(":/new/prefix1/pictures/leftArrow.png");
                painter.drawPixmap(dfsMaze[index][i].x * size, dfsMaze[index][i].y * size, size, size, pix);
                break;
            }
        }
    }
}

void allPath::setMaze(const QVector<QVector<int>>& mazeData, const QVector<QVector<int>>& bfsData, int startX, int startY, int endX, int endY, int TIME)
{
    maze = mazeData;
    bfsMaze = bfsData;
    len = maze.size();
    size = 550 / len;
    this->time = time;
    entrance = {startX, startY};
    exit = {endX, endY};

    visited.resize(len, QVector<int>(len, 0));
    visited[entrance.x][entrance.y] = 1;
    dfs(entrance);
}

void allPath:: dfs(Position pos)
{
    // 成功找到一条通路
    if (pos.x == exit.x && pos.y == exit.y) {
        QVector<Position> ans;
        Position tmp = s.top();
        s.pop();
        while (!s.isEmpty()) {
            ans.append(s.top());  // 存储栈中元素
            s.pop();
        }
        // 此时栈为空，还原数据供下次回溯
        for (int i = ans.size() - 1; i >= 0; i--) {
            s.push(ans[i]);
        }
        s.push(tmp);
        dfsMaze.append(ans);
        visited[pos.x][pos.y] = 0;  // 清除访问标记
        pos.dis = 0;  // 将pos的下一步方向设为未知状态

        return;
    }

    Position next;  // 下一步的方向元素
    // 查找pos的相邻宫格
    // 右方
    if (pos.y < len - 1 && (maze[pos.x][pos.y + 1] == 1 || (pos.x == exit.x && pos.y + 1 == exit.y)) && !visited[pos.x][pos.y + 1]) {
        pos.dis = 1;  // 下一步的方向标记
        visited[pos.x][pos.y + 1] = 1;  // 下一步的访问标记
        next = {pos.x, pos.y + 1, 1};  // 获取下一步的数据
        s.push(next);  // 将下一步的数据入栈

        dfs(next);  // 递归调用dfs函数
        s.pop();  // 递归完毕，退栈
    }

    // 下方
    if (pos.x < len - 1 && (maze[pos.x + 1][pos.y] == 1 || (pos.x + 1 == exit.x && pos.y == exit.y)) && !visited[pos.x + 1][pos.y]) {
        pos.dis = 2;
        visited[pos.x + 1][pos.y] = 1;
        next = {pos.x + 1, pos.y, 2};
        s.push(next);

        dfs(next);
        s.pop();
    }

    // 左方
    if (pos.y > 0 && (maze[pos.x][pos.y - 1] == 1 || (pos.x == exit.x && pos.y - 1 == exit.y)) && !visited[pos.x][pos.y - 1]) {
        pos.dis = 3;
        visited[pos.x][pos.y - 1] = 1;
        next = {pos.x, pos.y - 1, 3};
        s.push(next);

        dfs(next);
        s.pop();
    }

    // 上方
    if (pos.x > 0 && (maze[pos.x - 1][pos.y] == 1 || (pos.x - 1 == exit.x && pos.y == exit.y)) && !visited[pos.x - 1][pos.y]) {
        pos.dis = 4;
        visited[pos.x - 1][pos.y] = 1;
        next = {pos.x - 1, pos.y, 4};
        s.push(next);

        dfs(next);
        s.pop();
    }

    visited[pos.x][pos.y] = 0;  // 清除访问标记
    pos.dis = 0;  // 将pos的下一步方向设为未知状态
    return;
}

void allPath::on_nextButton_clicked()
{
    index = (index + 1) % dfsMaze.size();
    update();
}
