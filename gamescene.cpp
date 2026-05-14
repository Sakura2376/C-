#include "gamescene.h"
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QFont>
#include <QLinearGradient>

GameScene::GameScene(QObject *parent) : QGraphicsScene(parent)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameScene::gameLoop);
}

void GameScene::startGame(int mapIndex)
{
    gameOver = false;
    currentMap = mapIndex;
    projectiles.clear();
    items.clear();
    solids.clear();
    spawnSurfaces.clear();
    hpBlocks1.clear();
    hpBlocks2.clear();
    clear();

    setSceneRect(0, 0, 800, 600);
    setItemIndexMethod(QGraphicsScene::NoIndex);

    // 渐变背景
    QLinearGradient bg(0, 0, 0, 600);
    bg.setColorAt(0, QColor(20, 24, 40));
    bg.setColorAt(0.5, QColor(28, 32, 50));
    bg.setColorAt(1, QColor(18, 20, 35));
    setBackgroundBrush(bg);

    loadMap(mapIndex);

    p1 = new Stickman(QColor(55, 130, 255), "P1", true);
    p1->setPos(150, 550);
    p1->grounded = true;
    addItem(p1);

    p2 = new Stickman(QColor(255, 65, 65), "P2", false);
    p2->setPos(650, 550);
    p2->grounded = true;
    addItem(p2);

    // ── HUD 底板 ──
    auto *hudBg = addRect(0, 0, 800, 48, Qt::NoPen, QColor(0, 0, 0, 100));
    hudBg->setZValue(5);

    QFont f; f.setPixelSize(13); f.setBold(true); f.setFamily("Arial");

    auto *n1 = addSimpleText("P1");
    n1->setPos(20, 14);
    n1->setBrush(QColor(55, 130, 255));
    n1->setFont(f);
    n1->setZValue(6);

    for (int i = 0; i < 5; i++) {
        auto *b = addRect(50 + i * 24, 14, 20, 16, Qt::NoPen, QColor(55, 130, 255));
        hpBlocks1.append(b);
        b->setZValue(6);
    }

    killText1 = addSimpleText("K:0");
    killText1->setPos(180, 14);
    killText1->setBrush(QColor(180, 210, 255));
    QFont kf; kf.setPixelSize(13); kf.setBold(true);
    killText1->setFont(kf);
    killText1->setZValue(6);

    boostBg1 = addRect(50, 34, 120, 6, Qt::NoPen, QColor(60, 60, 60));
    boostBg1->setZValue(5);
    boostBar1 = addRect(50, 34, 0, 6, Qt::NoPen, QColor(255, 140, 40));
    boostBar1->setZValue(6);
    shieldBg1 = addRect(50, 42, 120, 6, Qt::NoPen, QColor(60, 60, 60));
    shieldBg1->setZValue(5);
    shieldBar1 = addRect(50, 42, 0, 6, Qt::NoPen, QColor(60, 160, 255));
    shieldBar1->setZValue(6);

    auto *n2 = addSimpleText("P2");
    n2->setPos(780, 14);
    n2->setBrush(QColor(255, 65, 65));
    n2->setFont(f);
    n2->setZValue(6);
    // right-align P2 name
    QRectF nr = n2->boundingRect();
    n2->setPos(780 - nr.width(), 14);

    for (int i = 0; i < 5; i++) {
        auto *b = addRect(780 - 24 - i * 24 - nr.width() - 8, 14, 20, 16, Qt::NoPen, QColor(255, 65, 65));
        hpBlocks2.append(b);
        b->setZValue(6);
    }

    killText2 = addSimpleText("K:0");
    killText2->setPos(780 - nr.width() - 8 - 5 * 24 - 44, 14);
    killText2->setBrush(QColor(255, 180, 180));
    killText2->setFont(kf);
    killText2->setZValue(6);

    double p2barX = 780 - nr.width() - 8 - 5 * 24 - 4;
    boostBg2 = addRect(p2barX, 34, 120, 6, Qt::NoPen, QColor(60, 60, 60));
    boostBg2->setZValue(5);
    boostBar2 = addRect(p2barX, 34, 0, 6, Qt::NoPen, QColor(255, 140, 40));
    boostBar2->setZValue(6);
    shieldBg2 = addRect(p2barX, 42, 120, 6, Qt::NoPen, QColor(60, 60, 60));
    shieldBg2->setZValue(5);
    shieldBar2 = addRect(p2barX, 42, 0, 6, Qt::NoPen, QColor(60, 160, 255));
    shieldBar2->setZValue(6);

    timer->start(16);
}

