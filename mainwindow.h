#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QBitmap>
#include <QPicture>
#include <QPixmap>

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
    QImage * MainWindow::greyScale(QImage * origin);

private slots:
    void on_imgbtn_clicked();

    void on_savebtn_clicked();

    void on_verticalSlider_valueChanged(int value);

    void on_greybtn_clicked();

    void on_undobtn_clicked();

    void on_redobtn_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap pixmap;
    QImage *Image;
    QImage *stepbackup[10];
    int currentstep=0;

};

#endif // MAINWINDOW_H
