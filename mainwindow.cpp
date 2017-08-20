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
    undo_redo_enable();             //每執行一次修改偵測一次有沒有復原和重做
}

void MainWindow::undo_redo_enable(){

    if(currentstep>0)
        ui->actionUndo->setDisabled(false);
    else
        ui->actionUndo->setDisabled(true);

    if(Image[currentstep+1]!=NULL)
        ui->actionRedo->setDisabled(false);
    else
        ui->actionRedo->setDisabled(true);

}


MainWindow::~MainWindow()
{
    delete ui;
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


    Image[currentstep+1]=greyScale(Image[currentstep]);
    pixmap = QPixmap::fromImage(*Image[currentstep+1]);
    ui->label->setPixmap(pixmap.scaled(ui->label->width(),ui->label->height()));
    currentstep++;

    undo_redo_enable();
}



void MainWindow::on_actionOpen_File_triggered()
{
    //載入圖片到lable
    *Image=new QImage;
    QString imgpath=QFileDialog::getOpenFileName(this, tr("Open File"),"/",tr("Image *.png *.jpg"));
    Image[0]->load(imgpath);

    pixmap = QPixmap::fromImage(*Image[0]);
    ui->label->setPixmap(pixmap.scaled(ui->label->width(),ui->label->height()));
    ui->greybtn->setEnabled(true);
}

void MainWindow::on_actionSave_File_triggered()
{
    if(Image[currentstep]==NULL){
        QMessageBox msgBox;
        msgBox.setText("The image hasn't been loaded.");
        msgBox.exec();

    }

    else{
        QString Simgpath=QFileDialog::getSaveFileName(this, tr("Save File"),"/",tr("PNG(*.png);;JPG(*.jpg)"));
        *Image[currentstep]=pixmap.toImage();
        Image[currentstep]->save(Simgpath);
    }
}

void MainWindow::on_actionUndo_triggered()
{

    currentstep--;

    pixmap = QPixmap::fromImage(*Image[currentstep]);
    ui->label->setPixmap(pixmap.scaled(ui->label->width(),ui->label->height()));
    undo_redo_enable();
}

void MainWindow::on_actionRedo_triggered()
{

    currentstep++;

    pixmap = QPixmap::fromImage(*Image[currentstep]);
    ui->label->setPixmap(pixmap.scaled(ui->label->width(),ui->label->height()));

    undo_redo_enable();
}
