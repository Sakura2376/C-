#include "stickman.h"
#include <QPainter>
#include <QtMath>

Stickman::Stickman(QColor color, QString name, bool faceRight)
    : color(color), name(name), facingRight(faceRight)
{
    setFlag(ItemIsFocusable);
}

QRectF Stickman::boundingRect() const
{
    return QRectF(-38, -65, 76, 95);
}

void Stickman::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (hurtTimer > 0 && hurtTimer % 6 < 3) return;

    int dir = facingRight ? 1 : -1;
    bool attacking = (attackCooldown > 14);
    int atkPhase = 20 - attackCooldown;
    double t = animFrame * 0.35;
    double breathe = qSin(animFrame * 0.04) * 1.5;

    p->save();

    if (hurtTimer > 0) {
        p->translate(0, 0);
        p->rotate(hurtTimer % 4 < 2 ? 4 : -4);
    }

    // ── 阴影 ──
    p->setBrush(QColor(0, 0, 0, 30));
    p->setPen(Qt::NoPen);
    double shadowScale = grounded ? 1.0 : qMax(0.3, 1.0 - qAbs(vy) * 0.05);
    p->drawEllipse(QPointF(0, grounded ? 22 : 17), 8 * shadowScale, 3 * shadowScale);

    // ── 头 ──
    p->setBrush(color);
    p->setPen(QPen(Qt::black, 1.8));
    p->drawEllipse(QPointF(0, -40 + breathe), 10, 10);

    // ── 眼睛 ──
    p->setPen(QPen(Qt::black, 1.5));
    if (hurtTimer > 0) {
        p->drawLine(-6, -44, -2, -38); p->drawLine(-2, -44, -6, -38);
        p->drawLine(2, -44, 6, -38); p->drawLine(6, -44, 2, -38);
    } else if (attacking) {
        p->drawLine(-7, -44, -2, -41); p->drawLine(2, -41, 7, -44);
        p->setBrush(Qt::black);
        p->drawEllipse(QPointF(-3, -41), 1.5, 1.5);
        p->drawEllipse(QPointF(3, -41), 1.5, 1.5);
    } else {
        // 圆眼 + 小高光
        p->setBrush(Qt::black);
        p->drawEllipse(QPointF(-4, -42), 2, 2);
        p->drawEllipse(QPointF(4, -42), 2, 2);
        p->setBrush(Qt::white);
        p->setPen(Qt::NoPen);
        p->drawEllipse(QPointF(-3.5, -42.5), 0.8, 0.8);
        p->drawEllipse(QPointF(4.5, -42.5), 0.8, 0.8);
        p->setPen(QPen(Qt::black, 1.5));
    }

    // ── 身体 ──
    p->setBrush(color);
    p->drawLine(QPointF(0, -30), QPointF(0, 0));

    // ── 手臂 ──
    if (attacking) {
        double extend = qMin(1.0, atkPhase / 4.0);
        p->drawLine(0, -25, dir * (5 + extend * 22), -22);
        p->drawLine(0, -25, -dir * 10, -18);
    } else if (!grounded) {
        double armUp = qBound(-8.0, vy * 1.5, 8.0);
        p->drawLine(0, -25, -12, -38 + armUp);
        p->drawLine(0, -25, 12, -38 + armUp);
    } else if (movingLeft || movingRight) {
        double laX = qSin(t) * 9;
        double laY = -12 + qCos(t) * 7;
        double raX = qSin(t + M_PI) * 9;
        double raY = -12 + qCos(t + M_PI) * 7;
        p->drawLine(0, -25, -12 + laX, laY);
        p->drawLine(0, -25, 12 + raX, raY);
    } else {
        double sway = qSin(animFrame * 0.03) * 2;
        p->drawLine(0, -25, -12 + sway, -12);
        p->drawLine(0, -25, 12 - sway, -12);
    }

    // ── 腿 ──
    if (!grounded) {
        double tuck = qBound(-6.0, vy * 0.8, 6.0);
        p->drawLine(0, 0, -7, 12 + tuck);
        p->drawLine(0, 0, 7, 12 + tuck);
    } else if (movingLeft || movingRight) {
        double llX = qSin(t + M_PI) * 7;
        double llY = 14 + qCos(t) * 8;
        double rlX = qSin(t) * 7;
        double rlY = 14 + qCos(t + M_PI) * 8;
        p->drawLine(0, 0, -8 + llX, llY);
        p->drawLine(0, 0, 8 + rlX, rlY);
    } else {
        double sway = qSin(animFrame * 0.03) * 1;
        p->drawLine(0, 0, -8 + sway, 20);
        p->drawLine(0, 0, 8 - sway, 20);
    }

    // ── 增伤光环 ──
    if (damageBoost) {
        p->setBrush(Qt::NoBrush);
        p->setPen(QPen(QColor(255, 40, 40, 150), 2.5));
        p->drawEllipse(QPointF(0, -25), 24, 32);
    }
    if (shieldActive) {
        p->setBrush(Qt::NoBrush);
        p->setPen(QPen(QColor(60, 160, 255, 180), 2.5));
        p->drawEllipse(QPointF(0, -25), 27, 35);
    }

    p->restore();
}

void Stickman::moveLeft(bool p)  { movingLeft = p; if (p) facingRight = false; }
void Stickman::moveRight(bool p) { movingRight = p; if (p) facingRight = true; }

void Stickman::jump()
{
    if (grounded) { vy = -12; grounded = false; }
}

void Stickman::attack()
{
    if (attackCooldown <= 0) { justAttacked = true; attackCooldown = 20; }
}

void Stickman::takeDamage(int dmg)
{
    if (hurtTimer > 0) return;
    if (shieldActive) { shieldActive = false; shieldTimer = 0; return; }
    hp -= dmg; if (hp < 0) hp = 0;
    hurtTimer = 30;
}

void Stickman::knockback(int dir, double force)
{
    setPos(pos().x() + dir * force, pos().y());
    if (pos().x() < 20)  setPos(20, pos().y());
    if (pos().x() > 780) setPos(780, pos().y());
}

void Stickman::updatePhysics()
{
    if (!grounded) { vy += 0.6; setPos(pos().x(), pos().y() + vy); }
    double dx = 0;
    if (movingLeft)  dx -= 5;
    if (movingRight) dx += 5;
    setPos(pos().x() + dx, pos().y());
    if (pos().x() < 20)  setPos(20, pos().y());
    if (pos().x() > 780) setPos(780, pos().y());
    if (attackCooldown > 0)  attackCooldown--;
    if (hurtTimer > 0)       hurtTimer--;
    if (damageBoostTimer > 0) { damageBoostTimer--; if (damageBoostTimer == 0) damageBoost = false; }
    if (shieldTimer > 0) { shieldTimer--; if (shieldTimer == 0) shieldActive = false; }
    animFrame++;
}
