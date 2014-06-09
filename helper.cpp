#include <QtGui>
#include <QTime>
#include "helper.h"
#include <math.h>

#define turn_degree 5
#define default_step_size 10

Helper::Helper(Widget *w, scoreScreen *s)
{
    screen = s;
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    widget = w;
    QResource::registerResource("resource.qrc");

    headImage = QImage(":/images/head.png");
    bonusImage = QImage(":/images/bonus.png");
    pillsImage = QImage(":/images/pills.png");
    scissorsImage = QImage(":/images/scissors.png");
    randomBonusImage = QImage(":/images/random_bonus.png");

    background = QBrush(QColor(64, 32, 64));
    circleBrush = QBrush(QColor(0xa6, 0xce, 0x39));
    circlePen = QPen(Qt::black);
    circlePen.setWidth(1);
    bonusPen = QPen(Qt::red);
    bonusPen.setWidth(3);
    textPen = QPen(Qt::white);
    textFont.setPixelSize(50);
    this->spawnSnake();
    collectorStage = 0;
}

void Helper::spawnSnake()
{
    running = true;
    screen->clear();
    bonusList.clear();
    fruitList.clear();
    animationList.clear();
    fruitDelay = 0;
    pillsHere = 0;
    bonusDelay = 10000;
    x = 300;
    y = 300;
    count = 0;
    for (int i = 1; i< 20; i++)
    {
        body_new[i].radius = 20;
        body_new[i].x = 200;
        body_new[i].y = 400 + 10*i;
        body_new[i].fruit = false;
        count++;
    }
    step = 0;
    body = body_new;
    direction = -90;
    //90 это вниз. до 180 включительно
    this->checkFruit();
}

void Helper::animate(QPainter *painter, QPaintEvent *event)
{
    painter->fillRect(event->rect(), background);
    if (!screen->dead && running)
    {
        this->checkBonus();
        this->checkFruit();
        if (step != 0)
        {
            if (body_new[count].fruit)
            {
                tempTail = true;
                body_new[count + 1].x = body_new[count].x;
                body_new[count + 1].y = body_new[count].y;
                body_new[count + 1].fruit = false;
                body_new[count + 1].radius = 20;
            }
            for (int i = count; i>0; i--)
            {
                body[i].x = (body[i].x + body_new[i].x)/2;
                body[i].y = (body[i].y + body_new[i].y)/2;
            }
            step = 0;
        }
        else
        {
            body = body_new;
            qreal k = tan(direction * M_PI/180);
            qreal c = cos(direction * M_PI/180);
            qreal s = sin(direction * M_PI/180);

            int step_size = default_step_size;
            if (screen->haveBonus("PILLS"))
                step_size += 5*cos(pillsHere * M_PI/180 * 2);
            //Тут щитаем угол между вектором движения и вектором направления
            qreal degree;
            int temp_x, temp_y;
            int new_x, new_y;
            new_x = x - body_new[1].x;
            new_y = y - body_new[1].y;
            temp_x = c*100;
            temp_y = s*100;
            degree = (temp_x*new_x + temp_y*new_y)/(sqrt(pow(temp_x,2) + pow(temp_y,2))*sqrt(pow(new_x,2) + pow(new_y,2)));
            //=================
            int degreeBonus = degree + 2;

            if (direction != 0)
            {

                if ( (y - body_new[1].y) > k*(x - body_new[1].x) )
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
                else if ( (y - body_new[1].y) < k*(x - body_new[1].x) )
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
                if (y > body_new[1].y)
                    direction += turn_degree*degreeBonus;
                else if (y < body_new[1].y)
                    direction -= turn_degree*degreeBonus;
            }
            if (direction > 180)
                direction = -360 + direction;
            else if (direction < -180)
                direction = 360 + direction;

            int diff_x = step_size*cos(direction * M_PI/180);
            int diff_y = step_size*sin(direction * M_PI/180);

            if (this->eatFruit())
                body_new[1].fruit = true;
            this->eatBonus();

            for (int i = count; i>1; i--)
            {
                if (i == count && body_new[i].fruit)
                {
                    count++;
                    body_new[count].x = body_new[i].x;
                    body_new[count].y = body_new[i].y;
                    body_new[count].fruit = false;
                    body_new[count].radius = 20;
                }
                body_new[i].x = body_new[i-1].x;
                body_new[i].y = body_new[i-1].y;
                if (body_new[i-1].fruit)
                    body_new[i].fruit = true;
                else
                    body_new[i].fruit = false;
            }

            body_new[1].fruit = false;
            body_new[1].x += diff_x;
            body_new[1].y += diff_y;

            //Проверка не сдохли ли мы нахуй
            for (int i = count; i > 33; i -= 2)
            {
                if (!screen->haveBonus("GHOST") && (pow((body_new[1].x - body_new[i].x),2) + pow((body_new[1].y - body_new[i].y),2) < 400))
                {
                    screen->dead = true;
                    screen->saveStats();
                    totalGameResult = screen->totalResult();
                }
            }
            //Уебывание в стену
            if (!screen->haveBonus("GHOST") && (body_new[1].x < 10 || body_new[1].x > 590 || body_new[1].y < 10 || body_new[1].y > 590))
            {
                screen->dead = true;
                screen->saveStats();
                totalGameResult = screen->totalResult();
            }
            step = 1;
        }
        screen->pass(20);
    }
    this->draw(painter);
}

