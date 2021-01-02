#include "myitem.h"
#include <QPainter>
#include <QColor>
#include "mygraphicsscene.h"
#include "commands.h"
#include <QGraphicsSceneMouseEvent>
#include "inputdialog.h"
#include "global.h"
#include "myarrow.h"
#include "resizer.h"



MyItem::MyItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    setFlags(ItemIsSelectable | ItemSendsGeometryChanges);
    qreal width = 100;
    qreal height = 50;
    rect = QRect(0,0,width,height);

}

MyItem::~MyItem()
{
}

void MyItem::setMyData(QString data)
{
    this->data = data;
    this->update();
}

QString MyItem::getMyData()
{
    return this->data;
}

int MyItem::parentId()
{
    MyItem* p = (MyItem*)this->parentItem();
    if (p)
        return p->id;
    return 0;
}

void MyItem::childToList(QList<MyItem *> &list)
{
    QList<QGraphicsItem*> items = this->childItems();
    foreach (QGraphicsItem* item, items) {
        MyItem *myitem = dynamic_cast<MyItem*>(item);
        if (myitem) {
            list.append(myitem);
            myitem->childToList(list);
        }
    }
}

QRectF MyItem::boundingRect() const
{
    qreal penwidth = 1;
    QRectF br = rect;
    br.adjust(-penwidth, -penwidth, penwidth, penwidth);
    return br;
}

QRectF MyItem::getRect() const
{
    return this->rect;
}

void MyItem::setRect(QRectF r)
{
    prepareGeometryChange();
    this->rect = r;
    update();
}

void MyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (option) {

    }

    if (widget) {

    }

    if (isSelected()) {
        painter->setPen(Qt::red);
    }
    else {
        painter->setPen(Qt::gray);
    }

    painter->setBrush(this->color);
    painter->drawRoundedRect(rect, 5, 5);

    QRectF textRect = rect.adjusted(5,5,-5,-5);
    painter->drawText(textRect, Qt::AlignLeft, this->data, &adjustTextRect);
}

void MyItem::setParentItem(MyItem *parent)
{
    QGraphicsItem::setParentItem(parent);

    if (parent) {
        if (arrowFromParent == nullptr) {
            arrowFromParent = new MyArrow;
        }
        arrowFromParent->setParentItem(parent);
        refreshArrow();
    }
    else {
        if (arrowFromParent) {
            scene()->removeItem(arrowFromParent);
            delete arrowFromParent;
            arrowFromParent = nullptr;
        }
    }
}

void MyItem::setPos(const QPointF &pos)
{
    QGraphicsItem::setPos(pos);
    refreshArrow();
}

void MyItem::resize(QRectF r)
{
    this->setRect(r);
    refreshArrow();
}

void MyItem::setColor(QColor c)
{
    this->color = c;
    update();
}

QColor MyItem::getColor()
{
    return this->color;
}

QRectF MyItem::autoAdjustRect()
{
    // resize rect
    qreal d = 5;
    return adjustTextRect.adjusted(-d,-d,d,d);
}

void MyItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (g_key_space) {
        setFlag(ItemIsMovable, true);
    }

    QGraphicsItem::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        oldpos = this->pos();
        return;
    }
}

void MyItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setFlag(ItemIsMovable, false);

    QGraphicsItem::mouseReleaseEvent(event);

    if (event->button() == Qt::LeftButton) {
        QPointF pos = this->pos();

        if (oldpos != pos) {
            MyGraphicsScene *scene = (MyGraphicsScene *) (this->scene());
            scene->undoStack->push(new MoveCommand(scene, this->id, oldpos, pos));
            return;
        }
    }
}

void MyItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);

    if (event->button() == Qt::LeftButton) {

        InputDialog dlg;
        dlg.setData(data);
        if (dlg.exec() == QDialog::Accepted) {
            QString newdata = dlg.getData();
            MyGraphicsScene *scene = (MyGraphicsScene *) (this->scene());
            scene->undoStack->push(new DataCommand(scene, this->id, data, newdata));
        }
        //this->scene()->setFocus(Qt::ActiveWindowFocusReason);
        return;
    }

}

void MyItem::refreshArrow()
{
    if (arrowFromParent == nullptr)
        return;

    QPointF p0 = this->parentItem()->boundingRect().center();

    QRectF aa = this->boundingRect().translated(this->pos());
    QPointF p1 = aa.center();

    QLineF arrowLine(p0, p1);

    QList<QLineF> lines = rectToLines(aa);
    QPointF intersectP;
    foreach (QLineF l, lines) {
        QLineF::IntersectType res = arrowLine.intersect(l, &intersectP);
        if (res == QLineF::BoundedIntersection) {
            p1 = intersectP;
            break;
        }
    }

    arrowFromParent->setP(p0, p1);
}

QList<QLineF> MyItem::rectToLines(QRectF r)
{
    QList<QLineF> list;
    list.append(QLineF(r.topLeft(), r.topRight()));
    list.append(QLineF(r.topRight(), r.bottomRight()));
    list.append(QLineF(r.bottomRight(), r.bottomLeft()));
    list.append(QLineF(r.bottomLeft(), r.topLeft()));

    return list;
}

QVariant MyItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSceneChange) {
        if (arrowFromParent) {
            if (value.isNull() == false) {
                QGraphicsScene *scene = value.value<QGraphicsScene*>();
                scene->addItem(arrowFromParent);
            }
            else {
                scene()->removeItem(arrowFromParent);
            }
        }
    }
    else if (change == ItemPositionChange) {
        refreshArrow();
    }
    return value;
}
