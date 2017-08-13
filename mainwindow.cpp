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

QImage *MainWindow::blur(QImage *origin){
    QImage *newImage = new QImage(*origin);

    int kernel [5][5]= {{0,0,1,0,0},
                        {0,1,3,1,0},
                        {1,3,7,3,1},
                        {0,1,3,1,0},
                        {0,0,1,0,0}};
    int kernelSize = 5;
    int sumKernel = 27;
    int r,g,b;
    QColor color;

    for(int x=kernelSize/2; x<newImage->width()-(kernelSize/2); x++){
        for(int y=kernelSize/2; y<newImage->height()-(kernelSize/2); y++){

            r = 0;
            g = 0;
            b = 0;

            for(int i = -kernelSize/2; i<= kernelSize/2; i++){
                for(int j = -kernelSize/2; j<= kernelSize/2; j++){
                    color = QColor(origin->pixel(x+i, y+j));
                    r += color.red()*kernel[kernelSize/2+i][kernelSize/2+j];
                    g += color.green()*kernel[kernelSize/2+i][kernelSize/2+j];
                    b += color.blue()*kernel[kernelSize/2+i][kernelSize/2+j];
                }
            }

            r = qBound(0, r/sumKernel, 255);
            g = qBound(0, g/sumKernel, 255);
            b = qBound(0, b/sumKernel, 255);

            newImage->setPixel(x,y, qRgb(r,g,b));

        }
    }
    return newImage;
}

void MainWindow::on_pushButton_2_clicked()
{



    QImage test=*Image;
    *Image=blur(test);

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
