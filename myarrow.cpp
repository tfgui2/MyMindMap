#include "myarrow.h"
#include <QPainter>
#include <qmath.h>


MyArrow::MyArrow(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    setFlags(ItemStacksBehindParent);
}

QRectF MyArrow::boundingRect() const
{
    QRectF rect = QRectF(p0, p1);
    rect = rect.normalized();
    rect.adjust(-10, -10, 10, 10);
    return rect;
}

void MyArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (option) {

    }

    if (widget) {

    }
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::gray);
    painter->setBrush(Qt::gray);

    QLineF line(p1, p0);
    double angle = std::atan2(-line.dy(), line.dx());
    qreal arrowSize = 10;
    QPointF arrowP1 = QPointF(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize, cos(angle + M_PI - M_PI / 3) * arrowSize);

    // head
    QPolygonF arrowHead;
    arrowHead << p1 << arrowP1 + p1 << arrowP2 + p1;
    painter->drawPolygon(arrowHead);

    // body
    QPolygonF body;
    body << p1 << arrowP1 + p0 << arrowP2 + p0;
    painter->drawPolygon(body);
}

void MyArrow::setP(QPointF p0, QPointF p1)
{
    prepareGeometryChange();
    this->p0 = p0;
    this->p1 = p1;
    update();
}
