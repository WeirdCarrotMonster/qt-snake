#ifndef SCORESCREEN_H
#define SCORESCREEN_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include "stats.h"

struct bonus_t2
{
    QString name;
    int bonusTime;
};
typedef struct bonus_t2 bonus2;

class scoreScreen : public QWidget
{
    Q_OBJECT
public:
    explicit scoreScreen(QWidget *parent = 0);
    void increaseScore(int s);
    void addMultiplier();
    void addBonus(QString name);
    void clear();
    void pass(int elapsed);
    void saveStats();
    bool haveBonus(QString name);
    bool dead;
    int scoreAnimation;
    int currentMultiplier();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QList<bonus2> bonusList;
    int multiply, multiplyTime;
    gameResult result;
    gameResultListInt topListInt;
    stats saver;
};

#endif // SCORESCREEN_H
