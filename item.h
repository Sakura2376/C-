#pragma once
#include <QGraphicsItem>

class Stickman;

class Item : public QGraphicsItem
{
public:
    enum Type { Health, DamageUp, Shield };

    Item(double x, double y, Type type);
    QRectF boundingRect() const override;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;

    void applyEffect(Stickman *s);

    int lifetime = 480;

private:
    Type type;
};
