#ifndef SCORESCREEN_H
#define SCORESCREEN_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>

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
    void addAchievement(QString s);
    void addMultiplier();
    void addBonus(QString name);
    void clear();
    void pass(int elapsed);
    bool haveBonus(QString name);
    int currentMultiplier();
    int currentScore();

private:
    QLabel *scoreLabel;
    QLabel *multiplyLabel;
    QLabel *multiplyTimeLabel;
    QList<bonus2> bonusList;
    QListWidget *list;
    int score, multiply;
    int multiplyTime;
};

#endif // SCORESCREEN_H
