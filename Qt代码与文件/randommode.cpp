#include "randommode.h"
#include "ui_randommode.h"

randomMode::randomMode(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::randomMode)
{
    ui->setupUi(this);

    flag = false;
    gameRunning = false;
    showBfs = false;

    // 创建倒计时标签
    timer = new QTimer;
    countDownLabel = new QLabel(this);
    countDownLabel->setGeometry(727, 90, 100, 20);  // 设置标签位置和大小
    QFont font("粗体", 20);  // 字体对象
    countDownLabel->setFont(font);

    installEventFilter(this);  // 事件过滤器，获取键盘信号
    len = 0;
    size = 550;
}

randomMode::~randomMode()
{
    delete ui;
}

void randomMode::paintEvent(QPaintEvent *)
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
    painter.drawPixmap(620, 30, 100, 100, pix);
    pix.load(":/new/prefix1/pictures/note.png");
    painter.drawPixmap(595, 150, 150, 150, pix);
    pix.load(":/new/prefix1/pictures/move.png");
    painter.drawPixmap(620, 180, 160, 80, pix);

    if (flag) {
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < len; j++) {
                if (randomMaze[i][j] == 0) {
                    pix.load(":/new/prefix1/pictures/tree.png");
                    painter.drawPixmap(i * size, j * size, size, size, pix);
                }
            }
        }
        pix.load(":/new/prefix1/pictures/cheese.png");
        painter.drawPixmap(exit.y * size, exit.x * size, size, size, pix);

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

void randomMode::generateMaze()
{
    len = QRandomGenerator::global()->bounded(6, 14) * 2 + 1;  // 生成随机奇数迷宫规模
    size = 550 / len;
    randomMaze.clear();
    randomMaze.resize(len, QVector<int>(len, 0));  // 将迷宫全部初始化为墙体

    QStack<Position> stack;
    randomMaze[1][1] = 1;
    stack.push({1, 1});

    bool isConnected = false;
    while (!isConnected && !stack.isEmpty()) {
        Position cur = stack.top();

        QVector<Position> neighbors;
        int dx[] = {-2, 2, 0, 0};
        int dy[] = {0, 0, -2, 2};
        for (int k = 0; k < 4; k++) {
            int nx = cur.x + dx[k];
            int ny = cur.y + dy[k];
            if (nx >= 0 && nx < len && ny >= 0 && ny < len && randomMaze[nx][ny] == 0) {
                if (randomMaze[cur.x + dx[k] / 2][cur.y + dy[k] / 2] == 0) {
                    neighbors.append({nx, ny});
                }
            }
        }

        if (neighbors.isEmpty()) {
            stack.pop();
        } else {
            int randIndex = QRandomGenerator::global()->bounded(0, neighbors.size());
            Position neighbor = neighbors[randIndex];

            randomMaze[neighbor.x][neighbor.y] = 1;
            randomMaze[cur.x + (neighbor.x - cur.x) / 2][cur.y + (neighbor.y - cur.y) / 2] = 1;

            stack.push(neighbor);

            // 检查是否全部连通
            isConnected = true;
            for (int i = 1; i < len; i += 2) {
                for (int j = 1; j < len; j += 2) {
                    if (randomMaze[i][j] == 0) {
                        isConnected = false;
                        break;
                    }
                }
                if (!isConnected) {
                    break;
                }
            }
        }
    }

    // 生成随机入口坐标
    do {
        entrance.x = QRandomGenerator::global()->bounded(1, len - 1);
        entrance.y = QRandomGenerator::global()->bounded(1, len - 1);
    } while(randomMaze[entrance.x][entrance.y] == 0);

    // bfs寻找出口并存储路径
    bfs();
}

