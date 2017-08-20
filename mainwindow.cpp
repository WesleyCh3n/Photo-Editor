/*
許述文 20170731
學習QImage、read TXT file、tab的使用，以及圖片灰階處理
想把圖片處理做成一個function,還沒成功
*/
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    this->setWindowTitle(tr("GrayScale"));


    this->setFixedSize(800,600);
    /*this->setStyleSheet(
                "background-image:url(https://up-1.cdn-fullscreendirect.com/production/photos/10393/large/20170214_211843_10393_968993.jpeg);background-position: center;");*/



    //QPixmap使用底層平台的繪製系統進行繪製，無法提供像素級別的操作，而QImage則是獨立於硬體的繪製系統，
    //因此可以用setPixel()函式，對影像的像素進行設置，且QImage在不同系統上有相同的影像。
    //bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);


    /*  QPalette palette;   //調色盤
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
*/

    ui->greybtn->setEnabled(false);


}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_imgbtn_clicked()
{
    //載入圖片到lable
    Image = new QImage();
    QString imgpath=QFileDialog::getOpenFileName(this, tr("Open File"),"/",tr("Image *.png *.jpg"));
    Image->load(imgpath);

    pixmap = QPixmap::fromImage(*Image);
    ui->label->setPixmap(pixmap.scaled(ui->label->width(),ui->label->height()));
    ui->greybtn->setEnabled(true);
}

void MainWindow::on_savebtn_clicked()
{
    QString Simgpath=QFileDialog::getSaveFileName(this, tr("Save File"),"/",tr("PNG(*.png);;JPG(*.jpg)"));
    *Image=pixmap.toImage();
    Image->save(Simgpath);
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    ui->sliderlabel->setText(QString::number(value));
}

QImage *MainWindow::greyScale(QImage *origin){
    QImage *newImage = new QImage(origin->width(), origin->height(), QImage::Format_ARGB32);

    QColor oldColor;

    for(int x = 0; x<newImage->width(); x++){
        for(int y = 0; y<newImage->height(); y++){
            oldColor = QColor(origin->pixel(x,y));
            int average = (oldColor.red()+oldColor.green()+oldColor.blue())/3;
            newImage->setPixel(x,y,qRgb(average,average,average));
        }
    }

    return newImage;
}



void MainWindow::on_greybtn_clicked()
{

    stepbackup[currentstep]=Image;
    currentstep++;
    stepbackup[currentstep]=greyScale(stepbackup[currentstep-1]);
    pixmap = QPixmap::fromImage(*stepbackup[currentstep]);
    ui->label->setPixmap(pixmap.scaled(ui->label->width(),ui->label->height()));

}

void MainWindow::on_undobtn_clicked()
{
    if(currentstep>0)currentstep--;

    pixmap = QPixmap::fromImage(*stepbackup[currentstep]);
    ui->label->setPixmap(pixmap.scaled(ui->label->width(),ui->label->height()));
}

void MainWindow::on_redobtn_clicked()
{
    if(stepbackup[currentstep+1]!=NULL)currentstep++;

    pixmap = QPixmap::fromImage(*stepbackup[currentstep]);
    ui->label->setPixmap(pixmap.scaled(ui->label->width(),ui->label->height()));
}
