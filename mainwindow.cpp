#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QImage>
#include<QPixmap>
#include<QMessageBox>
#include<QFileDialog>
#include<QTextCodec>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::setLablePixmapWithMat:使用openCV的图像来设置lable，缩放
 * @param lable
 * @param image
 */
void MainWindow::setLablePixmapWithMat (QLabel *lable,Mat image)
{
    QImage img=QImage((const unsigned char*)(image.data),
                      image.cols,
                      image.rows,
                      image.cols*image.channels (),
                      QImage::Format_RGB888);
    img=img.scaled (lable->size ());
    lable->clear ();
    lable->setPixmap (QPixmap::fromImage (img));
}

void MainWindow::setLablePixmapWithGreyMat (QLabel *lable,Mat image)
{
    cv::resize (image,image,Size(lable->width (),lable->height ()),0,0,3);
    QImage img=QImage((const unsigned char*)(image.data),
                      image.cols,
                      image.rows,
                      image.step,
                      QImage::Format_Indexed8);
    img=img.scaled (lable->size ());
    lable->clear ();
    lable->setPixmap (QPixmap::fromImage (img));
}

/**
 * @brief MainWindow::setLablePixmapWithMatNoScale:使用openCV的图像来设置lable，不缩放
 * @param lable
 * @param image
 */
void MainWindow::setLablePixmapWithMatNoScale (QLabel *lable,Mat image)
{
    QImage img=QImage((const unsigned char*)(image.data),
                      image.cols,
                      image.rows,
                      image.cols*image.channels (),
                      QImage::Format_RGB888);
    lable->clear ();
    lable->setPixmap (QPixmap::fromImage (img));
}

void MainWindow::setLablePixmapWithGreyMatNoScale (QLabel *lable,Mat image)
{
    QImage img=QImage((const unsigned char*)(image.data),
                      image.cols,
                      image.rows,
                      image.step,
                      QImage::Format_Indexed8);
    lable->clear ();
    lable->setPixmap (QPixmap::fromImage (img));
}

/**
 * @brief MainWindow::showWarningMessageBox:显示提示框
 * @param title
 * @param text
 */
void MainWindow::showWarningMessageBox(QString title,QString text)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle (title);
    msgBox.setText (text);
    msgBox.exec ();
}

/**
 * @brief MainWindow::on_action_Open_TestImage_triggered：打开测试文件
 */
void MainWindow::on_action_Open_TestImage_triggered()
{
    QString fileName=QFileDialog::getOpenFileName (this,tr("选择测试文件"),"/Users/zhangxiaomin",tr("Image Files(*.bmp *.jpg *.jpeg *.png)"));
    if(!fileName.isEmpty ())//读到文件了
    {
        QTextCodec *codec=QTextCodec::codecForName ("gb18030");
        std::string name=codec->fromUnicode (fileName).data ();
        this->testImage=imread (name);
        if(this->testImage.data)
        {
            cvtColor (this->testImage,this->testImage,CV_BGR2RGB);
            this->setLablePixmapWithMat (ui->testImage,this->testImage);
        }
        else
        {
            this->showWarningMessageBox (tr("错误"),tr("OpenCV读取文件不成功"));
        }
    }
    else //没有读到文件
    {
        this->showWarningMessageBox (tr("测试文件不存在"),tr("所选择的测试文件不存在，或者文件已损坏无法读取"));
    }
}

/**
 * @brief MainWindow::on_action_Restore_triggered:还原
 */
void MainWindow::on_action_Restore_triggered()
{
    this->testImage.copyTo (this->destImage);
    this->setLablePixmapWithMat (ui->dstImage,this->destImage);
}

/**
 * @brief MainWindow::on_actionClear_triggered:清除
 */
void MainWindow::on_actionClear_triggered()
{
    ui->testImage->clear ();
    ui->dstImage->clear ();
}

/**
 * @brief MainWindow::on_actionExit_triggered:退出程序
 */
void MainWindow::on_actionExit_triggered()
{
    qApp->exit (0);
}


/**
 *
 * @brief MainWindow::on_actionSobel_triggered:Sobel边缘检测
 */
void MainWindow::on_actionSobel_triggered()
{
    //Sobel算子
    cv::Sobel (this->testImage,this->destImage,this->testImage.depth (),0,1);
    this->setLablePixmapWithMat (ui->dstImage,this->destImage);
}

/**
 * @brief MainWindow::on_actionCanny_triggered:Canny算子
 */
void MainWindow::on_actionCanny_triggered()
{
    //Canny算子
    Mat greyImg;
    cv::cvtColor (this->testImage,greyImg,COLOR_RGB2GRAY);
    cv::Canny (greyImg,greyImg,30,100);
    this->setLablePixmapWithGreyMat (ui->dstImage,greyImg);
}

/**
 * @brief MainWindow::on_actionLaplacian_triggered:Laplacian算子
 */
void MainWindow::on_actionLaplacian_triggered()
{
    cv::Laplacian (this->testImage,this->destImage,this->testImage.depth (),31);
    this->setLablePixmapWithMat (ui->dstImage,this->destImage);
}

/**
 * @brief MainWindow::on_actionScharr_triggered:Scharr算子
 */
void MainWindow::on_actionScharr_triggered()
{
   cv::Scharr (this->testImage,this->destImage,this->testImage.depth (),0,1);
   this->setLablePixmapWithMat (ui->dstImage,this->destImage);
}
