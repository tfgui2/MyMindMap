#include "commands.h"
#include "mygraphicsscene.h"
#include "myarrow.h"



MyUndoCommand::MyUndoCommand()
{
    g_modified = true;
}


AddCommand::AddCommand(MyGraphicsScene *scene, QPointF pos, int parentId, int id)
{
    this->scene = scene;
    this->pos = pos;
    this->parentId = parentId;
    this->id = id;
}

void AddCommand::undo()
{
    MyItem *item = scene->deleteMyItem(id);
    delete item;
}

void AddCommand::redo()
{
    id = this->scene->addMyItem(pos, parentId, id, "", QRectF(0,0,100,50));
}


DeleteCommand::DeleteCommand(MyGraphicsScene *scene, int id, int parentId)
{
    this->scene = scene;
    this->id = id;
    this->parentId = parentId;
}

DeleteCommand::~DeleteCommand()
{
    if (this->item) {
        delete this->item;
    }
}

void DeleteCommand::undo()
{
    MyItem *parentItem = scene->findMyItem(parentId);
    if (parentItem) {
        item->setParentItem(parentItem);
    }
    else {
        scene->addItem(item);
    }
    item = nullptr;
}

void DeleteCommand::redo()
{
    item = scene->deleteMyItem(id);
}


MoveCommand::MoveCommand(MyGraphicsScene *scene, int id, QPointF oldpos, QPointF pos)
{
    this->scene = scene;
    this->id = id;
    this->oldpos = oldpos;
    this->pos = pos;
}

void MoveCommand::undo()
{
    MyItem *item = scene->findMyItem(id);
    item->setPos(oldpos);
}

void MoveCommand::redo()
{
    MyItem *item = scene->findMyItem(id);
    item->setPos(pos);
}


DataCommand::DataCommand(MyGraphicsScene *scene, int id, QString oldData, QString data)
{
    this->scene = scene;
    this->id = id;
    this->oldData = oldData;
    this->data = data;
}

void DataCommand::undo()
{
    MyItem *item = scene->findMyItem(id);
    if (item) {
        item->setMyData(oldData);
    }
}

void DataCommand::redo()
{
    MyItem *item = scene->findMyItem(id);
    if (item) {
        item->setMyData(data);
    }
}

SizeCommand::SizeCommand(MyGraphicsScene *scene, int id, QRectF oldRect, QRectF rect)
{
    this->scene = scene;
    this->id = id;
    this->oldRect = oldRect;
    this->rect = rect;
}

void SizeCommand::undo()
{
    MyItem *item = scene->findMyItem(id);
    if (item) {
        item->resize(oldRect);
        this->scene->onMyItemResized(item);
    }
}

void SizeCommand::redo()
{
    MyItem *item = scene->findMyItem(id);
    if (item) {
        item->resize(rect);
        this->scene->onMyItemResized(item);
    }
}

DetachCommand::DetachCommand(MyGraphicsScene *scene, int id)
{
    this->scene = scene;
    this->id = id;
}

void DetachCommand::undo()
{
    scene->_attach(id, parentId);
}

void DetachCommand::redo()
{
    MyItem *myitem = scene->findMyItem(id);
    this->parentId = myitem->parentId();

    scene->_detach(id);
}

AttachCommand::AttachCommand(MyGraphicsScene *scene, int id, int parentId)
{
    this->scene = scene;
    this->id = id;
    this->parentId = parentId;
}

void AttachCommand::undo()
{
    scene->_detach(id);
}

void AttachCommand::redo()
{
    scene->_attach(id, parentId);
}
