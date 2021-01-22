#include "minimapwidget.h"
#include <QPaintEvent>
#include <QPainter>
#include "global.h"


MinimapWidget::MinimapWidget(QWidget *parent) : QWidget(parent)
{
}

void MinimapWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QBrush brush(Qt::white);

    QColor color(255,255,255,200);
    brush.setColor(color);

    painter.setPen(Qt::gray);
    painter.setBrush(brush);
    painter.drawRect(this->rect());

    scene->render(&painter, this->rect());
}

void MinimapWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPoint p = event->pos();
    // position to scenePos
    QPoint sp = maptoscene(p);
    global::instance()->centerto(sp);
}

QPoint MinimapWidget::maptoscene(QPoint p)
{
    // p : 0 ~ rect
    // scene : 0 ~ scenerect
    qreal scale = scene->sceneRect().width() / this->rect().width();
    QPoint sp = p * scale;
    return sp;
}
