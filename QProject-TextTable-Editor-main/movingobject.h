// MovingObject.h
#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QObject>
#include <QMediaPlayer>
#include <string>


class MovingObject : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
private:
    QGraphicsScene *scene;
    int speedX;
    int speedY;

public:
    MovingObject(QGraphicsScene *scene, const QPixmap &pixmap, int speedX, int speedY);
    void move();  // Перемещение объекта
    void checkCollision();  // Проверка на столкновение
    void checkBoundaryCollision();

    QString path = ":/sound/amezing.wav";
};

#endif // MOVINGOBJECT_H
