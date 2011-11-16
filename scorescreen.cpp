#include <QPainter>
#include "scorescreen.h"

scoreScreen::scoreScreen(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedSize(200, 600);
    score = 0;
    multiply = 1;
    multiplyTime = 0;
}

void scoreScreen::increaseScore(int s)
{
    score += s*multiply;
}

void scoreScreen::addAchievement(QString s)
{
    /*
    if (list->findItems(s, Qt::MatchExactly).empty())
        list->addItem(s);
        */
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
    score = 0;
    multiply = 1;
    multiplyTime = 0;
    bonusList.clear();
}

void scoreScreen::addMultiplier()
{
    multiplyTime = 10000;
    multiply++;
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

int scoreScreen::currentScore()
{
    return score;
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
    painter.drawText(QRect(60, 15, 120, 20), QString::number(score));
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

    painter.end();
}