void GameScene::loadMap(int index)
{
    QColor gc(60, 48, 35), pc(90, 88, 95), ph(100, 105, 110), bc(75, 60, 42);

    solids.append(addRect(0, 550, 800, 50, Qt::NoPen, gc));
    spawnSurfaces.append(QRectF(20, 540, 760, 14));

    switch (index) {
    case 0: // Arena
        solids.append(addRect(300, 455, 200, 15, Qt::NoPen, pc));
        solids.append(addRect(120, 445, 160, 15, Qt::NoPen, pc));
        solids.append(addRect(520, 445, 160, 15, Qt::NoPen, pc));
        solids.append(addRect(370, 340, 60, 15, Qt::NoPen, ph));
        solids.append(addRect(180, 495, 45, 55, Qt::NoPen, bc));
        solids.append(addRect(575, 495, 45, 55, Qt::NoPen, bc));
        spawnSurfaces.append(QRectF(300, 445, 200, 10));
        spawnSurfaces.append(QRectF(120, 435, 160, 10));
        spawnSurfaces.append(QRectF(520, 435, 160, 10));
        spawnSurfaces.append(QRectF(370, 330, 60, 10));
        break;
    case 1: // Castle
        solids.append(addRect(280, 455, 240, 15, Qt::NoPen, pc));
        solids.append(addRect(60, 445, 160, 15, Qt::NoPen, pc));
        solids.append(addRect(580, 445, 160, 15, Qt::NoPen, pc));
        solids.append(addRect(325, 350, 150, 15, Qt::NoPen, pc));
        solids.append(addRect(75, 340, 120, 15, Qt::NoPen, pc));
        solids.append(addRect(605, 340, 120, 15, Qt::NoPen, pc));
        solids.append(addRect(375, 245, 50, 15, Qt::NoPen, ph));
        solids.append(addRect(155, 495, 45, 55, Qt::NoPen, bc));
        solids.append(addRect(600, 495, 45, 55, Qt::NoPen, bc));
        spawnSurfaces.append(QRectF(280, 445, 240, 10));
        spawnSurfaces.append(QRectF(60, 435, 160, 10));
        spawnSurfaces.append(QRectF(580, 435, 160, 10));
        spawnSurfaces.append(QRectF(325, 340, 150, 10));
        spawnSurfaces.append(QRectF(75, 330, 120, 10));
        spawnSurfaces.append(QRectF(605, 330, 120, 10));
        spawnSurfaces.append(QRectF(375, 235, 50, 10));
        break;
    case 2: // Towers
        solids.append(addRect(290, 450, 220, 15, Qt::NoPen, pc));
        solids.append(addRect(60, 445, 100, 15, Qt::NoPen, pc));
        solids.append(addRect(640, 445, 100, 15, Qt::NoPen, pc));
        solids.append(addRect(335, 345, 130, 15, Qt::NoPen, pc));
        solids.append(addRect(80, 340, 70, 15, Qt::NoPen, pc));
        solids.append(addRect(650, 340, 70, 15, Qt::NoPen, pc));
        solids.append(addRect(365, 245, 60, 15, Qt::NoPen, ph));
        solids.append(addRect(100, 235, 35, 15, Qt::NoPen, ph));
        solids.append(addRect(665, 235, 35, 15, Qt::NoPen, ph));
        solids.append(addRect(145, 495, 45, 55, Qt::NoPen, bc));
        solids.append(addRect(610, 495, 45, 55, Qt::NoPen, bc));
        spawnSurfaces.append(QRectF(290, 440, 220, 10));
        spawnSurfaces.append(QRectF(60, 435, 100, 10));
        spawnSurfaces.append(QRectF(640, 435, 100, 10));
        spawnSurfaces.append(QRectF(335, 335, 130, 10));
        spawnSurfaces.append(QRectF(80, 330, 70, 10));
        spawnSurfaces.append(QRectF(650, 330, 70, 10));
        spawnSurfaces.append(QRectF(365, 235, 60, 10));
        spawnSurfaces.append(QRectF(100, 225, 35, 10));
        spawnSurfaces.append(QRectF(665, 225, 35, 10));
        break;
    case 3: // Cave
        solids.append(addRect(280, 455, 240, 15, Qt::NoPen, pc));
        solids.append(addRect(55, 450, 160, 15, Qt::NoPen, pc));
        solids.append(addRect(585, 450, 160, 15, Qt::NoPen, pc));
        solids.append(addRect(335, 345, 130, 15, Qt::NoPen, pc));
        solids.append(addRect(75, 345, 110, 15, Qt::NoPen, pc));
        solids.append(addRect(615, 345, 110, 15, Qt::NoPen, pc));
        solids.append(addRect(375, 245, 50, 15, Qt::NoPen, ph));
        solids.append(addRect(190, 495, 45, 55, Qt::NoPen, bc));
        solids.append(addRect(565, 495, 45, 55, Qt::NoPen, bc));
        spawnSurfaces.append(QRectF(280, 445, 240, 10));
        spawnSurfaces.append(QRectF(55, 440, 160, 10));
        spawnSurfaces.append(QRectF(585, 440, 160, 10));
        spawnSurfaces.append(QRectF(335, 335, 130, 10));
        spawnSurfaces.append(QRectF(75, 335, 110, 10));
        spawnSurfaces.append(QRectF(615, 335, 110, 10));
        spawnSurfaces.append(QRectF(375, 235, 50, 10));
        break;
    }
}