bool Helper::eatFruit()
{
    int bonusRange = 0;
    if (screen->haveBonus("COLLECTOR"))
        bonusRange = 60;

    QList<fruit *> flist = fruitList;
    foreach(fruit *a, flist)
    {
        if (pow((body_new[1].x - a->coords.x()),2) + pow((body_new[1].y - a->coords.y()),2) < 400 + pow(bonusRange,2))
        {
            int s = sqrt(pow((300 - a->coords.x()),2) + pow((300 - a->coords.y()),2));
            screen->increaseScore(s);
            animation *b = new animation;
            b->coords = a->coords;
            fruitList.removeAll(a);
            delete a;
            b->value += QString::number(s);
            b->value += QString("x");
            b->value += QString::number(screen->currentMultiplier());
            b->state = 0;
            b->type = "SCORE";
            animationList.append(b);
            this->checkFruit();
            return true;
        }
    }

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
            if (pow((body_new[1].x - a.coords.x()),2) + pow((body_new[1].y - a.coords.y()),2) > 400)
            {
                bonusList.insert(i, a);
                i++;
            }
            else
            {
                if (a.type == "MULTIPLIER")
                    screen->addMultiplier();
                else if (a.type == "SCISSORS")
                {
                    screen->increaseScore(count*30/100 * 50);
                    screen->addBonus("SCISSORS");
                    count = count*70/100;
                }
                else if (a.type == "PILLS")
                {
                    pillsHere += 400;
                    screen->addBonus(a.type);
                }
                else
                {
                    if (!screen->haveBonus(a.type))
                    {
                        if (a.type == "COLLECTOR")
                            collectorStage = 0;
                        animation *b = new animation();
                        b->state = 0;
                        b->type = "MESSAGE";
                        b->value = a.type;
                        animationList.append(b);
                    }
                    screen->addBonus(a.type);
                }
                return true;
            }
        }
    }
    return false;
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

            a.bonusTime -= 20;
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
        int variant = (qrand() % ((10 + 1) - 1) + 1);
        if (variant >= 1 && variant < 7)
            b.type = "MULTIPLIER";
        else if (variant == 7)
            b.type = "SCISSORS";
        else if (variant == 8)
            b.type = "PILLS";
        else if (variant == 9)
            b.type = "COLLECTOR";
        else if (variant == 10)
            b.type = "GHOST";
        b.bonusMaxTime = (qrand() % ((10000 + 1000) - 1000) + 1000);
        b.bonusTime = b.bonusMaxTime;
        b.coords.setX(qrand() % ((590 + 1) - 10) + 10);
        b.coords.setY(qrand() % ((590 + 1) - 10) + 10);
        animation *a = new animation();
        a->coords = b.coords;
        a->state = 0;
        a->type = "SPAWN";
        animationList.append(a);
        bonusList.append(b);
        bonusDelay = ((qrand() % ((50 + 1) - 10) + 10) * 10)*10;
    }
}

void Helper::checkFruit()
{
    fruitDelay -= 30;
    if ( (fruitDelay <= 0 || fruitList.empty()) && fruitList.size() <= 10)
    {
        fruit *b = new fruit();
        animation *a = new animation();
        a->coords = b->coords;
        a->state = 0;
        a->type = "SPAWN";
        animationList.append(a);
        fruitList.append(b);
        fruitDelay = ((qrand() % ((50 + 1) - 10) + 10) * 10)*10;
    }
}

