#include <QtGui>
#include <QTime>
#include "helper.h"
#include <math.h>
#include <QDebug>
#include <iostream>

#define turn_degree 5
#define step_size 10

Helper::Helper(Widget *w, scoreScreen *s)
{
    screen = s;
    points = 0;
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    dead = false;
    widget = w;

    QResource::registerResource("resource.qrc");
    fruitImage = QImage(":/images/fruit.png");
    headImage = QImage(":/images/head.png");

    background = QBrush(QColor(64, 32, 64));
    circleBrush = QBrush(QColor(0xa6, 0xce, 0x39));
    circlePen = QPen(Qt::black);
    circlePen.setWidth(1);
    textPen = QPen(Qt::white);
    textFont.setPixelSize(50);
    count = 1;
    x = 200;
    y = 200;

    count = 0;
    for (int i = 1; i< 20; i++)
    {
        body[i].radius = 20;
        body[i].x = 200;
        body[i].y = 400 + body[i].radius/2*i;
        count++;
    }

    direction = -90;
    //90 это вниз. до 180 включительно
    this->spawnFruit();
}

void Helper::paint(QPainter *painter, QPaintEvent *event, int elapsed)
{
    painter->fillRect(event->rect(), background);
    if (!dead)
    {
        painter->save();
        qreal k = tan(direction * M_PI/180);
        qreal c = cos(direction * M_PI/180);
        qreal s = sin(direction * M_PI/180);

        //Тут щитаем угол между вектором движения и вектором направления
        qreal degree;
        int temp_x, temp_y;
        int new_x, new_y;
        new_x = x - body[1].x;
        new_y = y - body[1].y;
        temp_x = c*100;
        temp_y = s*100;
        degree = (temp_x*new_x + temp_y*new_y)/(sqrt(pow(temp_x,2) + pow(temp_y,2))*sqrt(pow(new_x,2) + pow(new_y,2)));
        //=================
        int bonus = degree + 2;

        if (direction != 0)
        {

            if ( (y - body[1].y) > k*(x - body[1].x) )
            {
                if (c > 0 && s > 0)
                    direction += turn_degree*bonus;
                else if (c < 0 && s > 0)
                    direction -= turn_degree*bonus;
                else if (c > 0 && s < 0)
                    direction += turn_degree*bonus;
                else if (c < 0 && s < 0)
                    direction -= turn_degree*bonus;
            }
            else if ( (y - body[1].y) < k*(x - body[1].x) )
            {
                if (c > 0 && s > 0)
                    direction -= turn_degree*bonus;
                else if (c < 0 && s > 0)
                    direction += turn_degree*bonus;
                else if (c > 0 && s < 0)
                    direction -= turn_degree*bonus;
                else if (c < 0 && s < 0)
                    direction += turn_degree*bonus;
            }
        }
        else
        {
            if (y > body[1].y)
                direction += turn_degree*bonus;
            else if (y < body[1].y)
                direction -= turn_degree*bonus;
        }
        if (direction > 180)
            direction = -360 + direction;
        else if (direction < -180)
            direction = 360 + direction;

        int diff_x = step_size*cos(direction * M_PI/180);
        int diff_y = step_size*sin(direction * M_PI/180);

        body[1].x += diff_x;
        body[1].y += diff_y;

        //Не пришло ли время сожрать фрукт?
        if (sqrt(pow((body[1].x - fruit.x()),2) + pow((body[1].y - fruit.y()),2)) < 20)
        {
            //Жрем фрукт, все дела
            body[1].radius += 5;
            points += sqrt(pow((300 - fruit.x()),2) + pow((300 - fruit.y()),2));
            screen->setScore(points);
            //Спавним новый
            this->spawnFruit();
        }

        for (int i = count; i>1; i--)
        {
            if (i == count && body[i].radius > 20)
            {
                count++;
                body[count].x = body[i].x;
                body[count].y = body[i].y;
                body[count].radius = 20;
            }
            body[i].x = body[i-1].x;
            body[i].y = body[i-1].y;
            body[i].radius = body[i-1].radius;
        }

        body[1].radius = 20;
        //Проверка не сдохли ли мы нахуй
        for (int i = count; i > 10; i -= 2)
        {
            if (sqrt(pow((body[1].x - body[i].x),2) + pow((body[1].y - body[i].y),2)) < 20)
            {
                dead = true;
                if (i == count)
                    screen->addAchievement("Uroboros");
            }
        }
        //Уебывание в стену
        if (body[1].x < 10 || body[1].x > 590 || body[1].y < 10 || body[1].y > 590)
            dead = true;

        //ПОЧТИ уебывание в стену, дает ачивку
        if ((body[1].x >= 11 && body[1].x <= 15) ||
            (body[1].x <= 589 && body[1].x >= 584) ||
            (body[1].y <= 11 && body[1].y >= 15) ||
            (body[1].y <= 589 && body[1].y >= 585))
        {
            points += 5;
            screen->setScore(points);
            screen->addAchievement("Obi-Wan");
        }
        this->draw(painter);
        painter->restore();
    }
    else
    {
        painter->save();
        draw(painter);
        painter->restore();
    }
}

void Helper::spawnFruit()
{
    fruit.setX(qrand() % ((590 + 1) - 10) + 10);
    fruit.setY(qrand() % ((590 + 1) - 10) + 10);
}

void Helper::draw(QPainter *painter)
{
    painter->setBrush(circleBrush);
    painter->setPen(circlePen);
    for (int i=count; i> 1; i--)
        painter->drawEllipse(body[i].x - body[i].radius/2, body[i].y - body[i].radius/2,
                             body[i].radius, body[i].radius);

    //и тут внезапно ГОЛОВА такая
    QTransform myTransform;
    myTransform.rotate(direction + 90);
    QImage headTransformed = headImage.transformed(myTransform);
    int w, h;
    w = headTransformed.width();
    h = headTransformed.height();
    headTransformed = headTransformed.copy( (w/2 - 10), (h/2 - 10), 20, 20);
    painter->drawImage(body[1].x - 10, body[1].y - 10, headTransformed);

    painter->drawImage(fruit.x() - 10, fruit.y() - 10, fruitImage);
}
