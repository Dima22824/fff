#ifndef GRAPHICSEDITOR_H
#define GRAPHICSEDITOR_H

#include "FigureTool.h"
#include "figuredialog.h"
#include "draggableitem.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QToolBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>
#include <QGraphicsLineItem>
#include <QBrush>
#include <QColorDialog>
#include <QInputDialog>
#include <QPainter>
#include <QFileDialog>
#include <QImageWriter>
#include <QMessageBox>

class GraphicsEditor : public QWidget
{
    Q_OBJECT

public:
    explicit GraphicsEditor(QWidget *parent = nullptr);
    void addMovingObject();

protected:
    // Добавляем объявление метода eventFilter
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QToolBar *toolBar;
    QBrush brush;
    QPen pen;
    QGraphicsLineItem *currentLine;
    QPointF previousPoint;

    bool brushActive;
    bool eraserActive;
    bool selectMode = false;  // Флаг режима выделения

    void setupScene();
    void setupWalls();

    int toolSize = 3; // Начальный размер и для кисти, и для ластика

};

#endif // GRAPHICSEDITOR_H
