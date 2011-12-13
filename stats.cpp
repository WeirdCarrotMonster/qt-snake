#include <QFile>
#include "stats.h"

QDataStream &operator<<( QDataStream &stream, const gameResult &info )
{
    stream << info.points <<
              info.multipliers <<
              info.fruits <<
              info.ghosts <<
              info.collectors <<
              info.pills <<
              info.scissors;

    return stream;
}

QDataStream &operator>>( QDataStream &stream, gameResult &info )
{
    stream >> info.points;
    stream >> info.multipliers;
    stream >> info.fruits;
    stream >> info.ghosts;
    stream >> info.collectors;
    stream >> info.pills;
    stream >> info.scissors;
    return stream;
}

inline bool operator<(const gameResult &a1, const gameResult &a2)
{ return a1.points < a2.points; }

stats::stats()
{
    //Грузим gameResultList из файла
    QFile file( "scores.dat" );
    if( !file.open( QIODevice::ReadOnly ) )
      return;

    QDataStream stream( &file );
    stream.setVersion( QDataStream::Qt_4_2 );

    stream >> list;

    file.close();
}

void stats::setStats(gameResult r)
{
    result = r;
    if (result.points == 0)
        return;
    list.append(result);
    qSort(list);

    QFile file( "scores.dat" );
    if( !file.open( QIODevice::WriteOnly ) )
      return;

    QDataStream stream( &file );
    stream.setVersion( QDataStream::Qt_4_2 );

    stream << list;

    file.close();
}

gameResultList stats::getNewStats()
{
    return list;
}
