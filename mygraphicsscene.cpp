#include "mygraphicsscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QRandomGenerator>
#include "commands.h"
#include "myitem.h"
#include <QGraphicsItem>
#include "resizer.h"


MyGraphicsScene::MyGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
    undoStack = new QUndoStack(this);
    resizer = new Resizer;
    addItem(resizer);
    resizer->hide();
}

int MyGraphicsScene::addMyItem(QPointF pos, int parentId, int id, QString data, QRectF rect)
{
    MyItem *parent = findMyItem(parentId);

    MyItem *item = new MyItem(nullptr);
    item->id = id;
    item->setRect(rect);

    if (parent) {
        item->setParentItem(parent);
        QPointF pos_child = pos - parent->scenePos();
        item->setPos(pos_child);
    }
    else {
        item->setPos(pos);
        this->addItem(item);
    }

    item->setMyData(data);
    return item->id;
}

MyItem* MyGraphicsScene::deleteMyItem(int id)
{
    MyItem *item = findMyItem(id);
    if (item) {
        removeItem(item);
    }
    return item;
}

void MyGraphicsScene::deleteSelectedItem()
{
    QList<QGraphicsItem*> items = this->selectedItems();
    foreach (QGraphicsItem *item, items) {
        MyItem *myitem = dynamic_cast<MyItem*>(item);
        if (myitem) {
            if (myitem->parentItem()) {
                MyItem *parent = dynamic_cast<MyItem*>(myitem->parentItem());
                undoStack->push(new DeleteCommand(this, myitem->id, parent->id));
            }
            else {
                undoStack->push(new DeleteCommand(this, myitem->id, 0));
            }
        }
    }
}

void MyGraphicsScene::resizerSelectedItem()
{
    MyItem *myitem = myselectedItem();
    if (myitem == nullptr)
        return;

    resizer->setTarget(myitem);

}

void MyGraphicsScene::detachSelectedItem()
{
    MyItem *myitem = myselectedItem();
    if (myitem == nullptr)
        return;

    MyItem *parent = dynamic_cast<MyItem*>(myitem->parentItem());
    if (parent == nullptr)
        return;

    undoStack->push(new DetachCommand(this, myitem->id));
}

void MyGraphicsScene::colorChangeSelectedItem(QColor color)
{
    MyItem *myitem = myselectedItem();
    if (myitem == nullptr)
        return;

    myitem->setColor(color);
}

QColor MyGraphicsScene::getColorSelectedItem()
{
    MyItem *myitem = myselectedItem();
    if (myitem == nullptr)
        return Qt::black;

    return myitem->getColor();
}

void MyGraphicsScene::adjustSelectedItem()
{
    MyItem *myitem = myselectedItem();
    if (myitem == nullptr)
        return;

    QRectF oldrect = myitem->getRect();
    QRectF rect = myitem->autoAdjustRect();
    undoStack->push(new SizeCommand(this, myitem->id, oldrect, rect));
}

bool MyGraphicsScene::contain(QGraphicsItem *item)
{
    QList<QGraphicsItem*> items = this->items();
    foreach (QGraphicsItem *i, items) {
        if (i == item) {
            return true;
        }
    }

    return false;
}

bool MyGraphicsScene::save(QDataStream &out)
{
    out << QString("MyMindMap File version");
    out << new_version; // version

    QList<MyItem*> items = this->mysorteditems();
    out << (qint32)items.length();
    foreach (MyItem *item, items) {
        _saveMyItem(out, item);
    }

    return true;
}

bool MyGraphicsScene::load(QDataStream &in)
{
    QString str;
    in >> str;
    in >> version;

    qint32 len;
    in >> len;
    for (int i = 0; i < len; i++) {
        _loadMyItem(in);
    }
    return true;
}

bool MyGraphicsScene::_saveMyItem(QDataStream &out, MyItem *item)
{
    out << item->scenePos() << item->parentId() << item->id;
    out << item->getMyData();
    out << item->getRect();
    out << item->getColor();
    return true;
}

