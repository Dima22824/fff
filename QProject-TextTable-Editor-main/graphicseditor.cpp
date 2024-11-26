#include "GraphicsEditor.h"
#include "MovingObject.h"  // Подключаем класс MovingObject
#include <QTimer>
#include "Wall.h"


GraphicsEditor::GraphicsEditor(QWidget *parent)
    : QWidget(parent),
      scene(new QGraphicsScene(this)),
      view(new QGraphicsView(scene, this)),
      toolBar(new QToolBar(this)),
      currentLine(nullptr),
      brushActive(false),
      eraserActive(false)
{
    // Устанавливаем макет
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(toolBar);
    layout->addWidget(view);
    setLayout(layout);

    // Настраиваем сцену
    setupScene();
    setupWalls();

    toolBar->addAction("Select", [this]() {
        selectMode = true;
        brushActive = false;
        eraserActive = false;
        view->viewport()->setCursor(Qt::ArrowCursor);
        // Разрешаем перемещение объектов
        view->setDragMode(QGraphicsView::RubberBandDrag);
    });

    // Добавляем базовые кнопки в панель инструментов
    toolBar->addAction("Brush", [this]() {
        brushActive = true;
        eraserActive = false;
        selectMode = false;  // Отключаем режим выделения
        pen.setColor(Qt::black);
        pen.setWidth(toolSize);
        view->viewport()->setCursor(Qt::ArrowCursor);
        view->setDragMode(QGraphicsView::NoDrag);  // Отключаем перемещение
    });
    toolBar->addAction("Color", [this]() {
        QColor color = QColorDialog::getColor(pen.color(), this, "Select Color");
        if (color.isValid()) {
            pen.setColor(color);
        }
    });

    toolBar->addAction("Size", [this]() {
        bool ok;
        int newSize = QInputDialog::getInt(this, "Tool Size",
                                         "Enter size (1-50):",
                                         toolSize, 1, 50, 1, &ok);
        if (ok) {
            toolSize = newSize;
            pen.setWidth(toolSize);

            // Обновляем курсор ластика, если он активен
            if (eraserActive) {
                QPixmap cursorPixmap(toolSize + 2, toolSize + 2);
                cursorPixmap.fill(Qt::transparent);
                QPainter painter(&cursorPixmap);
                painter.setPen(QPen(Qt::black, 1));
                painter.setBrush(Qt::white);
                painter.drawEllipse(1, 1, toolSize, toolSize);
                view->viewport()->setCursor(QCursor(cursorPixmap));
            }
        }
    });

    toolBar->addAction("Eraser", [this]() {
        eraserActive = true;
        brushActive = false;
        selectMode = false;  // Отключаем режим выделения
        QPixmap cursorPixmap(toolSize + 2, toolSize + 2);
        cursorPixmap.fill(Qt::transparent);
        QPainter painter(&cursorPixmap);
        painter.setPen(QPen(Qt::black, 1));
        painter.setBrush(Qt::white);
        painter.drawEllipse(1, 1, toolSize, toolSize);
        view->viewport()->setCursor(QCursor(cursorPixmap));
        view->setDragMode(QGraphicsView::NoDrag);  // Отключаем перемещение
    });

    toolBar->addAction("Add Figure", [this]() {
//        QRectF rect(0, 0, 50, 50); // Размер фигуры
//        auto *item = new DraggableItem();
//        item->setBrush(Qt::white);
//        item->setPen(QPen(Qt::black));
//        item->setPos(100, 100); // Начальная позиция
//        scene->addItem(item);
        QGraphicsItem *item;
        auto *dialog = new FigureDialog(scene, item, this); // Создаем окно с передачей сцены
        dialog->exec(); // Открываем диалог модально
    });

    toolBar->addAction("Remove Shape", [this]() {
        for (auto *item : scene->selectedItems()) {
            scene->removeItem(item); // Удаляем выбранный элемент
            delete item;
        }
    });

    toolBar->addAction("Change Background", [this]() {
        static int colorIndex = 0;
        QColor colors[] = {Qt::white, Qt::lightGray};

        colorIndex = (colorIndex + 1) % 4; // Циклически перебираем цвета
        scene->setBackgroundBrush(colors[colorIndex]);
    });

    toolBar->addAction("Clear All", [this]() {
        // Сохраняем стены
        QList<QGraphicsItem*> walls;
        for (auto item : scene->items()) {
            QGraphicsRectItem* rectItem = qgraphicsitem_cast<QGraphicsRectItem*>(item);
            if (rectItem) {
                QRectF rect = rectItem->rect();
                QPointF pos = rectItem->pos();
                if (pos.x() <= 5 || pos.x() >= scene->width() - 10 ||
                    pos.y() <= 5 || pos.y() >= scene->height() - 10) {
                    walls.append(item);
                    continue;
                }
            }
            scene->removeItem(item);
            delete item;
        }
    });

    toolBar->addAction("Line Style", [this]() {
        QStringList styles = {"Solid", "Dash", "Dot", "DashDot"};
        bool ok;
        QString style = QInputDialog::getItem(this, "Line Style",
                                            "Select line style:", styles, 0, false, &ok);
        if (ok) {
            if (style == "Solid") pen.setStyle(Qt::SolidLine);
            else if (style == "Dash") pen.setStyle(Qt::DashLine);
            else if (style == "Dot") pen.setStyle(Qt::DotLine);
            else if (style == "DashDot") pen.setStyle(Qt::DashDotLine);
        }
    });

    toolBar->addAction("Save", [this]() {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Image"), "",
            tr("PNG (*.png);;JPEG (*.jpg *.jpeg);;All Files (*)"));

        if (fileName.isEmpty())
            return;

        QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
        image.fill(Qt::transparent);

        QPainter painter(&image);
        scene->render(&painter);
        image.save(fileName);
    });

    toolBar->addAction("Load Background", [this]() {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Image"), "",
            tr("Image Files (*.png *.jpg *.jpeg);;All Files (*)"));

        if (!fileName.isEmpty()) {
            QImage image(fileName);
            if (!image.isNull()) {
                scene->setBackgroundBrush(QBrush(image.scaled(
                    scene->width(),
                    scene->height(),
                    Qt::KeepAspectRatioByExpanding,
                    Qt::SmoothTransformation
                )));
            }
        }
    });

    toolBar->addAction("Add Moving Object", [this]() {
        addMovingObject();  // Добавляем движущийся объект
    });



    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view->viewport()->installEventFilter(this);

        // Инициализируем pen
    pen.setColor(Qt::black);
    pen.setWidth(3);
}