void randomMode::bfs()
{
    QVector<QVector<int>> visited(len, QVector<int>(len, 0));  // 访问标记数组
    QQueue<node> q;  // 存储bfs路径
    bfsMaze.clear();
    bfsMaze.resize(len, QVector<int>(len, 0));
    node next, curMax;
    next.pos = entrance;
    next.l.append(entrance);
    q.enqueue(next);

    while (!q.isEmpty()) {
        if (q.size() == 1) {
            curMax = q.head();
        }

        node front = q.head();
        Position cur = front.pos;

        // 查找cur的相邻宫格
        // 下方
        if (cur.y + 1 < len && randomMaze[cur.x][cur.y + 1] == 1 && !visited[cur.x][cur.y + 1]) {
            visited[cur.x][cur.y + 1] = 1;  // 下一步的访问标记
            Position terandomMaze;
            terandomMaze.x = cur.x, terandomMaze.y = cur.y + 1, terandomMaze.dis = 1;  // 获取下一步的位置数据
            next = front;
            next.l.append(terandomMaze);
            next.pos = terandomMaze;
            q.enqueue(next);  // 将节点入队
        }
        // 右方
        if (cur.x + 1 < len && randomMaze[cur.x + 1][cur.y] == 1 && !visited[cur.x + 1][cur.y]) {
            visited[cur.x + 1][cur.y] = 1;
            Position terandomMaze;
            terandomMaze.x = cur.x + 1, terandomMaze.y = cur.y, terandomMaze.dis = 2;
            next = front;
            next.l.append(terandomMaze);
            next.pos = terandomMaze;
            q.enqueue(next);
        }
        // 上方
        if (cur.y > 0 && randomMaze[cur.x][cur.y - 1] == 1 && !visited[cur.x][cur.y - 1]) {
            visited[cur.x][cur.y - 1] = 1;
            Position terandomMaze;
            terandomMaze.x = cur.x, terandomMaze.y = cur.y - 1, terandomMaze.dis = 3;
            next = front;
            next.l.append(terandomMaze);
            next.pos = terandomMaze;
            q.enqueue(next);
        }
        // 左方
        if (cur.x > 0 && randomMaze[cur.x - 1][cur.y] == 1 && !visited[cur.x - 1][cur.y]) {
            visited[cur.x - 1][cur.y] = 1;
            Position terandomMaze;
            terandomMaze.x = cur.x - 1, terandomMaze.y = cur.y, terandomMaze.dis = 4;
            next = front;
            next.l.append(terandomMaze);
            next.pos = terandomMaze;
            q.enqueue(next);
        }

        // 将已搜索完的节点出队
        q.dequeue();
    }

    // 找到最大步数的路径
    for (auto &&i : curMax.l) {
        bfsMaze[i.x][i.y] = i.dis;
    }

    exit.x = curMax.pos.y;
    exit.y = curMax.pos.x;
}

void randomMode::updateCountdown()
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

void randomMode::on_startButton_clicked()
{
    generateMaze();
    flag = true;

    // 初始化小鼠到入口位置
    mousePos.setX(entrance.x);
    mousePos.setY(entrance.y);

    update();

    // 断开先前的定时器连接
    disconnect(timer, &QTimer::timeout, this, &randomMode::updateCountdown);

    // 停止先前的定时器
    timer->stop();

    // 创建并启动定时器，每秒更新一次倒计时
    connect(timer, &QTimer::timeout, this, &randomMode::updateCountdown);
    gameRunning = true;
    timer->start(1000);
    countdownTimer = len * 2;
    countDownLabel->setText(QString::number(countdownTimer));
}

void randomMode::keyPressEvent(QKeyEvent *event)
{
    if (!flag || !gameRunning) return;  // 如果游戏未开始，则不响应键盘事件

    // 获取当前坐标
    int currentX = mousePos.x();
    int currentY = mousePos.y();

    switch (event->key()) {
    case Qt::Key_W:  // 向上移动
        if (currentY > 0 && randomMaze[currentX][currentY - 1]) {
            mousePos.setY(currentY - 1);
        }
        break;
    case Qt::Key_A:  // 向左移动
        if (currentX > 0 && randomMaze[currentX - 1][currentY]) {
            mousePos.setX(currentX - 1);
        }
        break;
    case Qt::Key_S:  // 向下移动
        if (currentY + 1 < len && randomMaze[currentX][currentY + 1]) {
            mousePos.setY(currentY + 1);
        }
        break;
    case Qt::Key_D:  // 向右移动
        if (currentX + 1 < len && randomMaze[currentX + 1][currentY]) {
            mousePos.setX(currentX + 1);
        }
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }

    update();  // 更新小鼠位置
}

void randomMode::on_showButton_clicked()
{
    showBfs = !showBfs;  // 切换路径显示状态
    if (showBfs) {
        ui->showButton->setText("隐藏路径");
    } else {
        ui->showButton->setText("显示路径");
    }
    update();
}
