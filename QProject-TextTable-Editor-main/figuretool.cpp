#include "FigureTool.h"

// Конструктор, принимающий сцену
FigureTool::FigureTool(QGraphicsScene *scene)
    : scene(scene) {}

// Метод для добавления прямоугольника
QGraphicsItem* FigureTool::addRectangle() {
    QRectF rect(0, 0, 100, 100);
    auto *item = new QGraphicsRectItem(rect);
    item->setBrush(Qt::white);
    item->setPen(QPen(Qt::black, 2));
    item->setPos(100, 100);
    scene->addItem(item);
    return item;
}

// Метод для добавления эллипса
QGraphicsItem* FigureTool::addEllipse() {
    QRectF rect(0, 0, 50, 50);
    auto *item = new QGraphicsEllipseItem(rect);
    item->setBrush(Qt::white);
    item->setPen(QPen(Qt::black, 2));
    item->setPos(150, 150);
    scene->addItem(item);
    return item;
}

// Метод для добавления треугольника
QGraphicsItem* FigureTool::addTriangle() {
    QPolygonF triangle;
    triangle << QPointF(0, 50) << QPointF(50, 50) << QPointF(25, 0);

    auto *item = new QGraphicsPolygonItem(triangle);
    item->setBrush(Qt::white);
    item->setPen(QPen(Qt::black, 2));
    item->setPos(200, 200);
    scene->addItem(item);
    return item;
}

QGraphicsTextItem* FigureTool::addText(const QString& text)
{
    QGraphicsTextItem* textItem = new QGraphicsTextItem(text);

    // Устанавливаем начальную позицию текста (например, центр сцены)
    textItem->setPos(50, 50);

    // Настройка текста (опционально: шрифт, размер, цвет и т.д.)
    QFont font("Arial", 14); // Шрифт Arial, размер 14
    textItem->setFont(font);
    textItem->setDefaultTextColor(Qt::black);
    scene->addItem(textItem);

    return textItem;
}

// Метод для изменения цвета заливки
void FigureTool::fillColor(QGraphicsItem *item) {
    QColor fillColor = QColorDialog::getColor(Qt::white, nullptr, "Select Fill Color");
    if (fillColor.isValid()) {
        // Проверяем, есть ли элементы на сцене
        if (!scene->items().isEmpty()) {
            // Приводим к QAbstractGraphicsShapeItem
            if (auto *shape = dynamic_cast<QAbstractGraphicsShapeItem*>(item)) {
                shape->setBrush(fillColor);
            }
        }
    }
}

// Метод для изменения размеров фигуры
void FigureTool::resizeShape(QGraphicsItem *item) {
    if (!item) return; // Проверяем, что элемент не равен nullptr

    // Для прямоугольника
    if (auto *rectItem = dynamic_cast<QGraphicsRectItem *>(item)) {
        QRectF currentRect = rectItem->rect();
        bool ok;
        int newWidth = QInputDialog::getInt(nullptr, "Resize Rectangle",
                                            "Enter new width:",
                                            currentRect.width(), 1, 1000, 1, &ok);
        if (ok) {
            rectItem->setRect(0, 0, newWidth, newWidth);
        }
    }
    // Для круга (представленного как эллипс)
    else if (auto *ellipseItem = dynamic_cast<QGraphicsEllipseItem *>(item)) {
        QRectF currentRect = ellipseItem->rect();
        bool ok;
        int newWidth = QInputDialog::getInt(nullptr, "Resize Circle",
                                            "Enter new diameter:",
                                            currentRect.width(), 1, 1000, 1, &ok);
        if (ok) {
            ellipseItem->setRect(0, 0, newWidth, newWidth); // Окружность, поэтому ширина = высота
        }
    }
    // Для треугольника (или многоугольника)
    else if (auto *polygonItem = dynamic_cast<QGraphicsPolygonItem *>(item)) {
        QPolygonF currentPolygon = polygonItem->polygon();
        bool ok;
        qreal scaleFactor = QInputDialog::getDouble(nullptr, "Resize Polygon",
                                                    "Enter scale factor (e.g., 1.5):",
                                                    1.0, 0.1, 10.0, 1, &ok);
        if (ok) {
            QPolygonF scaledPolygon;
            for (const QPointF &point : currentPolygon) {
                scaledPolygon << QPointF(point.x() * scaleFactor, point.y() * scaleFactor);
            }
            polygonItem->setPolygon(scaledPolygon);
        }
    }
    // Для текста
    else if (auto *textItem = dynamic_cast<QGraphicsTextItem *>(item)) {
        QFont currentFont = textItem->font();
        bool ok;
        int newFontSize = QInputDialog::getInt(nullptr, "Resize Text",
                                               "Enter new font size:",
                                               currentFont.pointSize(), 1, 100, 1, &ok);
        if (ok) {
            currentFont.setPointSize(newFontSize); // Устанавливаем новый размер шрифта
            textItem->setFont(currentFont);
        }
    }
}



// Метод для изменения цвета обводки
void FigureTool::changeStroke() {
    QColor strokeColor = QColorDialog::getColor(Qt::black, nullptr, "Select Stroke Color");
    if (strokeColor.isValid()) {
        for (auto *item : scene->selectedItems()) {
            if (auto *shape = dynamic_cast<QAbstractGraphicsShapeItem*>(item)) {
                QPen pen = shape->pen();
                pen.setColor(strokeColor);
                shape->setPen(pen);
            }
        }
    }
}

// Метод для изменения толщины обводки
void FigureTool::changeStrokeWidth() {
    bool ok;
    int newWidth = QInputDialog::getInt(nullptr, "Stroke Width",
                                        "Enter width (1-20):",
                                        2, 1, 20, 1, &ok);
    if (ok) {
        for (auto *item : scene->selectedItems()) {
            if (auto *shape = dynamic_cast<QAbstractGraphicsShapeItem*>(item)) {
                QPen pen = shape->pen();
                pen.setWidth(newWidth);
                shape->setPen(pen);
            }
        }
    }
}