void GraphicsEditor::setupScene()
{
    // Задаем размеры сцены
    int sceneWidth = 600;
    int sceneHeight = 700;
    scene->setSceneRect(0, 0, sceneWidth, sceneHeight);

    // Устанавливаем фон по умолчанию
    scene->setBackgroundBrush(Qt::white);
}

void GraphicsEditor::setupWalls()
{
    // Добавляем стены по краям сцены
    QPen wallPen(Qt::black);
    wallPen.setWidth(5);

    // Верхняя стена
    Wall *topWall = new Wall(0, 0, scene->width(), 5, scene);
    scene->addItem(topWall);

    // Нижняя стена
    Wall *bottomWall = new Wall(0, scene->height() - 5, scene->width(), 5, scene);
    scene->addItem(bottomWall);

    // Левая стена
    Wall *leftWall = new Wall(0, 0, 5, scene->height(), scene);
    scene->addItem(leftWall);

    // Правая стена
    Wall *rightWall = new Wall(scene->width() - 5, 0, 5, scene->height(), scene);
    scene->addItem(rightWall);
}

bool GraphicsEditor::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == view->viewport()) {
        if (selectMode) {
                    return QWidget::eventFilter(obj, event);
                }
        switch (event->type()) {
            case QEvent::MouseButtonPress: {
                if (brushActive || eraserActive) {
                    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
                    previousPoint = view->mapToScene(mouseEvent->pos());
                    return true;
                }
                break;
            }
            case QEvent::MouseMove: {
                if (brushActive && !previousPoint.isNull()) {
                    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
                    QPointF currentPoint = view->mapToScene(mouseEvent->pos());

                    QLineF newLine(previousPoint, currentPoint);
                    QGraphicsLineItem *lineItem = new QGraphicsLineItem(newLine);
                    lineItem->setPen(pen);
                    scene->addItem(lineItem);

                    previousPoint = currentPoint;
                    return true;
                }
                else if (eraserActive) {
                    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
                    QPointF scenePos = view->mapToScene(mouseEvent->pos());

                    // Используем toolSize для области стирания
                    qreal eraserSize = toolSize;
                    QRectF eraseArea(
                        scenePos.x() - eraserSize/2,
                        scenePos.y() - eraserSize/2,
                        eraserSize,
                        eraserSize
                    );

                    // Получаем все элементы в области стирания
                    QList<QGraphicsItem*> itemsToRemove = scene->items(eraseArea);

                    // Удаляем все элементы, кроме стен
                    for (QGraphicsItem* item : itemsToRemove) {
                        // Проверяем, не является ли элемент стеной
                        QGraphicsRectItem* rectItem = qgraphicsitem_cast<QGraphicsRectItem*>(item);
                        if (rectItem) {
                            // Если это прямоугольник (стена), проверяем его положение
                            QRectF rect = rectItem->rect();
                            QPointF pos = rectItem->pos();
                            if (pos.x() <= 5 || pos.x() >= scene->width() - 10 ||
                                pos.y() <= 5 || pos.y() >= scene->height() - 10) {
                                continue; // Пропускаем стены
                            }
                        }
                        scene->removeItem(item);
                        delete item;
                    }
                    return true;
                }
                break;
            }
            case QEvent::MouseButtonRelease: {
                if (brushActive || eraserActive) {
                    previousPoint = QPointF();
                    return true;
                }
                break;
            }
            default:
                break;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void GraphicsEditor::addMovingObject() {
    // Открыть диалог выбора файла для изображения
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (!fileName.isEmpty()) {
        // Загружаем изображение из выбранного файла
        QPixmap pixmap(fileName);

        if (!pixmap.isNull()) {
            // Создаем объект с выбранным изображением и начальной скоростью
            MovingObject *object = new MovingObject(scene, pixmap, 15, 15);  // Задаем начальную скорость
            scene->addItem(object);

            // Настроим таймер для перемещения объекта
            QTimer *timer = new QTimer(this);
            connect(timer, &QTimer::timeout, object, &MovingObject::move);
            timer->start(30);  // Обновление каждую 50 миллисекунд
        } else {
            // Если изображение не удалось загрузить
            QMessageBox::warning(this, tr("Load Error"), tr("Failed to load image."));
        }
    } else {
        // Если файл не был выбран
        QMessageBox::information(this, tr("No File"), tr("No image file selected."));
    }
}


