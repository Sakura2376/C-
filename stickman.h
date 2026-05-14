#pragma once
#include <QGraphicsItem>
#include <QColor>
#include <QString>

class Stickman : public QGraphicsItem
{
public:
    Stickman(QColor color, QString name, bool faceRight);
    QRectF boundingRect() const override;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;

    void moveLeft(bool p);
    void moveRight(bool p);
    void jump();
    void attack();
    void takeDamage(int dmg);
    void knockback(int dir, double force);
    void updatePhysics();

    bool justAttacked = false;
    int hp = 5;
    int kills = 0;
    QString name;
    bool facingRight;
    bool grounded = false;
    double vy = 0;
    bool damageBoost = false;
    int damageBoostTimer = 0;
    bool shieldActive = false;
    int shieldTimer = 0;

private:
    QColor color;
    bool movingLeft = false;
    bool movingRight = false;
    int attackCooldown = 0;
    int hurtTimer = 0;
    int animFrame = 0;
};