bool MyGraphicsScene::_loadMyItem(QDataStream &in)
{
    QPointF pos;
    int parentId;
    int id;
    in >> pos >> parentId >> id;
    QString data;
    in >> data;
    QRectF rect;
    in >> rect;

    QColor color = Qt::white;
    if (version > 1) {
        in >> color;
    }

    int itemid = addMyItem(pos, parentId, id, data, rect);

    MyItem* item = findMyItem(itemid);
    item->setColor(color);

    return true;
}

void MyGraphicsScene::_attach(int id, int parent)
{
    MyItem *myitem = findMyItem(id);
    MyItem *parentItem = findMyItem(parent);

    QPointF newpos = myitem->scenePos() - parentItem->scenePos();
    myitem->setPos(newpos);
    myitem->setParentItem(parentItem);
}

void MyGraphicsScene::_detach(int id)
{
    MyItem *myitem = findMyItem(id);

    QPointF newpos = myitem->scenePos();
    myitem->setPos(newpos);
    myitem->setParentItem(nullptr);
}

MyItem *MyGraphicsScene::findMyItem(int id)
{
    QList<QGraphicsItem*> items = this->items();

    foreach (QGraphicsItem *item, items) {
        MyItem *myitem = dynamic_cast<MyItem*>(item);
        if (myitem) {
            if (myitem->id == id) {
                return myitem;
            }
        }
    }

    return nullptr;
}

void MyGraphicsScene::linkTo(int parentId, QPointF targetPos)
{
    MyItem* parentItem = findMyItem(parentId);
    if (parentItem == nullptr)
        return;

    QList<MyItem*> items = myitems(targetPos);
    if (items.length() == 0)
        return;

    MyItem* myitem = items[0];
    if (myitem->parentId() != 0)
        return;

    undoStack->push(new AttachCommand(this, myitem->id, parentId));
}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        QList<MyItem*> items = this->myitems(event->scenePos());
        if (items.length() > 0) {
            selectedparentId = ((MyItem*)(items[0]))->id;
        }
        else {
            selectedparentId = 0;
        }
        return;
    }

    QGraphicsScene::mousePressEvent(event);
}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        int res = checkPosInOther(event->scenePos());
        if (res == 0) {
            undoStack->push(new AddCommand(this, event->scenePos(), selectedparentId, this->generateId()));
        }
        else if (res == 1) { // 다른쪽에 붙이기, 부모가 없어야됨
            if (selectedparentId != 0) {
                this->linkTo(selectedparentId, event->scenePos());
            }
        }
        return;
    }

    QGraphicsScene::mouseReleaseEvent(event);
}

int MyGraphicsScene::generateId()
{
    return QRandomGenerator::global()->generate() + 1;
}

// if pos in parent: -1, pos in other: 1, pos in no: 0
int MyGraphicsScene::checkPosInOther(QPointF pos)
{
    int result = 0;

    QList<MyItem*> items = this->myitems(pos);
    foreach (MyItem *item, items) {
        if (item->id == selectedparentId) {
            return -1;
        }
        else {
            result = 1;
        }
    }

    return result;
}

QList<MyItem *> MyGraphicsScene::myitems(const QPointF pos)
{
    QList<MyItem*> list;

    QList<QGraphicsItem*> items = this->items(pos);
    foreach (QGraphicsItem *item, items) {
        MyItem *myitem = dynamic_cast<MyItem*>(item);
        if (myitem) {
            list.append(myitem);
        }
    }

    return list;
}

QList<MyItem *> MyGraphicsScene::mysorteditems()
{
    QList<MyItem*> list;

    // roots
    QList<QGraphicsItem*> items = this->items(Qt::AscendingOrder);
    foreach (QGraphicsItem *item, items) {
        MyItem *myitem = dynamic_cast<MyItem*>(item);
        if (myitem && myitem->parentId() == 0) {
            list.append(myitem);
        }
    }

    foreach (MyItem *root, list) {
        root->childToList(list);
    }

    return list;
}

MyItem *MyGraphicsScene::myselectedItem()
{
    QList<QGraphicsItem*> items = this->selectedItems();
    if (items.length() == 0)
        return nullptr;

    MyItem *myitem = dynamic_cast<MyItem*>(items[0]);
    return myitem;
}

void MyGraphicsScene::onMyItemResized(MyItem *item)
{
    if(resizer->isVisible()) {
        resizer->setTarget(item);
    }
}
