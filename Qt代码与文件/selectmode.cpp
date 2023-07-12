#include "selectmode.h"
#include "ui_selectmode.h"

selectMode::selectMode(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::selectMode)
{
    ui->setupUi(this);

    gameRunning = false;
    showBfs = false;

    // 创建倒计时标签
    timer = new QTimer;
    countDownLabel = new QLabel(this);
    countDownLabel->setGeometry(727, 90, 100, 20);  // 设置标签位置和大小
    QFont font("粗体", 20);  // 字体对象
    countDownLabel->setFont(font);

    installEventFilter(this);  // 事件过滤器，获取键盘信号
    loadMaze();  // 加载地图数据
    flag = false;
}

selectMode::~selectMode()
{
    delete ui;
}

void selectMode::loadMaze()
{
    // 加载选关模式全部地图
    for (int k = 1; k <= 20; k++) {
        // 获取地图路径
        QString path = ":/new/prefix1/selectMaze/maze";
        path += QString::number(k);
        path += ".txt";
        QFile file(path);

        // 从文件中读取地图数据
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QVector<QVector<int>> maze;
            while (!file.atEnd()) {
                // 逐行读取
                QByteArray line = file.readLine();
                QString str(line);
                std::string s = str.toStdString();
                QVector<int> v;
                for (unsigned int i = 0; i < s.size(); i++) {
                    v.append(s[i] - '0');
                }
                maze.append(v);
            }
            for(int i = 0; i < maze.size(); i++) {
                for(int j = 0; j < i; j++) {
                    int tmp = maze[i][j];
                    maze[i][j] = maze[j][i];
                    maze[j][i] = tmp;
                }
            }
            selectMaze.append(maze);  // 最后存储到地图库中
            file.close();
        }
    }
}

void selectMode::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/new/prefix1/pictures/background.jpg");
    painter.drawPixmap(0, 0, 851, 601, pix);
    painter.translate(70, 20);
    pix.load(":/new/prefix1/pictures/grass.jpg");
    painter.drawPixmap(0, 0, size * len, size * len, pix);
    pix.load(":/new/prefix1/pictures/alarm.png");
    painter.drawPixmap(620, 30, 100, 100, pix);
    pix.load(":/new/prefix1/pictures/note.png");
    painter.drawPixmap(595, 150, 150, 150, pix);
    pix.load(":/new/prefix1/pictures/move.png");
    painter.drawPixmap(620, 180, 160, 80, pix);

    if (flag) {
        // 地图绘制
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < len; j++) {
                if (selectMaze[index][i][j] == 0) {
                    pix.load(":/new/prefix1/pictures/tree.png");
                    painter.drawPixmap(i * size, j * size, size, size, pix);
                }
            }
        }
        pix.load(":/new/prefix1/pictures/cheese.png");
        painter.drawPixmap(exit.x * size, exit.y * size, size, size, pix);

        // 路径绘制
        if (showBfs) {
            for(int i = 0; i < len; i++) {
                for(int j = 0; j < len; j++) {
                    switch (bfsMaze[i][j]) {
                        case 1:
                            pix.load(":/new/prefix1/pictures/downArrow.png");
                            painter.drawPixmap(i * size, j * size, size, size, pix);
                            break;
                        case 2:
                            pix.load(":/new/prefix1/pictures/rightArrow.png");
                            painter.drawPixmap(i * size, j * size, size, size, pix);
                            break;
                        case 3:
                            pix.load(":/new/prefix1/pictures/upArrow.png");
                            painter.drawPixmap(i * size, j * size, size, size, pix);
                            break;
                        case 4:
                            pix.load(":/new/prefix1/pictures/leftArrow.png");
                            painter.drawPixmap(i * size, j * size, size, size, pix);
                            break;
                    }
                }
            }
        }

        // 键盘操作移动中绘制小鼠图片
        pix.load(":/new/prefix1/pictures/mouse.png");
        painter.drawPixmap(mousePos.x() * size, mousePos.y() * size, size, size, pix);

        // 判断胜利与失败条件并绘制图片
        if (mousePos.x() == exit.x && mousePos.y() == exit.y) {
            // 禁用键盘信号
            gameRunning = false;

            pix.load(":/new/prefix1/pictures/victory.png");
            painter.drawPixmap(100, 80, 400, 400, pix);
        } else if (countdownTimer < 0) {
            // 禁用键盘信号
            gameRunning = false;

            pix.load(":/new/prefix1/pictures/failure.png");
            painter.drawPixmap(100, 80, 400, 400, pix);
        }
    }
}

