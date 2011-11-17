#ifndef STATS_H
#define STATS_H

#include <QWidget>
#include <QList>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

struct gameResult_T
{
    int points;
    int multipliers;
    int fruits;
    int ghosts;
    int collectors;
    int pills;
};

typedef struct gameResult_T gameResult;
typedef QList<gameResult> gameResultList;
typedef QList<int> gameResultListInt;

class stats : public QWidget
{
    Q_OBJECT

public:
    stats();
    ~stats();
    void setStats(gameResult r);
    gameResultListInt getNewStatsInt();

private:
    void fillStats();
    void saveStats();
    gameResult result;
    gameResultList list;
    gameResultListInt listInt;
};

#endif // STATS_H
