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
    QString value;
    int x;
    int y;

};
typedef struct animation_t animation;

struct bonus_t
{
    QString type;
    QPoint coords;
    int bonusTime, bonusMaxTime;
};
typedef struct bonus_t bonus;

struct fruit_t
{
    QString type;
    QPoint coords;
};
typedef struct fruit_t fruit;

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
    void spawnSnake();
    void checkBonus();
    void checkFruit();
    bool eatFruit();
    bool eatBonus();
    scoreScreen *screen;
    Widget *widget;
    QBrush background;
    QBrush circleBrush;
    QImage fruitImage;
    QImage bonusImage;
    QImage ghostImage;
    QImage collectorImage;
    QImage randomBonusImage;
    QImage headImage;
    QImage pillsImage;
    QFont textFont;
    QPen circlePen;
    QPen textPen;
    QPen bonusPen;
    QPen spawnPen;
    int count, direction, bonusDelay, fruitDelay;
    int additionalSleep;
    int pillsHere;
    bool dead, running, bonusState;
    QList<animation> animationList;
    QList<bonus> bonusList;
    QList<fruit> fruitList;
    cell *head;
    cell body[1000];
};

#endif
