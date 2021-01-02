#include "resizer.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "mygraphicsscene.h"
#include "commands.h"
#include "myitem.h"
#include <QCursor>


Resizer::Resizer()
    : QGraphicsItem(nullptr)
{
    setFlag(ItemIsSelectable, true);
    setZValue(1000);
    setAcceptHoverEvents(true);
}

void Resizer::setTarget(MyItem *target)
{
    prepareGeometryChange();
    this->m_target = target;
    this->rect = target->getRect();
    this->setPos(target->scenePos());

    if (this->isVisible() == false)
        this->show();
    this->setSelected(true);

}

QRectF Resizer::boundingRect() const
{
    QRectF br = this->rect;
    qreal d = handleSize/2;
    br.adjust(-d, -d, d, d);
    return br;
}

void Resizer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // dot line
    QRectF dotline = this->rect;

    QPen pen;
    pen.setColor(Qt::gray);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(dotline);

    // handle boxes
    painter->setPen(Qt::black);
    painter->setBrush(Qt::darkGray);

    QList<QRectF> handles = getHandleRects();
    foreach (QRectF r, handles) {
        painter->drawRect(r);
    }

}

void Resizer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    QList<QRectF> handles = getHandleRects();
    QPointF p = event->pos();
    selectedhandle = -1;
    for (int i = 0; i < handles.length(); i++) {
        if (handles[i].contains(p)) {
            selectedhandle = i;
            oldPos = event->pos();
            break;
        }
    }

}

void Resizer::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (selectedhandle == -1)
        return;

    QPointF diff = event->pos() - oldPos;
    prepareGeometryChange();
    switch (selectedhandle) {
    case 1: {
        rect.setLeft(event->pos().x());
    } break;

    case 6: {
        rect.setRight(event->pos().x());
    } break;

    case 3: {
        rect.setTop(event->pos().y());
    } break;

    case 4: {
        rect.setBottom(event->pos().y());
    } break;

    case 0: {
        rect.setTopLeft(event->pos());
    } break;

    case 2: {
        rect.setBottomLeft(event->pos());
    } break;

    case 5: {
        rect.setTopRight(event->pos());
    } break;

    case 7: {
        rect.setBottomRight(event->pos());
    } break;

    default:
        qDebug() << diff.x() << " , " << diff.y();
        break;

    }
    update();

}

void Resizer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (selectedhandle == -1)
        return;

    MyGraphicsScene* scene = (MyGraphicsScene*)this->scene();
    scene->undoStack->push(new SizeCommand(scene, m_target->id, m_target->getRect(), rect));
}

QVariant Resizer::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) {
        if (value.toBool() == false) {
            this->hide();
        }
    }
    return value;
}

QList<QRectF> Resizer::getHandleRects()
{
    QList<QRectF> list;

    QRectF br = boundingRect();

    QRectF smallrect = QRectF(0,0,handleSize,handleSize);
    qreal x = br.left();
    qreal y = br.top();
    qreal dx = (br.width()-handleSize) /2;
    qreal dy = (br.height()-handleSize)/2;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == 1 && j == 1)
                continue;
            qreal tx = x + (i * dx);
            qreal ty = y + (j * dy);
            smallrect.moveTo(tx, ty);
            list.append(smallrect);
        }
    }

    return list;
}

void Resizer::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QList<QRectF> handles = getHandleRects();
    QPointF p = event->pos();

    for (int i = 0; i < handles.length(); i++) {
        if (handles[i].contains(p)) {
            // cursor
            //QCursor c = this->cursor();
            setCursor(Qt::SizeAllCursor);
            return;
        }
    }

    unsetCursor();
}

void Resizer::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    unsetCursor();
}
