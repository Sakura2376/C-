#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(800, 600);
    setWindowTitle("Stickman Fighter");

    // ── 主菜单 ──
    menuWidget = new QWidget(this);
    setCentralWidget(menuWidget);
    QVBoxLayout *ml = new QVBoxLayout(menuWidget);
    ml->setAlignment(Qt::AlignCenter);

    QLabel *title = new QLabel("Stickman Fighter", menuWidget);
    title->setAlignment(Qt::AlignCenter);
    QFont tf; tf.setPixelSize(36); tf.setBold(true);
    title->setFont(tf);
    title->setStyleSheet("color: white;");

    QLabel *c1 = new QLabel("P1: Arrows + / (attack)", menuWidget);
    c1->setAlignment(Qt::AlignCenter);
    c1->setStyleSheet("color: #4488ff; font-size: 14px;");
    QLabel *c2 = new QLabel("P2: WASD + Space (attack)", menuWidget);
    c2->setAlignment(Qt::AlignCenter);
    c2->setStyleSheet("color: #ff4444; font-size: 14px;");

    startBtn = new QPushButton("Start Game", menuWidget);
    exitBtn  = new QPushButton("Exit", menuWidget);
    startBtn->setFixedSize(200, 50);
    exitBtn->setFixedSize(200, 50);
    startBtn->setStyleSheet("font-size: 18px; font-weight: bold;");
    exitBtn->setStyleSheet("font-size: 16px;");

    ml->addWidget(title);
    ml->addSpacing(10);
    ml->addWidget(c1);
    ml->addWidget(c2);
    ml->addSpacing(30);
    ml->addWidget(startBtn, 0, Qt::AlignCenter);
    ml->addWidget(exitBtn, 0, Qt::AlignCenter);

    connect(startBtn, &QPushButton::clicked, this, &MainWindow::showMapSelect);
    connect(exitBtn,  &QPushButton::clicked, this, &QMainWindow::close);

    menuWidget->setStyleSheet("background-color: #1e1e2d;");

    // ── 选地图 ──
    mapWidget = new QWidget(this);
    QVBoxLayout *ml2 = new QVBoxLayout(mapWidget);
    ml2->setAlignment(Qt::AlignCenter);

    QLabel *mt = new QLabel("Select Map", mapWidget);
    mt->setAlignment(Qt::AlignCenter);
    mt->setFont(tf);
    mt->setStyleSheet("color: white;");

    QPushButton *m0 = new QPushButton("Arena  - Classic duel arena", mapWidget);
    QPushButton *m1 = new QPushButton("Castle - Fortress with walls", mapWidget);
    QPushButton *m2 = new QPushButton("Towers - Twin tower platforms", mapWidget);
    QPushButton *m3 = new QPushButton("Cave   - Underground cavern", mapWidget);
    backBtn = new QPushButton("Back", mapWidget);

    for (auto *b : {m0, m1, m2, m3}) b->setFixedSize(280, 45);
    backBtn->setFixedSize(200, 40);

    ml2->addWidget(mt);
    ml2->addSpacing(20);
    ml2->addWidget(m0, 0, Qt::AlignCenter);
    ml2->addWidget(m1, 0, Qt::AlignCenter);
    ml2->addWidget(m2, 0, Qt::AlignCenter);
    ml2->addWidget(m3, 0, Qt::AlignCenter);
    ml2->addSpacing(20);
    ml2->addWidget(backBtn, 0, Qt::AlignCenter);

    mapWidget->setStyleSheet("background-color: #1e1e2d;");

    connect(m0, &QPushButton::clicked, this, [this]() { startGame(0); });
    connect(m1, &QPushButton::clicked, this, [this]() { startGame(1); });
    connect(m2, &QPushButton::clicked, this, [this]() { startGame(2); });
    connect(m3, &QPushButton::clicked, this, [this]() { startGame(3); });
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showMenu);
}

void MainWindow::showMapSelect()
{
    menuWidget->hide();
    mapWidget->show();
    setCentralWidget(mapWidget);
}

void MainWindow::startGame(int mapIndex)
{
    mapWidget->hide();
    scene = new GameScene(this);
    view = new QGraphicsView(scene, this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view->setCacheMode(QGraphicsView::CacheNone);
    view->setRenderHint(QPainter::Antialiasing, false);
    view->installEventFilter(this);
    setCentralWidget(view);
    view->setFocus();
    scene->startGame(mapIndex);
}

void MainWindow::showMenu()
{
    setCentralWidget(menuWidget);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        auto *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Escape) {
            showMenu();
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
