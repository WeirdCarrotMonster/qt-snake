#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

#include "helper.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QWidget;
QT_END_NAMESPACE

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    void setScore(int s);

private:
    Helper *helper;
    Widget *native;
    QCursor *cursor;
    scoreScreen *screen;
    void asd();

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
};

#endif
