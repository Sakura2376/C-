#include "projectile.h"
#include <QPainter>

Projectile::Projectile(int direction, int dmg, QColor c)
    : dir(direction), damage(dmg), color(c)
{
}

QRectF Projectile::boundingRect() const
{
    return QRectF(-10, -10, 20, 20);
}

void Projectile::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
    p->setPen(Qt::NoPen);

    QRadialGradient glow(QPointF(0, 0), 10);
    QColor c = color;
    glow.setColorAt(0, Qt::white);
    glow.setColorAt(0.2, c.lighter(180));
    glow.setColorAt(0.5, c);
    glow.setColorAt(1, QColor(c.red(), c.green(), c.blue(), 0));
    p->setBrush(glow);
    p->drawEllipse(-10, -10, 20, 20);
}

void Projectile::updatePosition()
{
    setPos(x() + dir * speed, y());
    if (x() < -40 || x() > 840 || y() < -40 || y() > 640)
        alive = false;
}
