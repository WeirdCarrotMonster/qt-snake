#include <QtGui>
#include "widget.h"
#include "window.h"
#include "QCursor"
#include <QDebug>
#include <QMouseEvent>
#include <QInputDialog>
#include <QStringList>

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

    QStringList level;
    level << "easy" << "normal" << "hard" << "elite" << "japanese";

    QString title("Select level");
    QString query("levels:");
    QString pick = QInputDialog::getItem(0, title, query, level);

    int delay;
    if (pick == "easy")
        delay = 40;
    else if (pick == "normal")
        delay = 30;
    else if (pick == "hard")
        delay = 25;
    else if (pick == "elite")
        delay = 20;
    else if (pick == "japanese")
        delay = 0;
    else
        delay = 25;

    connect(timer, SIGNAL(timeout()), native, SLOT(animate()));
    timer->start(delay);
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
