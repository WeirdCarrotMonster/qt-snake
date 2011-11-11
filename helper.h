#ifndef HELPER_H
#define HELPER_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QCursor>
#include <QResource>
#include "widget.h"
#include "scorescreen.h"

QT_BEGIN_NAMESPACE
class QPainter;
class QPaintEvent;
QT_END_NAMESPACE

struct cell_t
{
    int x;
    int y;
    int radius;
    struct cell_t *next;
    struct cell_t *prev;
    bool fruit;
};
typedef struct cell_t cell;

struct animation_t
{
    QString type;
    int state;
    int value;
    int x;
    int y;

};
typedef struct animation_t animation;

class Helper
{
public:
    Helper(Widget *w, scoreScreen *s);
    void toggleRunning();
    int x;
    int y;

public:
    void animate(QPainter *painter, QPaintEvent *event, int elapsed);

private:
    void draw(QPainter *painter);
    void spawnFruit();
    void spawnSnake();
    void checkBonus();
    bool eatFruit();
    bool eatBonus();
    scoreScreen *screen;
    Widget *widget;
    QBrush background;
    QBrush circleBrush;
    QImage fruitImage;
    QImage bonusImage;
    QImage headImage;
    QFont textFont;
    QPen circlePen;
    QPen textPen;
    QPen bonusPen;
    QPen spawnPen;
    int count, direction, bonusTime, bonusDelay, bonusMaxTime;
    bool dead, running, bonusState;
    QList<animation> animationList;
    cell *head;
    cell body[1000];
    QPoint fruit, bonus;
};

#endif
