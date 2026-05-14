#pragma once
#include <QMainWindow>
#include <QGraphicsView>
#include <QPushButton>
#include "gamescene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private:
    QWidget *menuWidget;
    QWidget *mapWidget;
    QPushButton *startBtn, *exitBtn, *backBtn;
    QGraphicsView *view;
    GameScene *scene;

    void showMapSelect();
    void startGame(int mapIndex);
    void showMenu();
    bool eventFilter(QObject *obj, QEvent *event) override;
};
