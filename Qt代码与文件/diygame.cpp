#include "diygame.h"
#include "ui_diygame.h"

DIYGame::DIYGame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DIYGame)
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
}

DIYGame::~DIYGame()
{
    delete ui;
}

void DIYGame::paintEvent(QPaintEvent *) {
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
    painter.drawPixmap(595, 130, 150, 150, pix);
    pix.load(":/new/prefix1/pictures/move.png");
    painter.drawPixmap(620, 160, 160, 80, pix);

    if (flag) {
        // 地图绘制
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < len; j++) {
                if (maze[i][j] == 0) {
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

void DIYGame::setMaze(const QVector<QVector<int>>& mazeData, const QVector<QVector<int>>& bfsData, int startX, int startY, int endX, int endY, int TIME)
{
    maze = mazeData;
    bfsMaze = bfsData;
    len = maze.size();
    size = 550 / len;
    entrance = {startX, startY};
    exit = {endX, endY};
    time = TIME;
    maze[entrance.x][entrance.y] = maze[exit.x][exit.y] = 1;
}

void DIYGame::on_startButton_clicked() {
    // 加载地图
    flag = true;

    // 将小鼠移动到入口位置
    mousePos.setX(entrance.x);
    mousePos.setY(entrance.y);

    // 刷新并显示界面
    update();

    // 断开先前的定时器连接
    disconnect(timer, &QTimer::timeout, this, &DIYGame::updateCountdown);

    // 停止先前的定时器
    timer->stop();

    // 创建并启动定时器，每秒更新一次倒计时
    connect(timer, &QTimer::timeout, this, &DIYGame::updateCountdown);
    gameRunning = true;
    timer->start(1000);
    countdownTimer = time;
    countDownLabel->setText(QString::number(countdownTimer));
}

void DIYGame::updateCountdown() {
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

void DIYGame::keyPressEvent(QKeyEvent *event) {
    if (!flag || !gameRunning) return;  // 如果游戏未开始，则不响应键盘事件

    // 获取当前坐标
    int curX = mousePos.x();
    int curY = mousePos.y();

    switch (event->key()) {
    case Qt::Key_W:  // 向上移动
        if (curY > 0 && maze[curX][curY - 1]) {
            mousePos.setY(curY - 1);
        }
        break;
    case Qt::Key_A:  // 向左移动
        if (curX > 0 && maze[curX - 1][curY]) {
            mousePos.setX(curX - 1);
        }
        break;
    case Qt::Key_S:  // 向下移动
        if (curY + 1 < len && maze[curX][curY + 1]) {
            mousePos.setY(curY + 1);
        }
        break;
    case Qt::Key_D:  // 向右移动
        if (curX + 1 < len && maze[curX + 1][curY]) {
            mousePos.setX(curX + 1);
        }
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }

    update();  // 更新小鼠位置
}

void DIYGame::on_showButton_clicked() {
    showBfs = !showBfs;  // 切换路径显示状态
    if (showBfs) {
        ui->showButton->setText("隐藏路径");
    } else {
        ui->showButton->setText("显示路径");
    }
    update();
}

void DIYGame::on_allButton_clicked()
{
    // 进入全部路径界面
    allpath = new allPath;
    allpath->setMaze(maze, bfsMaze, entrance.x, entrance.y, exit.x, exit.y, time);
    this->hide();
    allpath->show();
}

void allPath::on_returnButton_clicked()
{
    // 返回主界面
    DIYGame *diygame = new DIYGame;
    diygame->setMaze(maze, bfsMaze, entrance.x, entrance.y, exit.x, exit.y, time);  // 将迷宫数据从allPath传递给DIYGame
    this->hide();
    diygame->show();
}
