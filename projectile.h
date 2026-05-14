#pragma once
#include <QGraphicsItem>
#include <QColor>

class Projectile : public QGraphicsItem
{
public:
    Projectile(int direction, int dmg, QColor ownerColor);
    QRectF boundingRect() const override;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;

    void updatePosition();
    int getDirection() const { return dir; }
    bool alive = true;
    int damage = 1;
    void *owner = nullptr;

private:
    int dir;
    double speed = 10;
    QColor color;
};
