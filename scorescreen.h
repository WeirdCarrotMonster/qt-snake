#ifndef SCORESCREEN_H
#define SCORESCREEN_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>

class scoreScreen : public QWidget
{
    Q_OBJECT
public:
    explicit scoreScreen(QWidget *parent = 0);
    void increaseScore(int s);
    void addAchievement(QString s);
    void addMultiplier();
    void clear();
    void pass(int elapsed);

private:
    QLabel *scoreLabel;
    QLabel *multiplyLabel;
    QLabel *multiplyTimeLabel;
    QListWidget *list;
    int score, multiply;
    int multiplyTime;
};

#endif // SCORESCREEN_H
