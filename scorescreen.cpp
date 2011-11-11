#include <QVBoxLayout>
#include <QHBoxLayout>
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
    if (list->findItems(s, Qt::MatchExactly).empty())
        list->addItem(s);
}

void scoreScreen::clear()
{
    score = 0;
    multiply = 1;
    multiplyTime = 0;
    scoreLabel->setText(QString::number(0));
    multiplyLabel->setText(QString::number(1));
    multiplyTimeLabel->setText(QString::number(multiplyTime/10));
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
    multiplyTimeLabel->setText(QString::number(multiplyTime/10));
}

int scoreScreen::currentMultiplier()
{
    return multiply;
}
