#ifndef MINIMAPWIDGET_H
#define MINIMAPWIDGET_H

#include <QWidget>
#include <QGraphicsScene>

class MinimapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MinimapWidget(QWidget *parent = nullptr);
    QGraphicsScene *scene;

signals:


    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

    // QWidget interface
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    QPoint maptoscene(QPoint p);
};

#endif // MINIMAPWIDGET_H
