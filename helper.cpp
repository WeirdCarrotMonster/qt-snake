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
    fruitImage = QImage(":/images/fruit.png");
    headImage = QImage(":/images/head.png");
    bonusImage = QImage(":/images/bonus.png");
    pillsImage = QImage(":/images/pills.png");
    ghostImage = QImage(":/images/ghost.png");
    collectorImage = QImage(":/images/collector.png");
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
    x = 300;
    y = 300;
    count = 0;
    for (int i = 1; i< 20; i++)
    {
        body[i].radius = 20;
        body[i].x = 200;
        body[i].y = 400 + 10*i;
        body[i].fruit = false;
        count++;
    }

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

        if (this->eatFruit())
            body[1].fruit = true;
        this->eatBonus();

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
        body[1].x += diff_x;
        body[1].y += diff_y;

        //Проверка не сдохли ли мы нахуй
        for (int i = count; i > 33; i -= 2)
        {
            if (!screen->haveBonus("GHOST") && (pow((body[1].x - body[i].x),2) + pow((body[1].y - body[i].y),2) < 400))
            {
                screen->dead = true;
                screen->saveStats();
            }
        }
        //Уебывание в стену
        if (!screen->haveBonus("GHOST") && (body[1].x < 10 || body[1].x > 590 || body[1].y < 10 || body[1].y > 590))
        {
            screen->dead = true;
            screen->saveStats();
        }

        screen->pass(40);
    }
    this->draw(painter);
}

bool Helper::eatFruit()
{
    int bonusRange = 0;
    if (screen->haveBonus("COLLECTOR"))
        bonusRange = 60;

    if (!fruitList.empty())
    {
        fruit a;
        int i = 0;
        while (i <= fruitList.count() && !fruitList.empty())
        {
            if (i < fruitList.count())
                a = fruitList.takeAt(i);
            else if (i == fruitList.count())
                a = fruitList.takeLast();
            if (pow((body[1].x - a.coords.x()),2) + pow((body[1].y - a.coords.y()),2) > 400 + pow(bonusRange,2))
            {
                fruitList.insert(i, a);
                i++;
            }
            else
            {
                int s = sqrt(pow((300 - a.coords.x()),2) + pow((300 - a.coords.y()),2));
                screen->increaseScore(s);
                animation b;
                b.x = a.coords.x();
                b.y = a.coords.y();
                b.value += QString::number(s);
                b.value += QString("x");
                b.value += QString::number(screen->currentMultiplier());
                b.state = 0;
                b.type = "SCORE";
                animationList.append(b);
                this->checkFruit();
                return true;
            }
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
            if (pow((body[1].x - a.coords.x()),2) + pow((body[1].y - a.coords.y()),2) > 400)
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
                        animation b;
                        b.state = 0;
                        b.type = "MESSAGE";
                        b.value = a.type;
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
        b.bonusMaxTime = (qrand() % ((100 + 1) - 10) + 10)*10000;
        b.bonusTime = b.bonusMaxTime;
        b.coords.setX(qrand() % ((590 + 1) - 10) + 10);
        b.coords.setY(qrand() % ((590 + 1) - 10) + 10);
        animation a;
        a.x = b.coords.x();
        a.y = b.coords.y();
        a.state = 0;
        a.type = "SPAWN";
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
        fruit b;
        b.type = (qrand() % ((10 + 1) - 1) + 1);
        b.coords.setX(qrand() % ((590 + 1) - 10) + 10);
        b.coords.setY(qrand() % ((590 + 1) - 10) + 10);
        animation a;
        a.x = b.coords.x();
        a.y = b.coords.y();
        a.state = 0;
        a.type = "SPAWN";
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
        QPen tempPen;
        tempPen = QPen(QColor(255, 255, 0, 150));
        tempPen.setWidth(4);
        painter->setPen(tempPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(body[1].x - 39, body[1].y - 39, 78, 78);
    }

    //Фрукты
    if (!fruitList.empty())
    {
        fruit a;
        int i = 0;
        while (i <= fruitList.count() && !fruitList.empty())
        {
            if (i < fruitList.count())
                a = fruitList.takeAt(i);
            else if (i == fruitList.count())
                a = fruitList.takeLast();
            painter->drawImage(a.coords.x() - 10, a.coords.y() - 10, fruitImage);
            fruitList.insert(i, a);
            i++;
        }
    }

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

    //Анимации
    if (!animationList.empty() )
    {
        animation a;
        int i = 0;
        while (i < animationList.count() && !animationList.empty())
        {
            //Вытаскиваем анимацию из списка
            if (i < animationList.count())
                a = animationList.takeAt(i);
            else if (i == animationList.count())
                a = animationList.takeLast();
            if (running && !screen->dead)
                a.state += 2;

            //Разбор типов анимаций
            if (a.type == "SCORE")
            {
                //Тут отрисовка надписи
                painter->setPen(textPen);
                painter->setBrush(Qt::NoBrush);
                painter->drawText(a.x + a.state/6 - 10, a.y - a.state/3 - 20, a.value);
            }
            else if (a.type == "MESSAGE")
            {
                QImage image;
                if (a.value == "GHOST")
                    image = ghostImage;
                else
                    image = collectorImage;
                for (int y = 0; y < image.height(); ++y)
                {
                  QRgb *row = (QRgb*)image.scanLine(y);
                  for (int x = 0; x < image.width(); ++x)
                    if (((unsigned char*)&row[x])[3] != 0)
                        ((unsigned char*)&row[x])[3] = 180 - a.state*3;
                }
                painter->drawImage(QRect(300 - (image.width()*(a.state/2+1))/2,
                                         300 - (image.height()*(a.state/2+1))/2,
                                         image.width()*(a.state/2+1),
                                         image.height()*(a.state/2+1)),
                                   image);
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

            //Кладем анимацию обратно
            if (a.state < 60)
            {
                animationList.insert(i, a);
                i++;
            }
        }
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
