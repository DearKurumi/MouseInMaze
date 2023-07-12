#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include "selectGame.h"
#include "selectmode.h"
#include "randommode.h"
#include "diymode.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *);  // 重写绘图事件

private slots:
    void on_exitButton_clicked();

    void on_selectButton_clicked();

    void on_randomButton_clicked();

    void on_DIYButton_clicked();

private:
    Ui::MainWindow *ui;
    SelectGame *selectGame = nullptr;
    selectMode *selectmode = nullptr;
    randomMode *randommode = nullptr;
    DIYMode *diymode = nullptr;
};
#endif // MAINWINDOW_H
