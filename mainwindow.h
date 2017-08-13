#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QMainWindow>
#include <QImage>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPixmap>
#include <QPainter>
#include <QBitmap>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    QImage

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap pixmap;
    QImage *Image;
};

#endif // MAINWINDOW_H
