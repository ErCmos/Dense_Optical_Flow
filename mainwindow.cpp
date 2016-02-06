#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include "opencv2/videoio.hpp"
//#include "opencv2/features2d.hpp"
//#include "opencv2/xfeatures2d.hpp"
#include <opencv2/opencv.hpp>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->StartFrame->setInputMask("9");
    ui->EndFrame->setInputMask("9");
    if (ui->ParametrosCheckBox->isChecked())
        ui->Parametros->setEnabled(true);
    else
        ui->Parametros->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
Video_Player video_player;
cv::VideoCapture player;

void MainWindow::on_OpenVideo_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Video"), "/home/ercmos", tr("Video Files (*.avi *.mpg *.mp4 *.*)"));

    ui->VideoFile->setText(fileName);

    //OCV_Capturer Capturer;
    //cv::VideoCapture cap=Capturer.OpenVideoFile(fileName.toStdString());
    player=video_player.Capturer(fileName.toStdString());
    video_player.Player(player,"Vídeo");
    video_player.Player_DOF(fileName.toStdString(),"Vídeo_DOF");
}

void MainWindow::on_ShowTrack_clicked()
{

}

void MainWindow::on_ParametrosCheckBox_clicked()
{
    if (ui->ParametrosCheckBox->isChecked())
        ui->Parametros->setEnabled(true);
    else
        ui->Parametros->setEnabled(false);
}

void MainWindow::on_ProcesarButton_clicked()
{
    DenseTrack procesar;
    if (ui->ParametrosCheckBox->isChecked())
    {
        procesar.Tracker(ui->ShowTrack->isChecked(),ui->VideoFile->text().toStdString(),ui->Parametros)
    }
    procesar.Tracker(ui->ShowTrack->isChecked(),ui->VideoFile->text().toStdString(),ui->ParametrosCheckBox->isChecked(),ui->StartFrame->text().toInt(),ui->EndFrame->text().toInt());
}
