#ifndef MYARROW_H
#define MYARROW_H

#include <QGraphicsItem>

class MyArrow : public QGraphicsItem
{
protected:
    MyArrow(QGraphicsItem *parent = nullptr);
    void setP(QPointF p0, QPointF p1);

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QPointF p0;
    QPointF p1;


friend class MyItem;
};

#endif // MYARROW_H
