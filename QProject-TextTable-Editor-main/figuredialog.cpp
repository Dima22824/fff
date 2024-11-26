#include "figuredialog.h"
#include "ui_figuredialog.h"

FigureDialog::FigureDialog(QGraphicsScene *scene, QGraphicsItem *currentItem, QWidget *parent) :
    QDialog(parent), editorScene(scene), currentItem(currentItem),
    ui(new Ui::FigureDialog)
{
    ui->setupUi(this);
    setWindowTitle("Add Figure");

    // Создание сцены
    previewScene = new QGraphicsScene(this);

    // Привязка сцены к previewView
    ui->graphicsView->setScene(previewScene);

    // Создание FigureTools
    FigureTools = new FigureTool(previewScene);

    // Кнопка для выбора цвета заливки
    connect(ui->FillButton, &QPushButton::clicked, this, [this]() {
        FigureTools->fillColor(currentPreviewItem);
    });

    // Кнопка для изменения размера фигуры
    connect(ui->resizeButton, &QPushButton::clicked, this, [this]() {
        FigureTools->resizeShape(currentPreviewItem);
    });


    connect(ui->okButton, &QPushButton::clicked, this, &FigureDialog::acceptFigure);
    connect(ui->resetButton, &QPushButton::clicked, this, &FigureDialog::resetSettings);

    // Обновление превью при смене фигуры
    connect(ui->FigureSelector, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), this,
            &FigureDialog::updatePreview);

    // Инициализируем превью
    updatePreview();
}

FigureDialog::~FigureDialog()
{
    delete ui;
}

void FigureDialog::acceptFigure()
{
    if (!currentPreviewItem) return;
    currentItem = currentPreviewItem;
    if (currentItem) {
        auto dragAbstructItem = new DraggableItem(currentItem, ui->FigureSelector->currentText());
        auto dragItem = dragAbstructItem->dragItem;
        dragItem->setPos(100, 100);
        editorScene->addItem(dragItem);
    }
    close();
}

void FigureDialog::resetSettings()
{
    fillColor = Qt::white;
    ui->FigureSelector->setCurrentIndex(0);
    updatePreview();
}

void FigureDialog::updatePreview()
{
    previewScene->clear();
    currentPreviewItem = nullptr;

    QString figureType = ui->FigureSelector->currentText();

    if (figureType == "Квадрат") {
        currentPreviewItem = FigureTools->addRectangle();
    } else if (figureType == "Круг") {
        currentPreviewItem = FigureTools->addEllipse();
    } else if (figureType == "Треугольник") {
        QPolygonF triangle;
        triangle << QPointF(75, 25) << QPointF(125, 75) << QPointF(25, 75);
        currentPreviewItem = FigureTools->addTriangle();
    }else if (figureType == "Текст"){
        QString text = QInputDialog::getText(this, "Введите текст", "Текст:");
        currentPreviewItem = FigureTools->addText(text);
    }
}
