#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv::xfeatures2d;

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
            tr("Open Video Files"), "/home/ercmos", tr("Video Files (*.avi)"));

    QString dirName = QFileInfo(fullName).absolutePath();
    //QString fileName = QFileInfo(fullName).fileName();

/*    QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home/ercmos",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
*/
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
    //QString fileName = QFileInfo(fullName).fileName();

    BoW saco;
    saco.CrearDiccionarioDirectorio(dirName.toStdString());
}

void MainWindow::on_TrainSVMButton_clicked()
{
/*    QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory (*.yml)"),
                                                 "/home/ercmos",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
*/
    QString fullName = QFileDialog::getOpenFileName(this,
               tr("Open Diccionary"), "/home/ercmos", tr("Clustered Labeled Diccionary File (*.yml)"));

    //QString dirName = QFileInfo(fullName).absolutePath();
    QString fileName = QFileInfo(fullName).fileName();
    Mat LabeledActions,LabeledActions_f;
    //FileStorage fs(dirName.toStdString()+"/"+"LabeledActions_Clustered.yml", FileStorage::READ);
    FileStorage fs(fullName.toStdString(), FileStorage::READ);
        fs["LabeledActions"] >> LabeledActions;
        LabeledActions.convertTo(LabeledActions_f, CV_32FC1);
        fs.release();

    Mat Cdata(LabeledActions_f.rows,LabeledActions_f.cols-1,CV_32FC1), Clabes(LabeledActions_f.rows,1,CV_32FC1);
    Clabes=LabeledActions_f.colRange(LabeledActions_f.cols-1,LabeledActions_f.cols);
    Cdata=LabeledActions_f.colRange(0,LabeledActions_f.cols-1);
    string Name = fullName.toStdString().substr(0,fullName.toStdString().find_first_of(".")) + ".xml";
    QString Nombre=QString::fromStdString(Name);
    ui->SVMFile->setText(Nombre);
    Classifier SVM;
    SVM.svm_train(Cdata,Clabes,Name);
}

void MainWindow::on_TestFileButton_clicked()
{
/*
    DenseTrack procesar;
    if (ui->ParametrosCheckBox->isChecked())
    {
        procesar.Tracker(ui->ShowTrack->isChecked(),ui->VideoFile->text().toStdString(), ui->ParametrosCheckBox->isChecked(), ui->StartFrame->text().toInt(),ui->EndFrame->text().toInt(),ui->TrajectoryLength->text().toInt(),ui->SamplingStride->text().toInt(),ui->NeighborhoodSize->text().toInt(),ui->SpatialCells->text().toInt(),ui->TrajectoryLength->text().toInt(),ui->ScaleNumber->text().toInt(),ui->InitialGap->text().toInt());
    }
    else
    {
        procesar.Tracker(ui->ShowTrack->isChecked(),ui->VideoFile->text().toStdString(), ui->ParametrosCheckBox->isChecked());
    }
    BoW saco;
    string Name=ui->VideoFile->text().toStdString();
    string Path=Name.substr(0,Name.find_last_of("/"));
    Name=Name.substr(0,Name.find_first_of("."));
    Name=Name+".txt";
    Name=Name.substr(Name.find_last_of("/")+1);


    saco.CrearDiccionario(Name,Path);
*/
    QString DicName = QFileDialog::getOpenFileName(this,
               tr("Open Diccionary"), "/home/ercmos", tr("Clustered Labeled Diccionary File (*.yml)"));

    //QString dirDicName = QFileInfo(DicName).absolutePath();
    //QString fileDicName = QFileInfo(DicName).fileName();

    QString SVMName = QFileDialog::getOpenFileName(this,
               tr("Open SVM"), "/home/ercmos", tr("Clustered Labeled Diccionary File (*.xml)"));

    //QString dirSVMName = QFileInfo(SVMName).absolutePath();
    //QString fileSVMName = QFileInfo(SVMName).fileName();

    Classifier SVM;
    Mat testData,testData_f;
    FileStorage fs(DicName.toStdString(), FileStorage::READ);
        fs["LabeledActions"] >> testData;
        testData.convertTo(testData_f, CV_32FC1);
        fs.release();

    Mat Cdata(testData.rows,testData.cols-1,CV_32FC1), Clabes(testData.rows,1,CV_32FC1);
    Clabes=testData.colRange(testData.cols-1,testData.cols);
    Cdata=testData.colRange(0,testData.cols-1);

    SVM.svm_test(Cdata,Clabes,SVMName.toStdString());
}
