#include "item.h"
#include "stickman.h"
#include <QPainter>

Item::Item(double x, double y, Type type) : type(type)
{
    setPos(x, y);
}

QRectF Item::boundingRect() const
{
    return QRectF(-12, -12, 24, 24);
}

void Item::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (lifetime < 120 && lifetime % 20 < 10) return;

    p->setPen(QPen(Qt::black, 1.5));
    if (type == Health) {
        p->setBrush(QColor(30, 200, 30));
        p->drawRoundedRect(-10, -10, 20, 20, 4, 4);
        p->setPen(QPen(Qt::white, 3));
        p->drawLine(0, -5, 0, 5);
        p->drawLine(-5, 0, 5, 0);
    } else if (type == DamageUp) {
        p->setBrush(QColor(220, 30, 30));
        p->drawRoundedRect(-10, -10, 20, 20, 4, 4);
        p->setPen(QPen(QColor(255, 180, 0), 3));
        p->drawLine(0, -8, 0, 4);
        p->drawLine(-6, -2, 6, -2);
        p->drawLine(-4, -5, 4, -5);
    } else {
        p->setBrush(QColor(40, 130, 230));
        p->drawRoundedRect(-10, -10, 20, 20, 4, 4);
        p->setPen(QPen(Qt::white, 3));
        p->drawEllipse(QPointF(0, -2), 4, 3);
        p->drawLine(-3, -2, 3, -2);
    }
}

void Item::applyEffect(Stickman *s)
{
    if (type == Health) {
        s->hp = qMin(s->hp + 1, 5);
    } else if (type == DamageUp) {
        s->damageBoost = true;
        s->damageBoostTimer = 300;
    } else {
        s->shieldActive = true;
        s->shieldTimer = 300;
    }
}
