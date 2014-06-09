#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>
#include <QPainter>

class animation : public QObject
{
    Q_OBJECT
public:
    explicit animation(QObject *parent = 0);
    void draw(QPainter *painter);
    QString type;
    QPen textPen;
    int state;
    QImage ghostImage;
    QImage collectorImage;
    QString value;
    QPoint coords;

signals:

public slots:

};

#endif // ANIMATION_H
