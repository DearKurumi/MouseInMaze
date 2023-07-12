#include "diymode.h"
#include "ui_diymode.h"

DIYMode::DIYMode(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DIYMode)
{
    ui->setupUi(this);

    haveEntrance = false;
    haveExit = false;
    showBfs = false;

    countDownLabel = new QLabel(this);
    countDownLabel->setGeometry(727, 90, 100, 20);  // 设置标签位置和大小
    QFont font("粗体", 20);  // 字体对象
    countDownLabel->setFont(font);

    len = 15;
    size = 550 / len;
    time = 0;
    DIYMaze.resize(len, QVector<int>(len, 0)); // 将迷宫全部初始化为墙体
}

DIYMode::~DIYMode()
{
    delete ui;
}

void DIYMode::paintEvent(QPaintEvent *)
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
    painter.drawPixmap(595, 110, 150, 150, pix);
    pix.load(":/new/prefix1/pictures/switch.png");
    painter.drawPixmap(620, 140, 180, 90, pix);

    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            if (DIYMaze[i][j] == 0) {
                pix.load(":/new/prefix1/pictures/tree.png");
                painter.drawPixmap(i * size, j * size, size, size, pix);
            }
        }
    }

    if (haveEntrance) {
        pix.load(":/new/prefix1/pictures/mouse.png");
        painter.drawPixmap(entrance.x * size, entrance.y * size, size, size, pix);
    }
    if (haveExit) {
        pix.load(":/new/prefix1/pictures/cheese.png");
        painter.drawPixmap(exit.x * size, exit.y * size, size, size, pix);
    }

    // 路径绘制
    if (showBfs && haveEntrance && haveExit) {
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
}

void DIYMode::on_startButton_clicked()
{
    if (!haveEntrance || !haveExit) {
        QMessageBox::warning(this, "警告", "请设置迷宫出入口！");
        return;
    } else if (q.head().l.empty()) {
        QMessageBox::warning(this, "警告", "请设置有解的迷宫！");
        return;
    }

    diygame = new DIYGame;
    diygame->setMaze(DIYMaze, bfsMaze, entrance.x, entrance.y, exit.x, exit.y, time);  // 将迷宫数据从DIYMode传递给DIYGame

    this->hide();
    diygame->show();
}

void DIYGame::on_returnButton_clicked()
{
    // 返回自定义界面
    DIYMode *diymode = new DIYMode;
    this->hide();
    diymode->show();
}

void DIYMode::on_addButton_clicked()
{
    int newLen = len + 1;
    if (newLen <= 26) {
        len = newLen;
        size = 550 / len;
        haveEntrance = haveExit = false;
        DIYMaze.clear();
        DIYMaze.resize(len, QVector<int>(len, 0));
        q.clear();
        bfsMaze.clear();
        update();
    }
}

void DIYMode::on_subButton_clicked()
{
    int newLen = len - 1;
    if (newLen >= 8) {
        len = newLen;
        size = 550 / len;
        haveEntrance = haveExit = false;
        DIYMaze.clear();
        DIYMaze.resize(len, QVector<int>(len, 0));
        q.clear();
        bfsMaze.clear();
        update();
    }
}

void DIYMode::on_upButton_clicked()
{
    countDownLabel->setText(QString::fromStdString(std::to_string(++time)));
}

void DIYMode::on_downButton_clicked()
{
    if(time > 0) {
        countDownLabel->setText(QString::fromStdString(std::to_string(--time)));
    }
}

void DIYMode::LupdateMaze(int x, int y)
{
    if (DIYMaze[x][y] == 0) {  // 墙变路
        DIYMaze[x][y] = 1;
    } else if (DIYMaze[x][y] == 1) {  // 路变墙
        DIYMaze[x][y] = 0;
    }
    bfs();
    update();
}

void DIYMode::RupdateMaze(int x, int y)
{
    if (DIYMaze[x][y] == 1) {
        if (!haveEntrance) {  // 没有起点，路变起点
            DIYMaze[x][y] = 2;
            haveEntrance = true;
            entrance = {x, y};
        } else if (haveEntrance && !haveExit) {  // 有起点，没有终点，路变终点
            DIYMaze[x][y] = 3;
            haveExit = true;
            exit = {x, y};
        }
    } else if (DIYMaze[x][y] == 2) {
        if(!haveExit) {  // 没有终点，起点变终点
            DIYMaze[x][y] = 3;
            haveEntrance = false;
            haveExit = true;
            exit = {x, y};
        } else {  // 有终点，起点变路
            DIYMaze[x][y] = 1;
            haveEntrance = false;
        }
    } else if (DIYMaze[x][y] == 3) {  // 终点变路
        DIYMaze[x][y] = 1;
        haveExit = false;
    }
    bfs();
    update();
}

void DIYMode::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos() - QPoint(70, 20);
    int x = pos.x() / size;
    int y = pos.y() / size;
    if (x >= 0 && x < len && y >= 0 && y < len) {
        if (event->button() == Qt::LeftButton) {
            LupdateMaze(x, y);
        } else if (event->button() == Qt::RightButton) {
            RupdateMaze(x, y);
        }
    }
}