void selectMode::bfs()
{
    QVector<QVector<int>> visited(len, QVector<int>(len, 0));  // 访问标记数组
    QQueue<node> q;  // 存储bfs路径
    bfsMaze.resize(len, QVector<int>(len, 0));
    node next;
    next.pos = entrance;
    next.l.append(entrance);
    q.enqueue(next);

    while (!q.isEmpty()) {
        node front = q.head();
        Position cur = front.pos;

        // 成功找到通路
        if (cur.x == exit.x && cur.y == exit.y) {
            qDebug() << "查找通路成功！" << Qt::endl;
            // 将路径存储到bfsMaze中
            for(auto &&i : q.head().l) {
                if (i.x == exit.x && i.y == exit.y) {
                    continue;
                }
                bfsMaze[i.x][i.y] = i.dis;
            }
            return;
        }

        // 查找cur的相邻宫格
        // 下方
        if (cur.y + 1 < len && selectMaze[index][cur.x][cur.y + 1] == 1 && !visited[cur.x][cur.y + 1]) {
            visited[cur.x][cur.y + 1] = 1;  // 下一步的访问标记
            Position temp;
            temp.x = cur.x, temp.y = cur.y + 1, temp.dis = 1;  // 获取下一步的位置数据
            next = front;
            next.l.append(temp);
            next.pos = temp;
            q.enqueue(next);  // 将节点入队
        }
        // 右方
        if (cur.x + 1 < len && selectMaze[index][cur.x + 1][cur.y] == 1 && !visited[cur.x + 1][cur.y]) {
            visited[cur.x + 1][cur.y] = 1;
            Position temp;
            temp.x = cur.x + 1, temp.y = cur.y, temp.dis = 2;
            next = front;
            next.l.append(temp);
            next.pos = temp;
            q.enqueue(next);
        }
        // 上方
        if (cur.y > 0 && selectMaze[index][cur.x][cur.y - 1] == 1 && !visited[cur.x][cur.y - 1]) {
            visited[cur.x][cur.y - 1] = 1;
            Position temp;
            temp.x = cur.x, temp.y = cur.y - 1, temp.dis = 3;
            next = front;
            next.l.append(temp);
            next.pos = temp;
            q.enqueue(next);
        }
        // 左方
        if (cur.x > 0 && selectMaze[index][cur.x - 1][cur.y] == 1 && !visited[cur.x - 1][cur.y]) {
            visited[cur.x - 1][cur.y] = 1;
            Position temp;
            temp.x = cur.x - 1, temp.y = cur.y, temp.dis = 4;
            next = front;
            next.l.append(temp);
            next.pos = temp;
            q.enqueue(next);
        }

        // 将已搜索完的节点出队
        q.dequeue();
    }
    qDebug() << "迷宫无解！" << Qt::endl;
}

void selectMode::initMaze(int index)
{
    this->index = index;
    len = selectMaze[index].size();
    size = 550 / len;
    bool haveInitEntrance = false;
    bool haveInitExit = false;

    // 初始化迷宫出入口
    for (int i = 0; i < len; i++) {
        if (haveInitEntrance && haveInitExit) break;
        for (int j = 0; j < len; j++) {
            if (haveInitEntrance && haveInitExit) break;
            if (selectMaze[index][i][j] == 2) {
                entrance.x = i;
                entrance.y = j;
                mousePos.setX(entrance.x);
                mousePos.setY(entrance.y);
                haveInitEntrance = true;
            } else if (selectMaze[index][i][j] == 3) {
                exit.x = i;
                exit.y = j;
                haveInitExit = true;
            }
        }
    }

    // 将出口的值改为1，便于进行bfs
    selectMaze[index][exit.x][exit.y] = 1;

    // 进行bfs搜索迷宫通路
    bfs();
}

void selectMode::updateCountdown()
{
    std::string time = std::to_string(countdownTimer);
    countDownLabel->setText(QString::fromStdString(time));  // 更新倒计时文本
    countdownTimer--;  // 每次调用减少1秒
    if (countdownTimer < 0 || (mousePos.x() == exit.x && mousePos.y() == exit.y)) {
        // 停止定时器
        timer->stop();

        // // 禁用键盘信号并显示图片
        update();
    }
}

void selectMode::on_startButton_clicked()
{
    // 加载地图
    flag = true;

    // 将小鼠移动到入口位置
    mousePos.setX(entrance.x);
    mousePos.setY(entrance.y);

    // 刷新并显示界面
    update();

    // 断开先前的定时器连接
    disconnect(timer, &QTimer::timeout, this, &selectMode::updateCountdown);

    // 停止先前的定时器
    timer->stop();

    // 创建并启动定时器，每秒更新一次倒计时
    connect(timer, &QTimer::timeout, this, &selectMode::updateCountdown);
    gameRunning = true;
    timer->start(1000);
    countdownTimer = selectMaze[index].size() * 2;
    countDownLabel->setText(QString::number(countdownTimer));
}

void selectMode::keyPressEvent(QKeyEvent *event)
{
    if (!flag || !gameRunning) return;  // 如果游戏未开始，则不响应键盘事件

    // 获取当前坐标
    int curX = mousePos.x();
    int curY = mousePos.y();

    switch (event->key()) {
        case Qt::Key_W:  // 向上移动
            if (curY > 0 && selectMaze[index][curX][curY - 1]) {
                mousePos.setY(curY - 1);
            }
            break;
        case Qt::Key_A:  // 向左移动
            if (curX > 0 && selectMaze[index][curX - 1][curY]) {
                mousePos.setX(curX - 1);
            }
            break;
        case Qt::Key_S:  // 向下移动
            if (curY + 1 < len && selectMaze[index][curX][curY + 1]) {
                mousePos.setY(curY + 1);
            }
            break;
        case Qt::Key_D:  // 向右移动
            if (curX + 1 < len && selectMaze[index][curX + 1][curY]) {
                mousePos.setX(curX + 1);
            }
            break;
        default:
            QMainWindow::keyPressEvent(event);
    }

    update();  // 更新小鼠位置
}

void selectMode::on_showButton_clicked()
{
    showBfs = !showBfs;  // 切换路径显示状态
    if (showBfs) {
        ui->showButton->setText("隐藏路径");
    } else {
        ui->showButton->setText("显示路径");
    }
    update();
}
