#ifndef SELECTGAME_H
#define SELECTGAME_H

#include <QMainWindow>
#include <QPixmap>
#include <QPainter>
#include "selectmode.h"

namespace Ui {
class SelectGame;
}

class SelectGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit SelectGame(QMainWindow *parent = nullptr);
    ~SelectGame();
    void paintEvent(QPaintEvent *);  // 重写绘图事件
    void initGame(int index);

private slots:
    void on_returnButton_clicked();

    void on_maze1_clicked();

    void on_maze16_clicked();

    void on_maze2_clicked();

    void on_maze3_clicked();

    void on_maze4_clicked();

    void on_maze5_clicked();

    void on_maze6_clicked();

    void on_maze7_clicked();

    void on_maze8_clicked();

    void on_maze9_clicked();

    void on_maze10_clicked();

    void on_maze11_clicked();

    void on_maze12_clicked();

    void on_maze13_clicked();

    void on_maze14_clicked();

    void on_maze15_clicked();

    void on_maze17_clicked();

    void on_maze18_clicked();

    void on_maze19_clicked();

    void on_maze20_clicked();

private:
    Ui::SelectGame *ui;
};

#endif // SELECTGAME_H
