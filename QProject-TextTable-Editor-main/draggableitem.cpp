#include "DraggableItem.h"
#include <QBrush>


DraggableItem::DraggableItem(QGraphicsItem *item, QString shapeType)
{
    if (shapeType == "Квадрат") {
        QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem *>(item);
        if (rectItem) {
            dragItem = new DraggableRectItem(rectItem);
        } else {
            throw std::logic_error("bad cast");
        }
    } else if (shapeType == "Круг") {
        QGraphicsEllipseItem *ellipseItem = dynamic_cast<QGraphicsEllipseItem *>(item);
        if (ellipseItem) {
            dragItem = new DraggableEllipseItem(ellipseItem);
        } else {
            throw std::logic_error("bad cast");
        }
    } else if (shapeType == "Треугольник") {
        QGraphicsPolygonItem *triangleItem = dynamic_cast<QGraphicsPolygonItem *>(item);
        if (triangleItem) {
            dragItem = new DraggableTriangleItem(triangleItem);
        } else {
            throw std::logic_error("bad cast");
        }
    } else if (shapeType == "Текст") {
        QGraphicsTextItem *textItem = dynamic_cast<QGraphicsTextItem *>(item);
        if (textItem) {
            dragItem = new DraggableTextItem(textItem);
        } else {
            throw std::logic_error("bad cast for text");
        }
    }
}

DraggableItem::~DraggableItem(){
    delete dragItem;
}

DraggableRectItem::DraggableRectItem(QGraphicsRectItem *Item)
    : QGraphicsRectItem(Item->rect())
{
    brush = new QBrush;
    *brush = Item->brush();

    setBrush(*brush);
    setFlag(QGraphicsRectItem::ItemIsMovable); // Устанавливаем флаг перемещения
    setFlag(QGraphicsRectItem::ItemIsSelectable); // Флаг выделения
}

void DraggableRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setBrush(QBrush(Qt::yellow)); // Изменяем цвет при захвате
    QGraphicsRectItem::mousePressEvent(event); // Вызываем базовый обработчик
}

void DraggableRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsRectItem::mouseMoveEvent(event); // Перемещение объекта
}

void DraggableRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setBrush(*brush);
    QGraphicsRectItem::mouseReleaseEvent(event); // Вызываем базовый обработчик
}

// ELLIPSE

DraggableEllipseItem::DraggableEllipseItem(QGraphicsEllipseItem *Item)
    : QGraphicsEllipseItem(Item->rect())
{
    brush = new QBrush;
    *brush = Item->brush();

    setBrush(*brush);
    setFlag(QGraphicsEllipseItem::ItemIsMovable); // Устанавливаем флаг перемещения
    setFlag(QGraphicsEllipseItem::ItemIsSelectable); // Флаг выделения
}

void DraggableEllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setBrush(QBrush(Qt::yellow)); // Изменяем цвет при захвате
    QGraphicsEllipseItem::mousePressEvent(event); // Вызываем базовый обработчик
}

void DraggableEllipseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mouseMoveEvent(event); // Перемещение объекта
}

void DraggableEllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setBrush(*brush);
    QGraphicsEllipseItem::mouseReleaseEvent(event); // Вызываем базовый обработчик
}

// TRIANGLE

DraggableTriangleItem::DraggableTriangleItem(QGraphicsPolygonItem *Item)
    : QGraphicsPolygonItem(Item->polygon())
{
    brush = new QBrush;
    *brush = Item->brush();

    setBrush(*brush);
    setFlag(QGraphicsItem::ItemIsMovable);   // Устанавливаем флаг перемещения
    setFlag(QGraphicsItem::ItemIsSelectable); // Устанавливаем флаг выделения
}

void DraggableTriangleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setBrush(QBrush(Qt::yellow)); // Изменяем цвет при захвате
    QGraphicsPolygonItem::mousePressEvent(event); // Вызываем базовый обработчик
}

void DraggableTriangleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPolygonItem::mouseMoveEvent(event); // Перемещение объекта
}

void DraggableTriangleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setBrush(*brush);
    QGraphicsPolygonItem::mouseReleaseEvent(event); // Вызываем базовый обработчик
}

// TEXT

DraggableTextItem::DraggableTextItem(QGraphicsTextItem *Item)
    : QGraphicsTextItem(Item->toPlainText())
{
    brush = new QColor;
    *brush = Item->defaultTextColor();

    setDefaultTextColor(*brush);
    //setTextInteractionFlags(Qt::TextEditorInteraction); // Позволяет редактировать текст
    setFlag(QGraphicsTextItem::ItemIsMovable);          // Устанавливаем флаг перемещения
    setFlag(QGraphicsTextItem::ItemIsSelectable);       // Флаг выделенияF
}

void DraggableTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setDefaultTextColor(Qt::yellow); // Изменяем цвет при захвате
    QGraphicsTextItem::mousePressEvent(event); // Вызываем базовый обработчик
}

void DraggableTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseMoveEvent(event); // Перемещение объекта
}

void DraggableTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setDefaultTextColor(*brush);
    QGraphicsTextItem::mouseReleaseEvent(event); // Вызываем базовый обработчик
}

