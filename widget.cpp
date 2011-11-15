#include "widget.h"
#include "helper.h"


Widget::Widget(Helper *helper, QWidget *parent)
    : QWidget(parent), helper(helper)
{
    x = 0;
    elapsed = 0;
    setFixedSize(600, 600);
}

void Widget::paintEvent(QPaintEvent *event)
{
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    helper->animate(&painter, event);
    painter.end();
}
