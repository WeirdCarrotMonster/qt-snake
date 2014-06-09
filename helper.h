#ifndef HELPER_H
#define HELPER_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QCursor>
#include <QResource>
#include <array>
#include "widget.h"
#include "scorescreen.h"
#include "fruit.h"
#include "animation.h"

QT_BEGIN_NAMESPACE
class QPainter;
class QPaintEvent;
QT_END_NAMESPACE

struct cell_t
{
    int x;
    int y;
    int radius;
    bool fruit;
};
typedef struct cell_t cell;

struct bonus_t
{
    QString type;
    QPoint coords;
    int bonusTime, bonusMaxTime;
};
typedef struct bonus_t bonus;

class Helper
{
public:
    Helper(Widget *w, scoreScreen *s);
    void toggleRunning();
    int x;
    int y;

public:
    void animate(QPainter *painter, QPaintEvent *event);

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
    QImage bonusImage;
    QImage scissorsImage;
    QImage randomBonusImage;
    QImage headImage;
    QImage pillsImage;
    QFont textFont;
    QPen circlePen;
    QPen textPen;
    QPen bonusPen;
    QPen spawnPen;
    int count, direction, bonusDelay, fruitDelay;
    int pillsHere;
    bool dead, running, bonusState, tempTail;
    gameResult totalGameResult;
    QList<animation *> animationList;
    QList<bonus> bonusList;
    QList<fruit *> fruitList;
    std::array<cell, 1000> body_new;
    std::array<cell, 1000> body;
    int step;
    int collectorStage;
};

#endif