void Helper::draw(QPainter *painter)
{
    painter->save();
    //Фон
    //PILLZ HERE
    if (pillsHere <= 0)
        painter->setBrush(QBrush(QColor(125, 125, 125)));
    else
    {
        painter->setBrush(QBrush(QColor(
                                     50 + 75*(cos(M_PI*(pillsHere%400)/200 - M_PI/2) +1),
                                     50 + 75*(sin(M_PI*(pillsHere%400)/200) +1),
                                     50 + 75*(-sin(M_PI*(pillsHere%400)/200) +1))));
        if (!screen->dead && running) pillsHere--;
    }
    painter->drawRect(0,0,600,600);

    //Тело
    int transparency = 255;
    if (screen->haveBonus("GHOST"))
        transparency = 50;
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
        painter->setBrush(QBrush(QColor(39 + (radius - 20)*10, 135, 63, transparency)));
        painter->drawEllipse(body[i].x - radius/2, body[i].y - radius/2,
                             radius, radius);
    }

    //Голова
    QTransform myTransform;
    myTransform.rotate(direction + 90);
    QImage headTransformed = headImage.transformed(myTransform);
    int w, h;
    w = headTransformed.width();
    h = headTransformed.height();
    headTransformed = headTransformed.copy( (w/2 - 10), (h/2 - 10), 20, 20);
    painter->drawImage(body[1].x - 10, body[1].y - 10, headTransformed);
    if (screen->haveBonus("COLLECTOR"))
    {
        for (int i = 1; i < 5; i++)
        {
            int radius = i * 10 - collectorStage / 4;
            QPen tempPen;
            tempPen = QPen(QColor(255, 255, 0, 240 - radius*6));
            tempPen.setWidth(4);
            painter->setPen(tempPen);
            painter->setBrush(Qt::NoBrush);
            painter->drawEllipse(QPoint(body[1].x, body[1].y), radius, radius);
        }
        collectorStage++;
        collectorStage %= 40;
    }

    //Фрукты
    foreach (fruit *a, fruitList)
        a->draw(painter);

    //Бонусы
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
            if (a.type == "MULTIPLIER")
                painter->drawImage(a.coords.x() - 10, a.coords.y() - 10, bonusImage);
            else if (a.type == "PILLS")
                painter->drawImage(a.coords.x() - 10, a.coords.y() - 10, pillsImage);
            else if (a.type == "SCISSORS")
                painter->drawImage(a.coords.x() - 10, a.coords.y() - 10, scissorsImage);
            else
                painter->drawImage(a.coords.x() - 10, a.coords.y() - 10, randomBonusImage);
            painter->drawArc(a.coords.x()  -10, a.coords.y() - 10,22, 22, 16*startAngle, 16*endAngle);
            bonusList.insert(i, a);
            i++;
        }
    }

    QList<animation *> alist = animationList;
    foreach(animation *a, alist)
    {
        a->draw(painter);
        if (a->state > 60)
        {
            animationList.removeAll(a);
            delete a;
        }
    }

    //Экран статов
    if (screen->dead)
    {
        painter->setPen(Qt::green);
        painter->setBrush(QBrush(QColor(0, 0, 0, 100)));
        painter->drawRect(50, 50, 500, 500);
        painter->setPen(textPen);
        painter->drawText(QRect(100, 100, 100, 20), "Total points:");
        painter->drawText(QRect(200, 100, 100, 20), QString::number(totalGameResult.points));
        painter->drawText(QRect(100, 120, 100, 20), "Multipliers :");
        painter->drawText(QRect(200, 120, 100, 20), QString::number(totalGameResult.multipliers));
        painter->drawText(QRect(100, 140, 100, 20), "Fruits collected:");
        painter->drawText(QRect(200, 140, 100, 20), QString::number(totalGameResult.fruits));
        painter->drawText(QRect(100, 160, 100, 20), "Pills collected:");
        painter->drawText(QRect(200, 160, 100, 20), QString::number(totalGameResult.pills));
        painter->drawText(QRect(100, 180, 100, 20), "Collectors:");
        painter->drawText(QRect(200, 180, 100, 20), QString::number(totalGameResult.collectors));
        painter->drawText(QRect(100, 200, 100, 20), "Ghosts:");
        painter->drawText(QRect(200, 200, 100, 20), QString::number(totalGameResult.ghosts));
        painter->drawText(QRect(100, 220, 100, 20), "Scissors:");
        painter->drawText(QRect(200, 220, 100, 20), QString::number(totalGameResult.scissors));
    }
    painter->restore();
}

void Helper::toggleRunning()
{
    running = !running;
    if (screen->dead)
    {
        spawnSnake();
        screen->clear();
    }
}
