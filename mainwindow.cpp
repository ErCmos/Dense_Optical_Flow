#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include "opencv2/videoio.hpp"
//#include "opencv2/features2d.hpp"
//#include "opencv2/xfeatures2d.hpp"
//#include <opencv/cv.h>
//#include <opencv/highgui.h>
//#include <opencv/cxcore.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->StartFrame->setInputMask("9999999999");
    ui->EndFrame->setInputMask("9999999999");
    //QString s = QString::number(constantes::end_frame);
    ui->EndFrame->setText(QString::number(constantes::end_frame));
    ui->TrajectoryLength->setInputMask("999");
    ui->SamplingStride->setInputMask("999");
    ui->NeighborhoodSize->setInputMask("999");
    ui->SpatialCells->setInputMask("999");
    ui->TemporaryCells->setInputMask("999");
    ui->ScaleNumber->setInputMask("999");
    ui->InitialGap->setInputMask("999");
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
    //video_player.Player(player,"Vídeo");
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
        procesar.Tracker(ui->ShowTrack->isChecked(),ui->VideoFile->text().toStdString(), ui->ParametrosCheckBox->isChecked(), ui->StartFrame->text().toInt(),ui->EndFrame->text().toInt(),ui->TrajectoryLength->text().toInt(),ui->SamplingStride->text().toInt(),ui->NeighborhoodSize->text().toInt(),ui->SpatialCells->text().toInt(),ui->TrajectoryLength->text().toInt(),ui->ScaleNumber->text().toInt(),ui->InitialGap->text().toInt());
    }
    else
    {
        procesar.Tracker(ui->ShowTrack->isChecked(),ui->VideoFile->text().toStdString(), ui->ParametrosCheckBox->isChecked());
    }
//    Tracker procesar;
//    DenseTrack procesar;
//    procesar.Tracker(ui->ShowTrack->isChecked(),ui->VideoFile->text().toStdString(), ui->ParametrosCheckBox->isChecked());
}

void MainWindow::on_CrearDiccButton_clicked()
{
    QString fullName = QFileDialog::getOpenFileName(this,
            tr("Open Diccionary"), "/home/ercmos", tr("Diccionary Files (*.txt)"));

    QString dirName = QFileInfo(fullName).absolutePath();
    QString fileName = QFileInfo(fullName).fileName();

    BoW saco;
    saco.CrearDiccionario(fileName.toStdString(),dirName.toStdString());
}

void MainWindow::on_DOFDirectorioButton_clicked()
{
    QString fullName = QFileDialog::getOpenFileName(this,
            tr("Open Diccionary"), "/home/ercmos", tr("Diccionary Files (*.avi)"));

    QString dirName = QFileInfo(fullName).absolutePath();
    //QString fileName = QFileInfo(fullName).fileName();

    DenseTrack procesar;
    if (ui->ParametrosCheckBox->isChecked())
    {
        procesar.TrackerDirectory(ui->ShowTrack->isChecked(),dirName.toStdString(), ui->ParametrosCheckBox->isChecked(), ui->StartFrame->text().toInt(),ui->EndFrame->text().toInt(),ui->TrajectoryLength->text().toInt(),ui->SamplingStride->text().toInt(),ui->NeighborhoodSize->text().toInt(),ui->SpatialCells->text().toInt(),ui->TrajectoryLength->text().toInt(),ui->ScaleNumber->text().toInt(),ui->InitialGap->text().toInt());
    }
    else
    {
        procesar.TrackerDirectory(ui->ShowTrack->isChecked(),dirName.toStdString(), ui->ParametrosCheckBox->isChecked());
    }
}

void MainWindow::on_DOFDirDiccButton_clicked()
{
    QString fullName = QFileDialog::getOpenFileName(this,
            tr("Open Diccionary"), "/home/ercmos", tr("Diccionary Files (*.txt)"));

    QString dirName = QFileInfo(fullName).absolutePath();
    QString fileName = QFileInfo(fullName).fileName();

    BoW saco;
    saco.CrearDiccionarioDirectorio(dirName.toStdString());
}

void MainWindow::on_TrainSVMButton_clicked()
{
    QString fullName = QFileDialog::getOpenFileName(this,
            tr("Open Diccionary"), "/home/ercmos", tr("Diccionary Files (*.yml)"));

    //QString dirName = QFileInfo(fullName).absolutePath();
    QString fileName = QFileInfo(fullName).fileName();
    string extension;
    string fichero;

    if(fileName.toStdString().find_last_of(".") != std::string::npos)
            extension = fileName.toStdString().substr(fileName.toStdString().find_last_of(".")+1);
            fichero = fileName.toStdString().substr(0,fileName.toStdString().find_last_of("."));

    Mat vocabulary;
    FileStorage fs(fullName.toStdString().c_str(), FileStorage::READ);
        fs[fichero.c_str()] >> vocabulary;
        fs.release();

    Classifier svm_class;
    BOWImgDescriptorExtractor bowide();
    //bowide().setVocabulary(vocabulary);


}
