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
    void setScore(int s);
    void addAchievement(QString s);

signals:

public slots:

private:
    QLabel *score;
    QListWidget *list;

};

#endif // SCORESCREEN_H
