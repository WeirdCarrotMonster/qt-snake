#ifndef STATS_H
#define STATS_H

#include <QWidget>
#include <QList>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

struct gameResult
{
    int points;
    int multipliers;
    int fruits;
    int ghosts;
    int collectors;
    int pills;
};

typedef QList<gameResult> gameResultList;

class stats : public QWidget
{
    Q_OBJECT

public:
    stats();
    void setStats(gameResult r);
    gameResultList getNewStats();

private:
    void fillStats();
    void saveStats();
    gameResult result;
    gameResultList list;
};

#endif // STATS_H
