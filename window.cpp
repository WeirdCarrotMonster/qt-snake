#include <QtGui>
#include "widget.h"
#include "window.h"
#include "QCursor"
#include <QDebug>
#include <QMouseEvent>

Window::Window()
    : QWidget()
{
    screen = new scoreScreen;
    cursor = new QCursor;
    helper = new Helper(native, screen);
    helper->toggleRunning();
    native = new Widget(helper, this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(native);
    layout->addWidget(screen);
    setLayout(layout);
    native->setMouseTracking(true);
    this->setMouseTracking(true);
    QTimer *timer = new QTimer(this);
    this->setFixedSize(820, 620);
    connect(timer, SIGNAL(timeout()), native, SLOT(repaint()));
    connect(timer, SIGNAL(timeout()), screen, SLOT(repaint()));
    timer->start(40);
}

void Window::mouseMoveEvent(QMouseEvent *event)
{
    helper->x = native->mapFromGlobal(cursor->pos()).x();
    helper->y = native->mapFromGlobal(cursor->pos()).y();
}

void Window::mousePressEvent(QMouseEvent *event)
{
    helper->toggleRunning();
}
