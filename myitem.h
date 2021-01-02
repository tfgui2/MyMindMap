#ifndef MYITEM_H
#define MYITEM_H

#include <QGraphicsItem>

class MyArrow;

class MyItem : public QGraphicsItem
{
public:
    MyItem(QGraphicsItem *parent);
    virtual ~MyItem();

    int id;
    void setMyData(QString data);
    QString getMyData();
    int parentId();

    void childToList(QList<MyItem*> &list);


    // QGraphicsItem interface
public:
    QRectF boundingRect() const override;
    QRectF getRect() const;
    void setRect(QRectF r);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setParentItem(MyItem *parent);
    void setPos(const QPointF &pos);
    void resize(QRectF r);
    void setColor(QColor c);
    QColor getColor();

    QRectF autoAdjustRect();
    QRectF adjustTextRect;

    // QGraphicsItem interface
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QRectF rect;
    QPointF oldpos;
    QString data;
    QColor color = Qt::white;

public:
    MyArrow *arrowFromParent = nullptr;

private:
    void refreshArrow();
    QList<QLineF> rectToLines(QRectF r);


    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

#endif // MYITEM_H
