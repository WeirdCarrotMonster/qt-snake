#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtGui>

class Helper;
QT_BEGIN_NAMESPACE
class QPaintEvent;
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(Helper *helper, QWidget *parent);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Helper *helper;
    QPainter painter;
};

#endif
