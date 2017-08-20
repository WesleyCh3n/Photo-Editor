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


    this->setFixedSize(1000,600);
    /*this->setStyleSheet(
                "background-image:url(https://up-1.cdn-fullscreendirect.com/production/photos/10393/large/20170214_211843_10393_968993.jpeg);background-position: center;");*/



    //QPixmap使用底層平台的繪製系統進行繪製，無法提供像素級別的操作，而QImage則是獨立於硬體的繪製系統，
    //因此可以用setPixel()函式，對影像的像素進行設置，且QImage在不同系統上有相同的影像。
    //bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);


    /*  QPalette palette;   //調色盤
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
*/




    undo_redo_enable();             //每執行一次修改偵測一次有沒有復原和重做
    //設置滑動條控件的最小值
    ui->BrightnessSlider->setMinimum(-60);
    //設置滑動條控件的最大值
    ui->BrightnessSlider->setMaximum(60);
    //設置滑動條控件的值
    ui->BrightnessSlider->setValue(0);
    ui->greybtn->setEnabled(false);
    ui->BrightnessSlider->setEnabled(false);
    ui->brightnessset_btn->setEnabled(false);
}

void MainWindow::undo_redo_enable(){

    if(currentstep>0)
        ui->actionUndo->setEnabled(true);
    else
        ui->actionUndo->setEnabled(false);

    if(Image[currentstep+1]!=NULL)
        ui->actionRedo->setEnabled(true);
    else
        ui->actionRedo->setEnabled(false);

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_BrightnessSlider_valueChanged(int value)
{
    ui->sliderlabel->setText(QString::number(value));
    brightnessdelta=value;
    brightnessScale(0,brightnessdelta,Image[currentstep]);
}


//-----------------------------------圖片處理區


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

QImage *MainWindow::brightnessScale(bool set, int value, QImage *origin){
    QImage *newImage = new QImage(origin->width(), origin->height(), QImage::Format_ARGB32);

    QColor oldColor;

    for(int x = 0; x<newImage->width(); x++){
        for(int y = 0; y<newImage->height(); y++){
            oldColor = QColor(origin->pixel(x,y));
            if(oldColor.red()+value>=0&&oldColor.red()+value<=255&&
                    oldColor.green()+value>=0&&oldColor.green()+value<=255&&
                    oldColor.blue()+value>=0&&oldColor.blue()+value<=255)
            {
                newImage->setPixel(x,y,qRgb(oldColor.red()+value,oldColor.green()+value,oldColor.blue()+value));

            }
            else
                newImage->setPixel(x,y,qRgb(oldColor.red(),oldColor.green(),oldColor.blue()));
        }
    }

    if(set==0){
        pixmap = QPixmap::fromImage(*newImage);
        ui->label->setPixmap(pixmap.scaled(ui->label->width(),ui->label->height()));
    }

    else if(set==1)
        return newImage;
}


//------------------------------------按鍵區

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

    for(int i=0;i<100;i++)
    {Image[i]={NULL};}            //每開一次新檔重置一次
    *Image=new QImage;            //宣告整個指標陣列都是QImage?
    currentstep=0;

    //開啟路徑
    QString imgpath=QFileDialog::getOpenFileName(this, tr("Open File"),"/",tr("Image *.png *.jpg"));
    Image[0]->load(imgpath);

    //載入圖片到lable
    pixmap = QPixmap::fromImage(*Image[0]);
    ui->label->setPixmap(pixmap.scaled(ui->label->width(),ui->label->height()));

    ui->greybtn->setEnabled(true);
    ui->BrightnessSlider->setEnabled(true);
    ui->brightnessset_btn->setEnabled(true);

    undo_redo_enable();
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

void MainWindow::on_brightnessset_btn_clicked()
{
    Image[currentstep+1]=brightnessScale(1,brightnessdelta,Image[currentstep]);
    pixmap = QPixmap::fromImage(*Image[currentstep+1]);
    ui->label->setPixmap(pixmap.scaled(ui->label->width(),ui->label->height()));
    currentstep++;

    undo_redo_enable();
}
