#include "fruit.h"

fruit::fruit(QObject *parent) :
    QObject(parent)
{
    fruitImage[0] = QImage(":/images/fruit.png");
    fruitImage[1] = QImage(":/images/orange.png");
    fruitImage[2] = QImage(":/images/banana.png");

    type = qrand() % 3;
    coords.setX(qrand() % ((590 + 1) - 10) + 10);
    coords.setY(qrand() % ((590 + 1) - 10) + 10);
}

void fruit::draw(QPainter *painter)
{
    painter->drawImage(
        coords.x() - 10,
        coords.y() - 10,
        fruitImage[type]
    );
}
