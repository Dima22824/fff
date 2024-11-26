#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GraphicsEditor.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QInputDialog>
#include <QCloseEvent>
#include <QColorDialog>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    GraphicsEditor *graphicsEditor = new GraphicsEditor;
    ui->tabWidget->addTab(graphicsEditor, "Graphical Editor");
    loadSettings();

    // Привязываем действия из меню к слотам
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewFile);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenFile);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveFile);
    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::onClearText);
    connect(ui->actionReturn, &QAction::triggered, this, &MainWindow::onRestoreText);
    connect(ui->actionAuthors, &QAction::triggered, this, &MainWindow::showAuthors);
    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::showHelp);
    connect(ui->actionFind, &QAction::triggered, this, &MainWindow::findText);
    connect(ui->actionReplace, &QAction::triggered, this, &MainWindow::replaceText);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::copyText);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::pasteText);
    connect(ui->actionSaveTable, &QAction::triggered, this, &MainWindow::onSaveTable);
    connect(ui->actionOpenTable, &QAction::triggered, this, &MainWindow::onOpenTable);
    connect(ui->actionDefault, &QAction::triggered, this, &MainWindow::onDefaultSettings);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Функция для создания нового файла
void MainWindow::onNewFile()
{
    ui->textEdit->clear();
}

// Функция для сохранения файла
void MainWindow::onSaveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), "", tr("HTML файлы (*.html);;Все файлы (*)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream out(&file);
            out << ui->textEdit->toHtml(); // Сохраняем как HTML
            file.close();
        }
    }
}


// Функция для открытия файла
void MainWindow::onOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "", tr("HTML файлы (*.html);;Все файлы (*)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&file);
            ui->textEdit->setHtml(in.readAll()); // Открываем как HTML
            file.close();
        }
    }
}

// Функция для сохранения таблицы
void MainWindow::onSaveTable()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить таблицу"), "", tr("HTML файлы (*.html);;Все файлы (*)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream out(&file);

            // Начало HTML таблицы
            out << "<html><body><table border=\"1\">";

            int rowCount = ui->tableWidget->rowCount();
            int columnCount = ui->tableWidget->columnCount();

            // Сохранение каждой строки и колонки таблицы
            for (int row = 0; row < rowCount; ++row) {
                out << "<tr>";  // Начало строки
                for (int column = 0; column < columnCount; ++column) {
                    QTableWidgetItem *item = ui->tableWidget->item(row, column);
                    QString cellText = item ? item->text() : "";

                    // Извлечение стилей ячейки
                    QString fontStyle;
                    if (item) {
                        QFont font = item->font();
                        QColor textColor = item->foreground().color();

                        fontStyle = "style='";

                        // Устанавливаем жирный шрифт
                        if (font.bold()) fontStyle += "font-weight:bold;";
                        // Курсив
                        if (font.italic()) fontStyle += "font-style:italic;";
                        // Зачеркивание
                        if (font.strikeOut()) fontStyle += "text-decoration:line-through;";
                        // Цвет текста
                        fontStyle += "color:" + textColor.name() + ";";
                        // Сохранение шрифта
                        fontStyle += "font-family:" + font.family() + ";";
                        // Сохранение размера шрифта
                        fontStyle += "font-size:" + QString::number(font.pointSize()) + "pt;";
                        fontStyle += "'";
                    }

                    // Сохранение ячейки с форматированием
                    out << "<td " << fontStyle << ">" << cellText << "</td>";
                }
                out << "</tr>";  // Конец строки
            }

            out << "</table></body></html>";
            file.close();
        }
    }
}


