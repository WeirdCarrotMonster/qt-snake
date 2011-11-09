#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

//! [0]
class Helper;
QT_BEGIN_NAMESPACE
class QPaintEvent;
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(Helper *helper, QWidget *parent);
    int x;

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Helper *helper;
    int elapsed;
};
//! [0]

#endif
