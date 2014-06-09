#include "animation.h"

animation::animation(QObject *parent) :
    QObject(parent)
{
    textPen = QPen(Qt::white);
    ghostImage = QImage(":/images/ghost.png");
    collectorImage = QImage(":/images/collector.png");
}

void animation::draw(QPainter *painter)
{
    if (this->type == "SCORE")
    {
        //Тут отрисовка надписи
        painter->setPen(textPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawText(this->coords.x() + this->state/6 - 10, this->coords.y() - this->state/3 - 20, this->value);
    }
    else if (this->type == "MESSAGE")
    {
        QImage image;
        if (this->value == "GHOST")
            image = ghostImage;
        else
            image = collectorImage;
        for (int y = 0; y < image.height(); ++y)
        {
            QRgb *row = (QRgb*)image.scanLine(y);
            for (int x = 0; x < image.width(); ++x)
                if (((unsigned char*)&row[x])[3] != 0)
                    ((unsigned char*)&row[x])[3] = 180 - this->state*3;
        }
        painter->drawImage(QRect(300 - (image.width()*(this->state/2+1))/2,
                                 300 - (image.height()*(this->state/2+1))/2,
                                 image.width()*(this->state/2+1),
                                 image.height()*(this->state/2+1)),
                           image);
    }
    else if (this->type == "SPAWN")
    {
        QPen spawnPen;
        //Круг яблока
        int radius = 20 + this->state;
        spawnPen = QPen(QColor(100, 255, 0, 164 - radius*2));
        spawnPen.setWidth(1 + radius/10);
        painter->setPen(spawnPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(this->coords, radius / 2, radius / 2);
    }
    this->state++;
}