void DIYMode::bfs() {
    if(!haveEntrance || !haveExit) {
        countDownLabel->setText("");
        time = 0;
        q.clear();
        bfsMaze.clear();
        bfsMaze.resize(len, QVector<int>(len, 0));
        return;
    }
    QVector<QVector<int>> visited(len, QVector<int>(len, 0));  // 访问标记数组
    q.clear();
    bfsMaze.clear();
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
            // 将路径存储到bfsMaze中
            for(auto &&i : q.head().l) {
                if (i.x == exit.x && i.y == exit.y) {
                    continue;
                }
                bfsMaze[i.x][i.y] = i.dis;
            }
            // 展示推荐倒计时时间
            time = q.head().l.size() * 2;
            countDownLabel->setText(QString::fromStdString(std::to_string(time)));

            return;
        }

        // 查找cur的相邻宫格
        // 下方
        if (cur.y + 1 < len && (DIYMaze[cur.x][cur.y + 1] == 1 || (cur.x == exit.x && cur.y + 1 == exit.y)) && !visited[cur.x][cur.y + 1]) {
            visited[cur.x][cur.y + 1] = 1;  // 下一步的访问标记
            Position temp;
            temp.x = cur.x, temp.y = cur.y + 1, temp.dis = 1;  // 获取下一步的位置数据
            next = front;
            next.l.append(temp);
            next.pos = temp;
            q.enqueue(next);  // 将节点入队
        }
        // 右方
        if (cur.x + 1 < len && (DIYMaze[cur.x + 1][cur.y] == 1 || (cur.x + 1 == exit.x && cur.y == exit.y)) && !visited[cur.x + 1][cur.y]) {
            visited[cur.x + 1][cur.y] = 1;
            Position temp;
            temp.x = cur.x + 1, temp.y = cur.y, temp.dis = 2;
            next = front;
            next.l.append(temp);
            next.pos = temp;
            q.enqueue(next);
        }
        // 上方
        if (cur.y > 0 && (DIYMaze[cur.x][cur.y - 1] == 1 || (cur.x == exit.x && cur.y - 1 == exit.y)) && !visited[cur.x][cur.y - 1]) {
            visited[cur.x][cur.y - 1] = 1;
            Position temp;
            temp.x = cur.x, temp.y = cur.y - 1, temp.dis = 3;
            next = front;
            next.l.append(temp);
            next.pos = temp;
            q.enqueue(next);
        }
        // 左方
        if (cur.x > 0 && (DIYMaze[cur.x - 1][cur.y] == 1 || (cur.x - 1 == exit.x && cur.y == exit.y)) && !visited[cur.x - 1][cur.y]) {
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

    countDownLabel->setText("");
    time = 0;
    q.clear();
    bfsMaze.clear();
    bfsMaze.resize(len, QVector<int>(len, 0));
}

void DIYMode::on_showButton_clicked() {
    showBfs = !showBfs;  // 切换路径显示状态
    if (showBfs) {
        ui->showButton->setText("隐藏路径");
    } else {
        ui->showButton->setText("显示路径");
    }
    update();
}

void DIYMode::on_saveButton_clicked()
{
    if (!haveEntrance || !haveExit) {
        QMessageBox::warning(this, "警告", "请设置迷宫出入口！");
        return;
    } else if (q.head().l.empty()) {
        QMessageBox::warning(this, "警告", "请设置有解的迷宫！");
        return;
    }
    QString filePath = QFileDialog::getSaveFileName(this, "保存迷宫", "../MouseInMaze/DIYMaze", "Text Files (*.txt)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            for (int i = 0; i < len; i++) {
                for (int j = 0; j < len; j++) {
                    stream << DIYMaze[i][j];
                }
                stream << Qt::endl;
            }
            file.close();
            QMessageBox::information(this, "保存迷宫", "迷宫保存成功！");
        } else {
            QMessageBox::warning(this, "保存迷宫", "迷宫保存失败！");
        }
    }
}

void DIYMode::on_readButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "读取迷宫", "../MouseInMaze/DIYMaze", "Text Files (*.txt)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QVector<QVector<int>> maze;
            while (!file.atEnd()) {
                QByteArray line = file.readLine();
                QString str(line);
                QVector<int> row;
                for (int i = 0; i < str.size(); i++) {
                    if (str[i].isDigit()) {
                        row.append(str[i].digitValue());
                    }
                }
                if (!row.isEmpty()) {
                    maze.append(row);
                }
            }
            DIYMaze = maze;
            size = 550 / len;
            len = DIYMaze.size();
            file.close();
            QMessageBox::information(this, "读取迷宫", "成功读取迷宫！");
            update();
        } else {
            QMessageBox::warning(this, "读取迷宫", "文件非法，无法读取迷宫！");
        }
    }
}
