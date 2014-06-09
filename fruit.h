#ifndef FRUIT_H
#define FRUIT_H

#include <QObject>
#include <QPainter>
#include <QPoint>

class fruit : public QObject
{
    Q_OBJECT
public:
    explicit fruit(QObject *parent = 0);
    void draw(QPainter *painter);
    QPoint coords;

signals:

public slots:

private:
    int type;
    QImage fruitImage[10];
};

#endif // FRUIT_H
