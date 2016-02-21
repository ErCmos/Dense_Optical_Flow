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

    QString dirName = QFileInfo(fullName).absolutePath();
    QString fileName = QFileInfo(fullName).fileName();
    string extension;
    string fichero;

    BoW saco;
    saco.CrearDiccionarioAcciones(dirName.toStdString());

    if(fileName.toStdString().find_last_of(".") != std::string::npos)
            extension = fileName.toStdString().substr(fileName.toStdString().find_last_of(".")+1);
            fichero = fileName.toStdString().substr(0,fileName.toStdString().find_last_of("."));

    Mat vocabulary, vocabulary_f;
    FileStorage fs(fullName.toStdString().c_str(), FileStorage::READ);
        fs[fichero.c_str()] >> vocabulary;
        vocabulary.convertTo(vocabulary_f, CV_32FC1);
        fs.release();

    Mat label,label_f;
    //QString dirName = QFileInfo(fullName).absolutePath();
    string labelfile=dirName.toStdString()+"/"+fichero.c_str()+"_Labels."+extension;
    FileStorage fs2(labelfile, FileStorage::READ);
        fs2[fichero.c_str()] >> label;
        label.convertTo(label_f, CV_32FC1);
        fs2.release();
/*
    float trainingData[4][2]= { {501, 10}, {255, 10}, {501, 255}, {10, 501} };
    Mat trainingDataMat(4, 2, CV_32FC1, trainingData);

        // Set up training data
    float etiquetas[4] = {1.0, -1.0, -1.0, -1.0};
    Mat etiquetasMat(4, 1, CV_32FC1, etiquetas);

    Mat voc(trainingDataMat.rows,trainingDataMat.cols,CV_32FC1),lab(etiquetasMat.rows,etiquetasMat.cols,CV_32FC1);

    voc.at<float>(0,0)=501; voc.at<float>(0,1)=10;
    voc.at<float>(1,0)=255; voc.at<float>(1,1)=10;
    voc.at<float>(2,0)=501; voc.at<float>(2,1)=255;
    voc.at<float>(3,0)=10; voc.at<float>(3,1)=501;

    lab.at<float>(0)=1.0;
    lab.at<float>(1)=-1.0;
    lab.at<float>(2)=-1.0;
    lab.at<float>(3)=-1.0;

    Classifier svm_class;
    //voc.refcount=0;
    //lab.refcount=0;
    std::cout << std::endl << "vocabulary=" << vocabulary << std::endl;
    std::cout << std::endl << "voc=" << voc << std::endl;
    std::cout << std::endl << "vocabulary_f=" << vocabulary_f << std::endl;

    cv::Mat diff = trainingDataMat != vocabulary_f;
    std::cout << std::endl << "diff=" << diff << std::endl;
    cv::compare(etiquetasMat, label_f, diff, cv::CMP_NE);
    std::cout << std::endl << "compare=" << diff << std::endl;
*/
    Classifier svm_class;
    svm_class.svm(vocabulary_f,label_f,vocabulary,label);
//    Track kk;
    //Ptr<DescriptorExtractor> extractor = OFDE::create("ofde");
    //Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");
    Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create("SURF");
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");
    //Ptr<DescriptorExtractor> extractor(new Track);
    //create Sift descriptor extractor
    //Ptr<DescriptorExtractor> extractor(new SiftDescriptorExtractor);
//    Ptr<Track> extractor(new Track);
    BOWImgDescriptorExtractor bowide(extractor,matcher);
    bowide.setVocabulary(vocabulary);
    //bowide().setVocabulary(vocabulary);
}
