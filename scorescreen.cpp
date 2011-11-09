#include <QVBoxLayout>
#include "scorescreen.h"

scoreScreen::scoreScreen(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    this->setFixedSize(100, 600);
    score = new QLabel("0");
    list = new QListWidget();
    layout->addWidget(score);
    layout->addWidget(list);
    this->setLayout(layout);
}

void scoreScreen::setScore(int s)
{
    score->setText(QString::number(s));
}

void scoreScreen::addAchievement(QString s)
{
    if (list->findItems(s, Qt::MatchExactly).empty())
        list->addItem(s);
}
