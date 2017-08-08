#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("GrayScale"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString FilePath;
    Image = new QImage();
    FilePath = QFileDialog::getOpenFileName(this, tr("Open File"), "/", tr("Images (*.png *.jpg)"));
    Image->load(FilePath);
    pixmap = QPixmap::fromImage(*Image);
    ui->label->setPixmap(pixmap.scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio));
}

void MainWindow::on_pushButton_2_clicked()
{
    QImage test=*Image;
    QRgb val;
    QColor oldColor;
    for(int x = 0;x<test.width();x++){
        for(int y = 0; y<test.height();y++){
            oldColor=QColor(test.pixel(x,y));
            int ave=(oldColor.red()+oldColor.green()+oldColor.blue())/3;
            val=qRgb(ave,ave,ave);
            test.setPixel(x,y,val);
        }
    }
    *Image=test;
    //ui->label->setFixedSize(test.size());
    pixmap = QPixmap::fromImage(test);
    ui->label->setPixmap(pixmap.scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio));
    statusBar()->showMessage(tr("GrayScale is down"));
}

void MainWindow::on_pushButton_3_clicked()
{
    QString ImagePath;
    ImagePath =QFileDialog::getSaveFileName(this, tr("Save File"), "/", tr("PNG(*.png);;JPG(*.jpg)"));
    *Image=pixmap.toImage();
    Image->save(ImagePath);
}
