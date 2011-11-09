#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

#include "helper.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QWidget;
QT_END_NAMESPACE

//! [0]
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
    void asd();

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
};
//! [0]

#endif
