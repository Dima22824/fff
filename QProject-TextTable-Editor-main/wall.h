#ifndef WALL_H
#define WALL_H

#include <QGraphicsRectItem>

class Wall : public QGraphicsRectItem {
public:
    // Конструктор для создания стены
    Wall(qreal x, qreal y, qreal width, qreal height, QGraphicsScene *scene);

    // Метод для проверки, является ли объект стеной
    bool isWall() const;
};

#endif // WALL_H