void GameScene::keyPressEvent(QKeyEvent *e)
{
    if (e->isAutoRepeat()) return;

    if (gameOver) {
        if (e->key() == Qt::Key_R) startGame(currentMap);
        return;
    }
    pressedKeys.insert(e->key());
}

void GameScene::keyReleaseEvent(QKeyEvent *e)
{
    if (e->isAutoRepeat()) return;
    pressedKeys.remove(e->key());
}

void GameScene::gameLoop()
{
    if (gameOver) return;

    // ── 键盘轮询 ──
    p1->moveLeft(pressedKeys.contains(Qt::Key_Left));
    p1->moveRight(pressedKeys.contains(Qt::Key_Right));
    p2->moveLeft(pressedKeys.contains(Qt::Key_A));
    p2->moveRight(pressedKeys.contains(Qt::Key_D));
    if (pressedKeys.contains(Qt::Key_Up))    p1->jump();
    if (pressedKeys.contains(Qt::Key_Slash)) p1->attack();
    if (pressedKeys.contains(Qt::Key_W))     p2->jump();
    if (pressedKeys.contains(Qt::Key_Space)) p2->attack();

    p1->updatePhysics(); p2->updatePhysics();
    resolveCollisions(p1); resolveCollisions(p2);

    if (p1->justAttacked) { p1->justAttacked = false; createProjectile(p1); }
    if (p2->justAttacked) { p2->justAttacked = false; createProjectile(p2); }

    for (int i = projectiles.size() - 1; i >= 0; i--) {
        auto *proj = projectiles[i];
        proj->updatePosition();
        bool hit = false;
        if (proj->owner != p1 && proj->collidesWithItem(p1)) { p1->takeDamage(proj->damage); hit = true; }
        if (proj->owner != p2 && proj->collidesWithItem(p2)) { p2->takeDamage(proj->damage); hit = true; }
        for (auto *s : solids) { if (proj->collidesWithItem(s)) { hit = true; break; } }
        if (!proj->alive || hit) { removeItem(proj); delete proj; projectiles.removeAt(i); }
    }

    if (QRandomGenerator::global()->bounded(150) == 0) spawnItem();

    bool itemChanged = false;
    for (int i = items.size() - 1; i >= 0; i--) {
        auto *it = items[i];
        it->lifetime--;
        if (it->lifetime <= 0) {
            removeItem(it); delete it; items.removeAt(i);
            itemChanged = true;
            continue;
        }
        if (p1->collidesWithItem(it)) {
            it->applyEffect(p1);
            removeItem(it); delete it; items.removeAt(i);
            itemChanged = true;
            continue;
        }
        if (p2->collidesWithItem(it)) {
            it->applyEffect(p2);
            removeItem(it); delete it; items.removeAt(i);
            itemChanged = true;
        }
    }
    if (itemChanged) update();

    updateHUD();
    if (p1->hp <= 0) { p2->kills++; endGame(p2->name + " Wins!"); }
    if (p2->hp <= 0) { p1->kills++; endGame(p1->name + " Wins!"); }
}

void GameScene::createProjectile(Stickman *owner)
{
    int dir = owner->facingRight ? 1 : -1;
    int dmg = owner->damageBoost ? 2 : 1;
    auto *proj = new Projectile(dir, dmg, owner->damageBoost ? QColor(255, 60, 60) : QColor(80, 200, 255));
    proj->owner = owner;
    proj->setPos(owner->pos().x(), owner->pos().y() - 22);
    addItem(proj);
    projectiles.append(proj);
}

