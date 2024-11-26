#ifndef DRAGGABLEITEM_H
#define DRAGGABLEITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QString>


class DraggableItem
{
public:
    DraggableItem(QGraphicsItem *item, QString shapeType);
    ~DraggableItem();

    QGraphicsItem *dragItem;
};

class DraggableRectItem : public QGraphicsRectItem
{
public:
    explicit DraggableRectItem(QGraphicsRectItem *rectItem);
    QGraphicsRectItem *rectItem;
    QBrush *brush;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

class DraggableEllipseItem : public QGraphicsEllipseItem
{
public:
    explicit DraggableEllipseItem(QGraphicsEllipseItem *Item);
    QGraphicsEllipseItem *Item;
    QBrush *brush;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

class DraggableTriangleItem : public QGraphicsPolygonItem
{
public:
    explicit DraggableTriangleItem(QGraphicsPolygonItem *Item);
    QGraphicsPolygonItem *Item;
    QBrush *brush;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

class DraggableTextItem : public QGraphicsTextItem
{
public:
    explicit DraggableTextItem(QGraphicsTextItem *Item);
    DraggableTextItem *Item;
    QColor *brush;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // DRAGGABLEITEM_H