// Функция для открытия8 таблицы
void MainWindow::onOpenTable()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть таблицу"), "", tr("HTML файлы (*.html);;Все файлы (*)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&file);
            QString htmlContent = in.readAll();

            // Парсинг HTML для заполнения таблицы
            QRegularExpression rowRegex("<tr>(.+?)</tr>");
            QRegularExpression cellRegex("<td.*?>(.*?)</td>");
            QRegularExpression styleRegex("style='(.*?)'");

            ui->tableWidget->setRowCount(0);
            QRegularExpressionMatchIterator rowMatchIterator = rowRegex.globalMatch(htmlContent);

            while (rowMatchIterator.hasNext()) {
                QRegularExpressionMatch rowMatch = rowMatchIterator.next();
                QString rowData = rowMatch.captured(1);
                QRegularExpressionMatchIterator cellMatchIterator = cellRegex.globalMatch(rowData);
                int currentRowCount = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(currentRowCount);
                int column = 0;

                while (cellMatchIterator.hasNext()) {
                    QRegularExpressionMatch cellMatch = cellMatchIterator.next();
                    QString cellText = cellMatch.captured(1);

                    // Извлечение стилей ячейки
                    QRegularExpressionMatch styleMatch = styleRegex.match(cellMatch.captured(0));
                    QTableWidgetItem *newItem = new QTableWidgetItem(cellText);

                    if (styleMatch.hasMatch()) {
                        QString styleData = styleMatch.captured(1);

                        // Применение стилей (жирный, курсив, зачеркивание, цвет)
                        if (styleData.contains("font-weight:bold")) newItem->setFont(QFont("", -1, QFont::Bold));
                        if (styleData.contains("font-style:italic")) newItem->setFont(QFont("", -1, QFont::Normal, true));
                        if (styleData.contains("text-decoration:line-through")) {
                            QFont font = newItem->font();
                            font.setStrikeOut(true);
                            newItem->setFont(font);
                        }
                        if (styleData.contains("font-size:")) {
                            QFont font = newItem->font();
                            font.setStrikeOut(true);
                            newItem->setFont(font);
                        }
                        QFont font;
                        // Извлечение размера шрифта
                        QRegularExpression fontSizeRegex("font-size:(\\d+)pt");
                        QRegularExpressionMatch fontSizeMatch = fontSizeRegex.match(styleData);
                        if (fontSizeMatch.hasMatch()) {
                            int fontSize = fontSizeMatch.captured(1).toInt();
                            font.setPointSize(fontSize);
                        }

                        // Извлечение шрифта
                        QRegularExpression fontFamilyRegex("font-family:([^;]+);");
                        QRegularExpressionMatch fontFamilyMatch = fontFamilyRegex.match(styleData);
                        if (fontFamilyMatch.hasMatch()) {
                            QString fontFamily = fontFamilyMatch.captured(1);
                            font.setFamily(fontFamily);
                        }

                        newItem->setFont(font);
                        QRegularExpression colorRegex("color:(#[0-9a-fA-F]{6})");
                        QRegularExpressionMatch colorMatch = colorRegex.match(styleData);
                        if (colorMatch.hasMatch()) {
                            QColor textColor(colorMatch.captured(1));
                            newItem->setForeground(textColor);
                        }
                    }

                    ui->tableWidget->setItem(currentRowCount, column, newItem);
                    column++;
                }
            }
            file.close();
        }
    }
}



void MainWindow::onClearText()
{
    QFile file(tempFilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << ui->textEdit->toPlainText(); // Сохраняем текст в файл
        file.close();
    }

    ui->textEdit->clear(); // Очищаем текстовый редактор
}

void MainWindow::onRestoreText()
{
    QFile file(tempFilePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString text = in.readAll(); // Читаем содержимое временного файла
        ui->textEdit->setPlainText(text); // Восстанавливаем текст в редакторе
        file.close();
    } else {
        QMessageBox::warning(this, tr("Ошибка"), tr("Невозможно открыть временный файл."));
    }
}

void MainWindow::findText()
{
    bool ok;
    QString searchText = QInputDialog::getText(this, tr("Найти"), tr("Введите текст для поиска:"), QLineEdit::Normal, "", &ok);

    if (ok && !searchText.isEmpty()) {
        if (!ui->textEdit->find(searchText)) {
            QMessageBox::information(this, tr("Поиск"), tr("Текст не найден"));
        }
    }
}

void MainWindow::replaceText()
{
    bool ok;
    QString searchText = QInputDialog::getText(this, tr("Найти"), tr("Введите текст для поиска:"), QLineEdit::Normal, "", &ok);

    if (ok && !searchText.isEmpty()) {
        QString replaceText = QInputDialog::getText(this, tr("Заменить"), tr("Заменить на:"), QLineEdit::Normal, "", &ok);

        if (ok && !replaceText.isEmpty()) {
            ui->textEdit->moveCursor(QTextCursor::Start);
            while (ui->textEdit->find(searchText)) {
                ui->textEdit->textCursor().insertText(replaceText);
            }
        }
    }
}

void MainWindow::copyText()
{
    ui->textEdit->copy();
}

void MainWindow::pasteText()
{
    ui->textEdit->paste();
}

void MainWindow::onDefaultSettings(){

    int font_size = 10;
    QColor color = "Black";
    int row_count = 5;
    int column_count = 5;
    QString font_family = "MS Shell Dlg 2";

    ui->textEdit->selectAll();
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat format;

    format.setFontWeight(QFont::Normal);
    format.setFontItalic(false);
    format.setFontStrikeOut(false);
    format.setFontPointSize(font_size);
    format.setForeground(color);
    cursor.mergeCharFormat(format);
    ui->textEdit->setFontFamily(font_family);

    ui->fontComboBox->setCurrentFont(font_family);
    ui->spinFontSize->setValue(font_size);

    if (ui->tableWidget->rowCount() > row_count){
        for (int i = ui->tableWidget->rowCount(); i >= row_count; --i){
            ui->tableWidget->removeRow(i);
        }
    }else{
        for (int i = ui->tableWidget->rowCount(); i < row_count; ++i){
            ui->tableWidget->insertRow(i);
        }
    }

    if (ui->tableWidget->columnCount() > column_count){
        for (int i = ui->tableWidget->columnCount(); i >= column_count; --i){
            ui->tableWidget->removeColumn(i);
        }
    }else{
        for (int i = ui->tableWidget->columnCount(); i < column_count; ++i){
            ui->tableWidget->insertColumn(i);
        }
    }
}

