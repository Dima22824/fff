#ifndef FIGUREDIALOG_H
#define FIGUREDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QColorDialog>
#include <unordered_map>
#include "figuretool.h"
#include "draggableitem.h"

namespace Ui {
class FigureDialog;
}

class FigureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FigureDialog(QGraphicsScene *scene,QGraphicsItem *currentItem, QWidget *parent = nullptr);
    ~FigureDialog();

private slots:
    void acceptFigure();   // Добавление фигуры на сцену
    void resetSettings();  // Сброс настроек
    void updatePreview();  // Обновление превью


private:
    Ui::FigureDialog *ui;
    QGraphicsScene *editorScene;
    QGraphicsView *previewView;
    QGraphicsScene *previewScene;
    QGraphicsItem *currentItem;
    QGraphicsItem *currentPreviewItem;
    QColor fillColor;
    FigureTool *FigureTools;
};

#endif // FIGUREDIALOG_H
