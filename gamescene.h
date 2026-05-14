#pragma once
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QTimer>
#include <QList>
#include "stickman.h"
#include "projectile.h"
#include "item.h"
#include <QSet>

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);
    void startGame(int mapIndex = 0);

private:
    Stickman *p1, *p2;
    QTimer *timer;
    QList<Projectile *> projectiles;
    QList<Item *> items;
    QList<QGraphicsRectItem *> solids;
    QList<QGraphicsRectItem *> hpBlocks1, hpBlocks2;
    QGraphicsSimpleTextItem *killText1, *killText2;
    QGraphicsRectItem *boostBg1, *boostBar1, *boostBg2, *boostBar2;
    QGraphicsRectItem *shieldBg1, *shieldBar1, *shieldBg2, *shieldBar2;
    QList<QRectF> spawnSurfaces;
    QSet<int> pressedKeys;
    bool gameOver = false;
    int currentMap = 0;

    void loadMap(int index);
    void spawnItem();
    void createProjectile(Stickman *owner);
    void resolveCollisions(Stickman *player);
    void updateHUD();
    void endGame(const QString &winner);

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

private slots:
    void gameLoop();
};