void MainWindow::showAuthors()
{
    QMessageBox::information(this, "Авторы", "Сивов Семен\n"
                                             "Швецов Павел\n"
                                             "Иванов Максим\n"
                                             "ИП-215");
}

void MainWindow::showHelp()
{
    QMessageBox::information(this, "Помощь", "Это базовый текстовый и табличный редактор.\n\nИспользуйте меню 'Файл' для создания, открытия, сохранения файлов.\nИспользуйте меню 'Инструменты' для дополнительных действий.");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    // Проверяем, были ли изменения в тексте
    if (ui->textEdit->document()->isModified()) {
        QMessageBox::StandardButton resBtn = QMessageBox::question(this, tr("Закрыть"),
            tr("Документ был изменен.\n"
               "Хотите сохранить изменения?"),
            QMessageBox::No | QMessageBox::Yes,
            QMessageBox::Yes);
        if (resBtn == QMessageBox::Yes) {
            onSaveFile();
        }
    }
    event->accept(); // Закрываем окно
}

void MainWindow::loadSettings() {
    QSettings settings("Text_table_editor", "MyApp");

    QString font_family = settings.value("Text_editor/font", "MS Shell Dlg 2").toString();
    int font_size = settings.value("Text_editor/font_size", 10).toInt();
    bool is_bold = settings.value("Text_editor/bold", false).toBool();
    bool is_italic = settings.value("Text_editor/italic", false).toBool();
    bool is_strike_out = settings.value("Text_editor/strike-out", false).toBool();
    QColor color = settings.value("Text_editor/color", "black").value<QColor>();
    int row_count = settings.value("Table_settings/rows", 5).toInt();
    int column_count = settings.value("Table_settings/columns", 5).toInt();

    ui->textEdit->selectAll();
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat format;

    ui->textEdit->setFontFamily(font_family);
    ui->fontComboBox->setCurrentFont(font_family);
    ui->spinFontSize->setValue(font_size);
    format.setFontPointSize(font_size);
    format.setFontWeight(is_bold == false ? QFont::Normal : QFont::Bold);
    format.setFontItalic(is_italic);
    format.setFontStrikeOut(is_strike_out);
    format.setForeground(color);

    cursor.mergeCharFormat(format);

    if (ui->tableWidget->rowCount() > row_count){
        for (int i = ui->tableWidget->rowCount(); i >= row_count; --i){
            ui->tableWidget->removeRow(i);
        }
    }else{
        for (int i = ui->tableWidget->rowCount(); i < row_count; ++i){
            ui->tableWidget->insertRow(i);
        }
    }

    if (ui->tableWidget->columnCount() > column_count){
        for (int i = ui->tableWidget->columnCount(); i >= column_count; --i){
            ui->tableWidget->removeColumn(i);
        }
    }else{
        for (int i = ui->tableWidget->columnCount(); i < column_count; ++i){
            ui->tableWidget->insertColumn(i);
        }
    }

}

void MainWindow::saveSettings() {
    QSettings settings("Text_table_editor", "MyApp");

    settings.beginGroup("Table_settings");
    int rowCount = ui->tableWidget->rowCount();
    int columnCount = ui->tableWidget->columnCount();
    settings.setValue("rows", rowCount);
    settings.setValue("columns", columnCount);
    settings.endGroup();

}


//********ФУНКЦИИ СОЗДАННЫЕ ЧЕРЕЗ СЛОТЫ***************


void MainWindow::on_addRowButton_clicked()
{
    int currentRowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(currentRowCount);
}


void MainWindow::on_removeRowButton_clicked()
{
    // Получаем текущий индекс выбранной строки
    int currentRow = ui->tableWidget->currentRow();

    // Если строка выбрана удаляем её
    if (currentRow != -1) {
        ui->tableWidget->removeRow(currentRow);
    } else {
        QMessageBox::information(this, tr("Удаление строки"), tr("Пожалуйста, выберите строку для удаления."));
    }
}

void MainWindow::on_addColButton_clicked()
{
    int currentColumnCount = ui->tableWidget->columnCount();
    ui->tableWidget->insertColumn(currentColumnCount);
}

