#ifndef COMMANDS_H
#define COMMANDS_H


#include <QUndoCommand>
#include <QGraphicsItem>
#include "global.h"


class MyGraphicsScene;
class MyItem;

class MyUndoCommand : public QUndoCommand
{
public:
    MyUndoCommand();
    // QUndoCommand interface
public:
    //virtual int id() const override;
};

class AddCommand : public MyUndoCommand
{
public:
    AddCommand(MyGraphicsScene *scene, QPointF pos, int parentId, int id);

    // QUndoCommand interface
public:
    void undo() override;
    void redo() override;

private:
    MyGraphicsScene *scene;
    QPointF pos;
    QGraphicsItem *parent;
    int parentId;
    int id;
};


class DeleteCommand : public MyUndoCommand
{
public:
    DeleteCommand(MyGraphicsScene *scene, int id, int parentId);
    virtual ~DeleteCommand();

    // QUndoCommand interface
public:
    void undo() override;
    void redo() override;

private:
    MyGraphicsScene *scene;
    int id;
    int parentId;
    MyItem *item;
};


class MoveCommand : public MyUndoCommand
{
public:
    MoveCommand(MyGraphicsScene *scene, int id, QPointF oldpos, QPointF pos);

    // QUndoCommand interface
public:
    void undo() override;
    void redo() override;

private:
    MyGraphicsScene *scene;
    int id;
    QPointF pos;
    QPointF oldpos;
};


class DataCommand : public MyUndoCommand
{
public:
    DataCommand(MyGraphicsScene *scene, int id, QString oldData, QString data);

    // QUndoCommand interface
public:
    void undo() override;
    void redo() override;

private:
    MyGraphicsScene *scene;
    int id;
    QString oldData;
    QString data;
};


class SizeCommand : public MyUndoCommand
{
public:
    SizeCommand(MyGraphicsScene *scene, int id, QRectF oldRect, QRectF rect);

    // QUndoCommand interface
public:
    void undo() override;
    void redo() override;

private:
    MyGraphicsScene *scene;
    int id;
    QRectF oldRect;
    QRectF rect;
};


class DetachCommand : public MyUndoCommand
{
public:
    DetachCommand(MyGraphicsScene *scene, int id);

    // QUndoCommand interface
public:
    void undo() override;
    void redo() override;

private:
    MyGraphicsScene *scene;
    int id;
    int parentId;
};



class AttachCommand : public MyUndoCommand
{
public:
    AttachCommand(MyGraphicsScene *scene, int id, int parentId);

    // QUndoCommand interface
public:
    void undo() override;
    void redo() override;

private:
    MyGraphicsScene *scene;
    int id;
    int parentId;
};


#endif // COMMANDS_H
