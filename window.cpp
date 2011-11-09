#include <QtGui>
#include "widget.h"
#include "window.h"
#include "QCursor"
#include <QDebug>
#include <QMouseEvent>

Window::Window()
    : QWidget()
{
    scoreScreen *screen = new scoreScreen;
    cursor = new QCursor;
    helper = new Helper(native, screen);
    native = new Widget(helper, this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(native);
    layout->addWidget(screen);
    setLayout(layout);
    native->setMouseTracking(true);
    this->setMouseTracking(true);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), native, SLOT(animate()));
    timer->start(30);
    this->setFixedSize(720, 620);

    QResource::registerResource("resource.qrc");
    this->setWindowTitle(tr("Snake"));
    QIcon icon = QIcon(":/images/logo.ico");
    this->setWindowIcon(icon);
}

void Window::mouseMoveEvent(QMouseEvent *event)
{
    helper->x = native->mapFromGlobal(cursor->pos()).x();
    helper->y = native->mapFromGlobal(cursor->pos()).y();
}
