#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv::xfeatures2d;
using namespace std;

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

void MainWindow::on_ClusterDiccButton_clicked()
{
    QString fullDiccName = QFileDialog::getOpenFileName(this,
               tr("Open Diccionary"), "/home/ercmos", tr("Diccionary File (*.yml)"));
    QString dirDiccName = QFileInfo(fullDiccName).absolutePath();
/*    QString fileDiccName = QFileInfo(fullDiccName).fileName();

    QString fullTestName = QFileDialog::getOpenFileName(this,
               tr("Open TestFiles"), "/home/ercmos", tr("Test File (*.txt)"));
    QString dirTestName = QFileInfo(fullTestName).absolutePath();
    QString fileTestName = QFileInfo(fullTestName).fileName();
*/
    BoW saco;
    saco.CrearDiccionarioAcciones(dirDiccName.toStdString());
    //saco.BoW_Clasificador(dirTestName.toStdString(),fullDiccName.toStdString());
}

void MainWindow::on_TrainSVMButton_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("DoF Train Files (*.txt)"),
                                                 "/home/ercmos",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    QString DicfullName = QFileDialog::getOpenFileName(this,
               tr("Open Diccionary"), "/home/ercmos", tr("Diccionary File (*.yml)"));

    Mat BoWFeatures,BoWLabels;
    BoW saco;
    saco.BoW_DOF_DE(dirName.toStdString(),DicfullName.toStdString(),BoWFeatures, BoWLabels);

    //string Name = dirName.toStdString() + "/" +dirName.toStdString().substr(dirName.toStdString().find_last_of("/")+1) + ".xml";
    string Name = dirName.toStdString() + "/SVM.xml";

    ui->SVMFile->setText(QString::fromStdString(Name));

    Classifier SVM;
    SVM.svm_train(BoWFeatures,BoWLabels,Name);
}

void MainWindow::on_TestFileButton_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("DoF Test Files (*.txt)"),
                                                 "/home/ercmos",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    QString DicfullName = QFileDialog::getOpenFileName(this,
               tr("Open Diccionary"), "/home/ercmos", tr("Diccionary File (*.yml)"));

    QString SVMfullName = QFileDialog::getOpenFileName(this,
               tr("Open SVM"), "/home/ercmos", tr("SVM File (*.xml)"));

    ui->SVMFile->setText((SVMfullName));

    Mat BoWFeatures,BoWLabels;

    BoW saco;
    saco.BoW_DOF_DE(dirName.toStdString(),DicfullName.toStdString(),BoWFeatures, BoWLabels);

    Classifier SVM;
    Mat Pred=SVM.svm_test(BoWFeatures,BoWLabels,SVMfullName.toStdString());
    Mat ConfMtx=SVM.ConfusionMatrix(Pred,BoWLabels);
    cout << "Matriz de confusion: " << endl;
    cout << ConfMtx << endl;
}