void MainWindow::on_removeColButton_clicked()
{
    // Получаем текущий индекс выбранной колонки
        int currentColumn = ui->tableWidget->currentColumn();

        // Если колонка выбрана удаляем её
        if (currentColumn != -1) {
            ui->tableWidget->removeColumn(currentColumn);
        } else {
            QMessageBox::information(this, tr("Удаление колонки"), tr("Пожалуйста, выберите колонку для удаления."));
        }
}

void MainWindow::on_boldButton_clicked()
{
    QSettings settings("Text_table_editor", "MyApp");

    // Изменение жирности текста в textEdit
    QTextCursor cursor = ui->textEdit->textCursor();
    if (cursor.hasSelection()) {
        QTextCharFormat format;
        bool is_bold = (cursor.charFormat().fontWeight() == QFont::Bold) ? 1 : 0;
        format.setFontWeight(is_bold == 1 ? QFont::Normal : QFont::Bold);
        settings.setValue("Text_editor/bold", !is_bold);
        cursor.mergeCharFormat(format);
    }

    // Изменение жирности текста в выделенных ячейках tableWidget
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    for (QTableWidgetItem* item : selectedItems) {
        if (item) {
            QFont font = item->font();
            font.setBold(!font.bold());
            settings.setValue("Text_editor/bold", !font.bold());
            item->setFont(font);
        }
    }
}

void MainWindow::on_italicButton_clicked()
{
    QSettings settings("Text_table_editor", "MyApp");

    // Изменение курсива текста в textEdit
    QTextCursor cursor = ui->textEdit->textCursor();
    if (cursor.hasSelection()) {
        QTextCharFormat format;
        bool flag = !cursor.charFormat().fontItalic();
        format.setFontItalic(flag);
        settings.setValue("Text_editor/italic", flag);
        cursor.mergeCharFormat(format);
    }

    // Изменение курсива текста в выделенных ячейках tableWidget
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    for (QTableWidgetItem* item : selectedItems) {
        if (item) {
            QFont font = item->font();
            bool flag = !font.italic();
            font.setItalic(flag);
            settings.setValue("Text_editor/italic", flag);
            item->setFont(font);
        }
    }
}

void MainWindow::on_strikeoutButton_clicked()
{
    QSettings settings("Text_table_editor", "MyApp");
    bool flag = 0;
    // Изменение зачеркнутого текста в textEdit
        QTextCursor cursor = ui->textEdit->textCursor();
        if (cursor.hasSelection()) {
            QTextCharFormat format;
            flag = !cursor.charFormat().fontStrikeOut();
            format.setFontStrikeOut(flag);
            settings.setValue("Text_editor/strike-out", flag);
            cursor.mergeCharFormat(format);
        }

        // Изменение зачеркнутого текста в выделенных ячейках tableWidget
        QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
        for (QTableWidgetItem* item : selectedItems) {
            if (item) {
                QFont font = item->font();
                flag = !font.strikeOut();
                font.setStrikeOut(flag);
                settings.setValue("Text_editor/strike-out", flag);
                item->setFont(font);
            }
        }
}

void MainWindow::on_fontComboBox_currentFontChanged(const QFont &fontName)
{
    QSettings settings("Text_table_editor", "MyApp");
    // Изменение шрифта в textEdit
       QFont font(fontName);
       ui->textEdit->setCurrentFont(font);

       // Изменение шрифта в выделенных ячейках tableWidget
       QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
       for (QTableWidgetItem* item : selectedItems) {
           if (item) {
               item->setFont(font);
           }
       }
       settings.setValue("Text_editor/font", fontName);
}

// Слот для обновления размера шрифта
void MainWindow::on_spinFontSize_valueChanged(int arg1)
{
    QSettings settings("Text_table_editor", "MyApp");

    QTextCursor cursor = ui->textEdit->textCursor();
    if (cursor.hasSelection()) {
        QTextCharFormat format;
        format.setFontPointSize(arg1);
        cursor.mergeCharFormat(format);
    }

    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    for (QTableWidgetItem* item : selectedItems) {
        if (item) {
            QFont font = item->font();
            font.setPointSize(arg1);
            item->setFont(font);
        }
    }
    settings.setValue("Text_editor/font_size", arg1);
}

void MainWindow::on_colorButton_clicked()
{
    QSettings settings("Text_table_editor", "MyApp");

    QColor color = QColorDialog::getColor(Qt::white, this, "Выбор цвета текста");
    if (color.isValid()) {
        QTextCharFormat format;
        format.setForeground(color);
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.mergeCharFormat(format);

        QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
        for (QTableWidgetItem* item : selectedItems) {
            if (item) {
                item->setTextColor(color);
            }
        }
        settings.setValue("Text_editor/color", color);
    }
}
