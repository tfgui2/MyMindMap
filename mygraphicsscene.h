#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QUndoStack>
#include "myitem.h"


class Resizer;

class MyGraphicsScene : public QGraphicsScene
{
public:
    MyGraphicsScene(QObject *parent);

    void deleteSelectedItem();
    void resizerSelectedItem();
    void detachSelectedItem();
    void colorChangeSelectedItem(QColor color);
    QColor getColorSelectedItem();
    void adjustSelectedItem();

    QUndoStack *undoStack;

    bool contain(QGraphicsItem *item);

    bool save(QDataStream &out);
    bool load(QDataStream &in);

    qint32 new_version = 2;
    qint32 version;

protected:
    bool _saveMyItem(QDataStream &out, MyItem *item);
    bool _loadMyItem(QDataStream &in);

    void _attach(int id, int parent);
    void _detach(int id);

protected:
    int addMyItem(QPointF pos, int parentId, int id, QString data = "", QRectF rect = QRectF());
    MyItem* deleteMyItem(int id);
    MyItem* findMyItem(int id);
    void linkTo(int parentId, QPointF targetPos);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    int generateId();
    // if pos in parent: -1, pos in other: 1, pos in no: 0
    int checkPosInOther(QPointF pos);

private:
    QList<MyItem*> myitems(const QPointF pos);
    QList<MyItem*> mysorteditems();
    MyItem* myselectedItem();

private:
    int selectedparentId;

protected:
    Resizer *resizer;
    void onMyItemResized(MyItem *item);


friend class AddCommand;
friend class DeleteCommand;
friend class MoveCommand;
friend class DataCommand;
friend class SizeCommand;
friend class DetachCommand;
friend class AttachCommand;
};

#endif // MYGRAPHICSSCENE_H
