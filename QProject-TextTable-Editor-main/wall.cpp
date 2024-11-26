#include "Wall.h"
#include <QGraphicsScene>

Wall::Wall(qreal x, qreal y, qreal width, qreal height, QGraphicsScene *scene)
    : QGraphicsRectItem(x, y, width, height) {
    setPen(QPen(Qt::gray));  // Цвет границы стены
    setBrush(Qt::gray);  // Цвет заливки
    setFlag(QGraphicsItem::ItemIsFocusable);  // Флаг для возможной фокусировки
}

bool Wall::isWall() const {
    return true;  // Этот метод возвращает true, так как это стена
}