void GameScene::resolveCollisions(Stickman *player)
{
    bool onGround = false;
    QRectF pb = player->sceneBoundingRect();

    for (auto *s : solids) {
        QRectF sb = s->sceneBoundingRect();
        if (!pb.intersects(sb)) continue;

        double to = pb.bottom() - sb.top();
        double bo = sb.bottom() - pb.top();
        double lo = pb.right() - sb.left();
        double ro = sb.right() - pb.left();

        double hOverlap = qMin(pb.right(), sb.right()) - qMax(pb.left(), sb.left());
        double ratio = hOverlap / pb.width();
        bool isThin = (sb.height() <= 20);
        double cDiff = pb.center().y() - sb.center().y();
        bool playerAbove = (cDiff < -8);
        bool playerBelow = (cDiff > 8);

        if (playerAbove) {
            if (ratio > 0.15 && to > 0 && to < pb.height() * 0.4) {
                player->setPos(player->pos().x(), player->pos().y() - to);
                player->vy = 0;
                player->grounded = true;
                onGround = true;
            } else if (!isThin && ratio > 0.15) {
                if (lo < ro) player->setPos(player->pos().x() - lo, player->pos().y());
                else         player->setPos(player->pos().x() + ro, player->pos().y());
            }
        } else if (playerBelow) {
            if (isThin && ratio > 0.15 && bo > 0 && bo < pb.height() * 0.4) {
                player->setPos(player->pos().x(), player->pos().y() + bo + 3);
                player->vy = 3;
            } else if (!isThin && ratio > 0.15) {
                if (lo < ro) player->setPos(player->pos().x() - lo, player->pos().y());
                else         player->setPos(player->pos().x() + ro, player->pos().y());
            }
        } else {
            // 同高度 → 横向推出
            if (ratio > 0.15) {
                if (lo < ro) player->setPos(player->pos().x() - lo, player->pos().y());
                else         player->setPos(player->pos().x() + ro, player->pos().y());
            }
        }
        pb = player->sceneBoundingRect();
    }

    if (!onGround && player->pos().y() >= 550) {
        player->setPos(player->pos().x(), 550);
        player->vy = 0;
        player->grounded = true;
    } else if (!onGround && player->pos().y() < 550) {
        player->grounded = false;
    }
}

void GameScene::spawnItem()
{
    if (spawnSurfaces.isEmpty()) return;
    auto rng = QRandomGenerator::global();

    for (int attempt = 0; attempt < 12; attempt++) {
        auto &surf = spawnSurfaces[rng->bounded(spawnSurfaces.size())];
        double x = surf.left() + 10 + rng->bounded(qMax(1.0, surf.width() - 20));
        double y = surf.top() - 15;

        QRectF ir(x - 12, y - 12, 24, 24);
        bool blocked = false;
        for (auto *s : solids) {
            if (ir.intersects(s->sceneBoundingRect())) { blocked = true; break; }
        }
        if (blocked) continue;

        int t = rng->bounded(3);
        auto *it = new Item(x, y, t == 0 ? Item::Health : t == 1 ? Item::DamageUp : Item::Shield);
        addItem(it);
        items.append(it);
        return;
    }
}

void GameScene::updateHUD()
{
    for (int i = 0; i < 5; i++) {
        hpBlocks1[i]->setVisible(i < p1->hp);
        hpBlocks2[i]->setVisible(i < p2->hp);
    }
    killText1->setText("K:" + QString::number(p1->kills));
    killText2->setText("K:" + QString::number(p2->kills));

    double p2barX = boostBg2->rect().x();
    boostBar1->setRect(50, 34, p1->damageBoost ? 120.0 * p1->damageBoostTimer / 300 : 0, 6);
    boostBar2->setRect(p2barX, 34, p2->damageBoost ? 120.0 * p2->damageBoostTimer / 300 : 0, 6);
    shieldBar1->setRect(50, 42, p1->shieldActive ? 120.0 * p1->shieldTimer / 300 : 0, 6);
    shieldBar2->setRect(p2barX, 42, p2->shieldActive ? 120.0 * p2->shieldTimer / 300 : 0, 6);
}

void GameScene::endGame(const QString &winner)
{
    gameOver = true;
    timer->stop();
    auto *bg = addRect(180, 190, 440, 220, Qt::NoPen, QColor(0, 0, 0, 210));
    bg->setZValue(10);

    auto *wt = addSimpleText(winner);
    wt->setPos(300, 210);
    QFont f; f.setPixelSize(40); f.setBold(true);
    wt->setFont(f);
    wt->setBrush(Qt::white);
    wt->setZValue(11);

    auto *hint = addSimpleText("[ R ] Restart      [ Esc ] Exit");
    hint->setPos(260, 300);
    QFont f2; f2.setPixelSize(16);
    hint->setFont(f2);
    hint->setBrush(QColor(160, 160, 160));
    hint->setZValue(11);
}
