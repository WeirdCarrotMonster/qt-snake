#include <QVBoxLayout>
#include <QHBoxLayout>
#include <iostream>
#include "scorescreen.h"

scoreScreen::scoreScreen(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *layout2 = new QHBoxLayout;
    QHBoxLayout *layout3 = new QHBoxLayout;
    QHBoxLayout *layout4 = new QHBoxLayout;
    QLabel *s = new QLabel(trUtf8("Score:"));
    QLabel *m = new QLabel(trUtf8("Multiplier:"));
    QLabel *t = new QLabel(trUtf8("Multiplier time left:"));
    this->setFixedSize(200, 600);
    scoreLabel = new QLabel("0");
    multiplyLabel = new QLabel("1");
    multiplyTimeLabel = new QLabel("0");
    score = 0;
    multiply = 1;
    multiplyTime = 0;
    list = new QListWidget();
    layout2->addWidget(s);
    layout2->addWidget(scoreLabel);
    layout3->addWidget(m);
    layout3->addWidget(multiplyLabel);
    layout4->addWidget(t);
    layout4->addWidget(multiplyTimeLabel);
    layout->addLayout(layout2);
    layout->addLayout(layout3);
    layout->addLayout(layout4);
    layout->addWidget(list);
    this->setLayout(layout);
}

void scoreScreen::increaseScore(int s)
{
    score += s*multiply;
    scoreLabel->setText(QString::number(score));
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
        while (i <= bonusList.count() && !bonusList.empty() && !found)
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
    bool found = false;
    if (!bonusList.empty())
    {
        bonus2 a;
        int i = 1;
        while (i <= bonusList.count() && !bonusList.empty() && !found)
        {
            if (i < bonusList.count())
                a = bonusList.takeAt(i);
            else if (i == bonusList.count())
                a = bonusList.takeLast();

            if (a.name == name)
                found = true;
            bonusList.insert(i, a);
            i++;
        }
    }
    return found;
}

void scoreScreen::clear()
{
    score = 0;
    multiply = 1;
    multiplyTime = 0;
    scoreLabel->setText(QString::number(0));
    multiplyLabel->setText(QString::number(1));
    multiplyTimeLabel->setText(QString::number(multiplyTime/10));
    bonusList.clear();
    list->clear();
}

void scoreScreen::addMultiplier()
{
    multiplyTime = 10000;
    multiply++;
    multiplyLabel->setText(QString::number(multiply));
}

void scoreScreen::pass(int elapsed)
{
    list->clear();
    if (multiplyTime > 0)
    {
        multiplyTime -= elapsed;
        if (multiplyTime <= 0)
        {
            multiply = 1;
            multiplyLabel->setText(QString::number(1));
            multiplyTime = 0;
        }
    }
    multiplyTimeLabel->setText(QString::number(multiplyTime/100));

    if (!bonusList.empty())
    {
        bonus2 a;
        int i = 1;
        while (i <= bonusList.count() && !bonusList.empty())
        {
            if (i < bonusList.count())
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
                list->addItem(s);
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
