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
};

typedef struct cell_t cell;

class Helper
{
public:
    Helper(Widget *w, scoreScreen *s);
    void toggleRunning();
    int x;
    int y;

public:
    void paint(QPainter *painter, QPaintEvent *event, int elapsed);

private:
    void spawnFruit();
    void draw(QPainter *painter);
    void spawnSnake();
    int points;
    scoreScreen *screen;
    Widget *widget;
    QBrush background;
    QBrush circleBrush;
    QImage fruitImage;
    QImage headImage;
    QFont textFont;
    QPen circlePen;
    QPen textPen;
    int count;
    int direction;
    bool dead;
    bool running;
    cell *head;
    cell body[1000];
    QPoint fruit;
};

#endif
