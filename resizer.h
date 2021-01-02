#ifndef RESIZER_H
#define RESIZER_H

#include <QGraphicsScene>
#include <QGraphicsItem>

class MyItem;

class Resizer : public QGraphicsItem
{
public:
    Resizer();

public:
    void setTarget(MyItem* target);

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    QList<QRectF> getHandleRects();

private:
    QRectF rect;
    qreal handleSize = 8;
    int selectedhandle = -1;
    QPointF oldPos;
    MyItem* m_target;

    // QGraphicsItem interface
protected:
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

#endif // RESIZER_H
