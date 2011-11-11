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
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    widget = w;

    QResource::registerResource("resource.qrc");
    fruitImage = QImage(":/images/fruit.png");
    headImage = QImage(":/images/head.png");
    bonusImage = QImage(":/images/bonus.png");

    background = QBrush(QColor(64, 32, 64));
    circleBrush = QBrush(QColor(0xa6, 0xce, 0x39));
    circlePen = QPen(Qt::black);
    circlePen.setWidth(1);
    bonusPen = QPen(Qt::red);
    bonusPen.setWidth(3);
    textPen = QPen(Qt::white);
    textFont.setPixelSize(50);
    this->spawnSnake();
}

void Helper::spawnSnake()
{
    dead = false;
    running = true;
    bonusList.clear();
    x = 200;
    y = 200;
    count = 0;
    for (int i = 1; i< 20; i++)
    {
        body[i].radius = 20;
        body[i].x = 200;
        body[i].y = 400 + body[i].radius/2*i;
        body[i].fruit = false;
        count++;
    }

    direction = -90;
    //90 это вниз. до 180 включительно
    this->spawnFruit();
}

void Helper::animate(QPainter *painter, QPaintEvent *event, int elapsed)
{
    painter->fillRect(event->rect(), background);
    if (!dead && running)
    {
        painter->save();
        this->checkBonus();
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
        int degreeBonus = degree + 2;

        if (direction != 0)
        {

            if ( (y - body[1].y) > k*(x - body[1].x) )
            {
                if (c > 0 && s > 0)
                    direction += turn_degree*degreeBonus;
                else if (c < 0 && s > 0)
                    direction -= turn_degree*degreeBonus;
                else if (c > 0 && s < 0)
                    direction += turn_degree*degreeBonus;
                else if (c < 0 && s < 0)
                    direction -= turn_degree*degreeBonus;
            }
            else if ( (y - body[1].y) < k*(x - body[1].x) )
            {
                if (c > 0 && s > 0)
                    direction -= turn_degree*degreeBonus;
                else if (c < 0 && s > 0)
                    direction += turn_degree*degreeBonus;
                else if (c > 0 && s < 0)
                    direction -= turn_degree*degreeBonus;
                else if (c < 0 && s < 0)
                    direction += turn_degree*degreeBonus;
            }
        }
        else
        {
            if (y > body[1].y)
                direction += turn_degree*degreeBonus;
            else if (y < body[1].y)
                direction -= turn_degree*degreeBonus;
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
        if (this->eatFruit())
        {
            //Жрем фрукт, все дела
            body[1].fruit = true;
        }
        //Теперь бонус
        this->eatBonus();


        //Тут все хуево. Вообще хуево, не надо так делать. Никогда.
        //Вот теперь нормик
        for (int i = count; i>1; i--)
        {
            if (i == count && body[i].fruit)
            {
                count++;
                body[count].x = body[i].x;
                body[count].y = body[i].y;
                body[count].fruit = false;
                body[count].radius = 20;
            }
            body[i].x = body[i-1].x;
            body[i].y = body[i-1].y;
            if (body[i-1].fruit)
                body[i].fruit = true;
            else
                body[i].fruit = false;
        }

        body[1].fruit = false;
        //Проверка не сдохли ли мы нахуй
        for (int i = count; i > 33; i -= 2)
        {
            if (pow((body[1].x - body[i].x),2) + pow((body[1].y - body[i].y),2) < 400)
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
            screen->increaseScore(5);
            screen->addAchievement("Obi-Wan");
        }
        this->draw(painter);
        painter->restore();
        screen->pass(30);
    }
    else
    {
        painter->save();
        draw(painter);
        painter->restore();
    }
}

bool Helper::eatFruit()
{
    if (pow((body[1].x - fruit.x()),2) + pow((body[1].y - fruit.y()),2) < 400)
    {
        int s = sqrt(pow((300 - fruit.x()),2) + pow((300 - fruit.y()),2));
        screen->increaseScore(s);
        animation a;
        a.x = fruit.x();
        a.y = fruit.y();
        a.value = s;
        a.state = 0;
        a.type = "SCORE";
        animationList.append(a);
        this->spawnFruit();
        return true;
    }
    else
        return false;
}

bool Helper::eatBonus()
{
    if (!bonusList.empty())
    {
        bonus a;
        int i = 0;
        while (i <= bonusList.count() && !bonusList.empty())
        {
            if (i < bonusList.count())
                a = bonusList.takeAt(i);
            else if (i == bonusList.count())
                a = bonusList.takeLast();
            if (pow((body[1].x - a.coords.x()),2) + pow((body[1].y - a.coords.y()),2) > 400)
            {
                bonusList.insert(i, a);
                i++;
            }
            else
            {
                screen->addMultiplier();
                return true;
            }
        }
    }
    return false;
}

void Helper::spawnFruit()
{
    fruit.setX(qrand() % ((590 + 1) - 10) + 10);
    fruit.setY(qrand() % ((590 + 1) - 10) + 10);
    animation b;
    b.x = fruit.x();
    b.y = fruit.y();
    b.state = 0;
    b.type = "SPAWN";
    animationList.append(b);
}

void Helper::checkBonus()
{
    if (!bonusList.empty())
    {
        bonus a;
        int i = 0;
        while (i <= bonusList.count() && !bonusList.empty())
        {
            if (i < bonusList.count())
                a = bonusList.takeAt(i);
            else if (i == bonusList.count())
                a = bonusList.takeLast();

            a.bonusTime -= 3000;
            if (a.bonusTime > 0)
            {
                bonusList.insert(i, a);
                i++;
            }
        }
    }
    bonusDelay -= 30;
    if (bonusDelay <= 0)
    {
        bonus b;
        b.type = "MULTIPLIER";
        b.bonusMaxTime = (qrand() % ((100 + 1) - 10) + 10)*10000;
        b.bonusTime = b.bonusMaxTime;
        b.coords.setX(qrand() % ((590 + 1) - 10) + 10);
        b.coords.setY(qrand() % ((590 + 1) - 10) + 10);
        bonusList.append(b);
        bonusDelay = ((qrand() % ((50 + 1) - 10) + 10) * 10)*10;
    }
}

void Helper::draw(QPainter *painter)
{
    painter->setPen(circlePen);
    for (int i=count; i> 1; i--)
    {
        int radius = body[i].radius;
        if (body[i].fruit) radius += 10;
        else
        {
            if (i < count && (body[i-1].fruit || body[i+1].fruit))
                radius += 5;
            else if (i == count && body[i-1].fruit)
                radius += 5;
        }
        painter->setBrush(QBrush(QColor(39 + (radius - 20)*10, 135, 63)));
        painter->drawEllipse(body[i].x - radius/2, body[i].y - radius/2,
                             radius, radius);
    }

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
    //Тут будет порядочная отрисовка бонусов. Когда-нибудь потом
    //А вот и она. Или типо она

    if (!bonusList.empty())
    {
        painter->setPen(bonusPen);
        bonus a;
        int i = 0;
        while (i <= bonusList.count() && !bonusList.empty())
        {
            if (i < bonusList.count())
                a = bonusList.takeAt(i);
            else if (i == bonusList.count())
                a = bonusList.takeLast();
            int startAngle = 90;
            int endAngle = 360*a.bonusTime/a.bonusMaxTime;
            painter->drawImage(a.coords.x() - 10, a.coords.y() - 10, bonusImage);
            painter->drawArc(a.coords.x()  -10, a.coords.y() - 10,22, 22, 16*startAngle, 16*endAngle);

            if (a.bonusTime > 0)
            {
                bonusList.insert(i, a);
                i++;
            }
        }
    }

    //Отрисовка всплывающих сообщений с очками
    if (!animationList.empty() )
    {
        animation a;
        int i = 0;
        while (i <= animationList.count() && !animationList.empty())
        {
            if (i < animationList.count())
                a = animationList.takeAt(i);
            else if (i == animationList.count())
                a = animationList.takeLast();
            //Разбор типов анимаций
            if (running && !dead)
                a.state += 2;
            if (a.type == "SCORE")
            {
                //Тут отрисовка надписи
                painter->setPen(textPen);
                painter->setBrush(Qt::NoBrush);
                painter->drawText(a.x + a.state/6 - 10, a.y - a.state/3 - 20, QString::number(a.value));
            }

            else if (a.type == "SPAWN")
            {
                //Круг яблока
                int radius = 20 + a.state;
                spawnPen = QPen(QColor(100, 255, 0, 164 - radius*2));
                spawnPen.setWidth(1 + radius/10);
                painter->setPen(spawnPen);
                painter->setBrush(Qt::NoBrush);
                painter->drawEllipse(a.x - radius/2, a.y - radius/2, radius, radius);
            }
            if (a.state < 60)
            {
                animationList.insert(i, a);
                i++;
            }
        }
    }
}

void Helper::toggleRunning()
{
    running = !running;
    if (dead)
    {
        spawnSnake();
        screen->clear();
    }
}
