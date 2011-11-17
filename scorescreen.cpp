#include <QPainter>
#include "scorescreen.h"

scoreScreen::scoreScreen(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedSize(200, 600);
    this->clear();
}

void scoreScreen::increaseScore(int s)
{
    result.points += s*multiply;
    result.fruits++;
}

void scoreScreen::addBonus(QString name)
{
    bool found = false;
    if (!bonusList.empty())
    {
        bonus2 a;
        int i = 0;
        while (!found && i < bonusList.count() && !bonusList.empty())
        {
            if (i < bonusList.count())
                a = bonusList.takeAt(i);
            else if (i == bonusList.count())
                a = bonusList.takeLast();

            if (a.name == name)
            {
                a.bonusTime = 10000;
                found = true;
            }
            bonusList.insert(i, a);
            i++;
        }
    }
    if (!found)
    {
        bonus2 b;
        b.name = name;
        if (name == "GHOST")
            result.ghosts++;
        else if (name == "COLLECTOR")
            result.collectors++;
        else if (name == "PILLS")
            result.pills++;
        b.bonusTime = 10000;
        bonusList.append(b);
    }
}

bool scoreScreen::haveBonus(QString name)
{
    foreach (const bonus2 &value, bonusList)
    {
        if (value.name == name)
            return true;
    }

    return false;
}

void scoreScreen::clear()
{
    dead = false;
    result.fruits = 0;
    result.collectors = 0;
    result.ghosts = 0;
    result.multipliers = 0;
    result.pills = 0;
    result.points = 0;
    multiply = 1;
    multiplyTime = 0;
    scoreAnimation = 370;
    bonusList.clear();
}

void scoreScreen::addMultiplier()
{
    multiplyTime = 10000;
    multiply++;
    result.multipliers++;
}

void scoreScreen::pass(int elapsed)
{
    if (multiplyTime > 0)
    {
        multiplyTime -= elapsed;
        if (multiplyTime <= 0)
        {
            multiply = 1;
            multiplyTime = 0;
        }
    }

    if (!bonusList.empty())
    {
        bonus2 a;
        int i = 0;
        while (i < bonusList.size() && !bonusList.empty())
        {
            if (i < bonusList.size())
                a = bonusList.takeAt(i);
            else if (i == bonusList.count())
                a = bonusList.takeLast();

            a.bonusTime -= 30;
            if (a.bonusTime > 0)
            {
                QString s;
                s += a.name;
                s += " ";
                s += QString::number(a.bonusTime/100);
                bonusList.insert(i, a);
                i++;
            }

        }
    }
}

int scoreScreen::currentMultiplier()
{
    return multiply;
}

void scoreScreen::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //Вот тут начинаем все рисовать
    painter.setBrush(QBrush(QColor(125, 125, 125)));
    painter.drawRect(0,0,200,600);
    //Отрисовка экрана очков и множителя
    painter.setBrush(QBrush(QColor(0, 100, 0)));
    painter.drawRect(10,10,180,50);
    painter.setPen(QColor(255, 255, 255));
    painter.drawText(QRect(15, 15, 50, 20), QString("Score:"));
    painter.drawText(QRect(60, 15, 120, 20), QString::number(result.points));
    painter.drawText(QRect(15, 30, 60, 40), QString("Multiplier:"));
    painter.setPen(QColor(255, 255, 255));
    painter.drawText(QRect(80, 30, 120, 40), QString::number(multiply));
    painter.setBrush(QBrush(QColor(0, 0, 100)));
    painter.setPen(Qt::NoPen);
    if (multiplyTime > 0)
        painter.drawRect(15,45,170*multiplyTime/10000,10);

    int startx, starty;
    startx = 10;
    starty = 70;

    foreach (const bonus2 &value, bonusList)
    {
        //TODO:заменить на отрисовку изображения с прогрессбаром
        painter.setBrush(QBrush(QColor(0, 100, 0)));
        painter.setPen(QColor(255, 255, 255));
        painter.drawRect(startx,starty,180,50);
        painter.drawText(QRect(startx + 5, starty +5, 170, 30), value.name);
        painter.setBrush(QBrush(QColor(0, 0, 100)));
        painter.setPen(Qt::NoPen);
        painter.drawRect(startx + 5,starty + 35,170*value.bonusTime/10000,10);
        starty += 60;
    }

    if (this->dead)
    {
        //Отрисовка статов
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(QColor(100, 0, 0)));
        painter.drawRect(15, 220, 170, 370);
        painter.setPen(Qt::white);
        painter.drawText(QRect(20, 225, 160, 15), "Top scores");
        int y = 240;
        int number = topList.count();
        if (number > 23)
            number = 23;
        int asd = 1;
        while (number > 0)
        {
            QString s;
            s += QString::number(asd);
            asd++;
            s += QString(") ");
            s += QString::number(topList.at(number - 1).points);
            painter.drawText(QRect(20, y, 160, 15), s);
            number--;
            y+= 15;
        }
        if (scoreAnimation != 0)
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(QColor(125, 125, 125)));
            painter.drawRect(15, 220 + 370 - scoreAnimation, 170, scoreAnimation);
            scoreAnimation -= 5;
        }
    }

    painter.end();
}

void scoreScreen::saveStats()
{
    saver.setStats(result);
    topList = saver.getNewStats();
}
