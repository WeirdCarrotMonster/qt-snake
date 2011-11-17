#include "stats.h"

stats::stats()
{
    //Грузим gameResultList из файла
}

stats::~stats()
{
    //Записываем gameResultList в файл

}

void stats::setStats(gameResult r)
{
    result = r;
    list.append(result);
}

gameResultListInt stats::getNewStatsInt()
{
    listInt.append(result.points);
    qSort(listInt);
    return listInt;
}
