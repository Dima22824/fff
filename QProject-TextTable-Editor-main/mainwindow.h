#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNewFile();
    void onOpenFile();
    void onSaveFile();
    void onClearText();
    void onRestoreText();
    void showAuthors();
    void showHelp();
    void findText();
    void replaceText();
    void copyText();
    void pasteText();
    void onDefaultSettings();
    void onSaveTable();
    void onOpenTable();



    void on_addRowButton_clicked();


    void on_removeRowButton_clicked();

    void on_addColButton_clicked();

    void on_removeColButton_clicked();

    void on_boldButton_clicked();

    void on_italicButton_clicked();

    void on_strikeoutButton_clicked();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_spinFontSize_valueChanged(int arg1);

    void on_colorButton_clicked();

    void saveSettings();

    void loadSettings();

private:
    Ui::MainWindow *ui;
    QString tempFilePath = "temp.txt";

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
