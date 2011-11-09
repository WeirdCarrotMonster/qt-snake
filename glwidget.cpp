#include <QtGui>
#include "glwidget.h"
#include "helper.h"

//! [0]
GLWidget::GLWidget(Helper *helper, QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent), helper(helper)
{
    elapsed = 0;
    setFixedSize(200, 200);
    setAutoFillBackground(false);
}
//! [0]

//! [1]
void GLWidget::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    repaint();
}
//! [1]

//! [2]
void GLWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    helper->paint(&painter, event, elapsed);
    painter.end();
}
//! [2]
