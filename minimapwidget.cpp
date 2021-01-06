#include "minimapwidget.h"
#include <QPaintEvent>
#include <QPainter>


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
