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
/*
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
*/
/*
MainWindow::MainWindow(QStringList arguments, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)*/
MainWindow::MainWindow(int argc, char *argv[], QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    //qDebug << arguments;
    //qDebug() << arguments;
    ui->setupUi(this);

    //if (arguments.size()<=1)
    if (argc<=1)
    {
        //ui->svm_type->setText(QString::fromStdString("CvSVM::C_SVC"));
        ui->svm_type->setText(QString::fromStdString("100"));
        //ui->kernel_type->setText(QString::fromStdString("CvSVM::RBF")); //CvSVM::RBF, CvSVM::LINEAR CvSVM::POLY, CvSVM::SIGMOID
        ui->kernel_type->setText(QString::fromStdString("2")); //CvSVM::RBF, CvSVM::LINEAR CvSVM::POLY, CvSVM::SIGMOID
        ui->degree->setText(QString::fromStdString("0")); // for poly
        ui->gamma->setText(QString::fromStdString("10")); // 20 for poly/rbf/sigmoid 20
        ui->coef0->setText(QString::fromStdString("0")); // for poly/sigmoid

        ui->C->setText(QString::fromStdString("100")); // 7 for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
        ui->nu->setText(QString::fromStdString("0.0")); // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
        ui->p->setText(QString::fromStdString("0.0")); // for CV_SVM_EPS_SVR

        ui->class_weights->setText(QString::fromStdString("NULL")); // for CV_SVM_C_SVC
        //ui->term_crit_type->setText(QString::fromStdString("CV_TERMCRIT_ITER +CV_TERMCRIT_EPS"));
        ui->term_crit_type->setText(QString::fromStdString("3"));
        ui->term_crit_max_iter->setText(QString::fromStdString("1000"));
        ui->term_crit_epsilon->setText(QString::fromStdString("0,000001"));
    }
    else
    {
        ui->svm_type->setText(QString::fromStdString(argv[1]));
        ui->kernel_type->setText(QString::fromStdString(argv[2])); //CvSVM::RBF, CvSVM::LINEAR CvSVM::POLY, CvSVM::SIGMOID
        ui->degree->setText(QString::fromStdString(argv[3])); // for poly
        ui->gamma->setText(QString::fromStdString(argv[4])); // 20 for poly/rbf/sigmoid 20
        ui->coef0->setText(QString::fromStdString(argv[5])); // for poly/sigmoid

        ui->C->setText(QString::fromStdString(argv[6])); // 7 for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
        ui->nu->setText(QString::fromStdString(argv[7])); // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
        ui->p->setText(QString::fromStdString(argv[8])); // for CV_SVM_EPS_SVR

        ui->class_weights->setText(QString::fromStdString(argv[9])); // for CV_SVM_C_SVC
        ui->term_crit_type->setText(QString::fromStdString(argv[10]));
        ui->term_crit_max_iter->setText(QString::fromStdString(argv[11]));
        ui->term_crit_epsilon->setText(QString::fromStdString(argv[12]));

        TestAutomatico();
    }
    //qDebug() << arguments;
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

    ui->Homo_X->setInputMask("9");
    ui->Homo_Y->setInputMask("9");
    ui->Homo_X->setText("1");
    ui->Homo_Y->setText("1");

    if (ui->ParametrosCheckBox->isChecked())
        ui->Parametros->setEnabled(true);
    else
        ui->Parametros->setEnabled(false);

    if (ui->SVM_Parameters_Check->isChecked())
        ui->SVM_Parameters->setEnabled(true);
    else
        ui->SVM_Parameters->setEnabled(false);

    if (ui->Homography_checkBox->isChecked())
        ui->Homography->setEnabled(true);
    else
        ui->Homography->setEnabled(false);
}

void MainWindow::TestAutomatico()
{
///////////////////////////Train SVM/////////////////////////////////////////////
    Classifier SVM;
    CvSVMParams param = CvSVMParams();

    //param.svm_type = CvSVM::C_SVC;
    //param.kernel_type = CvSVM::RBF; //CvSVM::RBF, CvSVM::LINEAR CvSVM::POLY, CvSVM::SIGMOID
    param.svm_type=atoi(ui->svm_type->text().toStdString().c_str());
    param.kernel_type=atoi(ui->kernel_type->text().toStdString().c_str());
    param.degree = atof(ui->degree->text().toStdString().c_str()); // for poly
    param.gamma = atof(ui->gamma->text().toStdString().c_str()); // 20 for poly/rbf/sigmoid 20
    param.coef0 = atof(ui->coef0->text().toStdString().c_str()); // for poly/sigmoid

    param.C = atof(ui->C->text().toStdString().c_str()); // 7 for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
    param.nu= atof(ui->nu->text().toStdString().c_str()); // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
    param.p = atof(ui->p->text().toStdString().c_str()); // for CV_SVM_EPS_SVR

    param.class_weights = NULL; // for CV_SVM_C_SVC

    param.term_crit.type=atoi(ui->term_crit_type->text().toStdString().c_str());
    param.term_crit.max_iter=atoi(ui->term_crit_max_iter->text().toStdString().c_str());
    param.term_crit.epsilon=atof(ui->term_crit_epsilon->text().toStdString().c_str());

    Mat BoWFeatures,BoWLabels;
    BoW saco;
    saco.BoW_DOF_DE("/home/ercmos/Videos/Train","/home/ercmos/Dropbox/Master/TFM/Soft/workspace/Videos/Actions/Actions_Clustered.yml",BoWFeatures, BoWLabels);

    //ui->SVMFile->setText("/home/ercmos/Dropbox/Master/TFM/Soft/workspace/Videos/Actions/SVM_Type="+QString::number(param.svm_type).toStdString()+"_KernelType="+QString::number(param.kernel_type).toStdString()+"_Degree="+QString::number(param.degree).toStdString()+"_Gamma=" + QString::number(param.gamma).toStdString() + "_Coef0="+QString::number(param.coef0).toStdString()+ "_C="+QString::number(param.C).toStdString()+"_nu="+QString::number(param.nu).toStdString()+"_p="+QString::number(param.p).toStdString()+ "_SVM.xml");

    //SVM.svm_train(BoWFeatures,BoWLabels,"/home/ercmos/Dropbox/Master/TFM/Soft/workspace/Videos/Actions/SVM.xml", param);
    SVM.svm_train(BoWFeatures,BoWLabels,"/home/ercmos/Dropbox/Master/TFM/Soft/workspace/Videos/Actions/SVM_Type="+QString::number(param.svm_type).toStdString()+"_KernelType="+QString::number(param.kernel_type).toStdString()+"_Degree="+QString::number(param.degree).toStdString()+"_Gamma=" + QString::number(param.gamma).toStdString() + "_Coef0="+QString::number(param.coef0).toStdString()+ "_C="+QString::number(param.C).toStdString()+"_nu="+QString::number(param.nu).toStdString()+"_p="+QString::number(param.p).toStdString()+ "_SVM.xml", param);

/////////////////////////////////////////////////////////////////////////////////////



///////////////////////////Test Train Files/////////////////////////////////////////////
   Mat BoWFeatures_Train=BoWFeatures.clone(),BoWLabels_Train=BoWLabels.clone();


    //BoW saco;
    //saco.BoW_DOF_DE("/home/ercmos/Videos/Train","/home/ercmos/Dropbox/Master/TFM/Soft/workspace/Videos/Actions/Actions_Clustered.yml",BoWFeatures_Train, BoWLabels_Train);

   // Classifier SVM;
    //Mat Pred=SVM.svm_test(BoWFeatures,BoWLabels,"/home/ercmos/Dropbox/Master/TFM/Soft/workspace/Videos/Actions/SVM.xml");
    Mat Pred_Train=SVM.svm_test(BoWFeatures_Train,BoWLabels_Train,"/home/ercmos/Dropbox/Master/TFM/Soft/workspace/Videos/Actions/SVM_Type="+QString::number(param.svm_type).toStdString()+"_KernelType="+QString::number(param.kernel_type).toStdString()+"_Degree="+QString::number(param.degree).toStdString()+"_Gamma=" + QString::number(param.gamma).toStdString() + "_Coef0="+QString::number(param.coef0).toStdString()+ "_C="+QString::number(param.C).toStdString()+"_nu="+QString::number(param.nu).toStdString()+"_p="+QString::number(param.p).toStdString()+ "_SVM.xml");
    Mat ConfMtx_Train=SVM.ConfusionMatrix(Pred_Train,BoWLabels_Train);
    float diag_Train=0, sum_Train=0, Accuracy_Train=0;
    for(int i=0;i<ConfMtx_Train.rows;i++)
    {
         for(int j=0;j<ConfMtx_Train.cols;j++)
         {
              if(i==j)
                  diag_Train=diag_Train+ConfMtx_Train.at<float>(i,j);
         }
    }
    for(int i=0;i<ConfMtx_Train.rows;i++)
    {
         for(int j=0;j<ConfMtx_Train.cols;j++)
         {
                  sum_Train=sum_Train+ConfMtx_Train.at<float>(i,j);
         }
    }
    Accuracy_Train=(diag_Train/sum_Train)*100;

    Mat BoWFeatures_Test,BoWLabels_Test;
    saco.BoW_DOF_DE("/home/ercmos/Videos/Test","/home/ercmos/Dropbox/Master/TFM/Soft/workspace/Videos/Actions/Actions_Clustered.yml",BoWFeatures_Test, BoWLabels_Test);

    //Pred=SVM.svm_test(BoWFeatures,BoWLabels,"/home/ercmos/Dropbox/Master/TFM/Soft/workspace/Videos/Actions/SVM.xml");
    Mat Pred_Test=SVM.svm_test(BoWFeatures_Test,BoWLabels_Test,"/home/ercmos/Dropbox/Master/TFM/Soft/workspace/Videos/Actions/SVM_Type="+QString::number(param.svm_type).toStdString()+"_KernelType="+QString::number(param.kernel_type).toStdString()+"_Degree="+QString::number(param.degree).toStdString()+"_Gamma=" + QString::number(param.gamma).toStdString() + "_Coef0="+QString::number(param.coef0).toStdString()+ "_C="+QString::number(param.C).toStdString()+"_nu="+QString::number(param.nu).toStdString()+"_p="+QString::number(param.p).toStdString()+ "_SVM.xml");
    Mat ConfMtx_Test=SVM.ConfusionMatrix(Pred_Test,BoWLabels_Test);
    float diag_Test=0, sum_Test=0, Accuracy_Test=0;
    for(int i=0;i<ConfMtx_Train.rows;i++)
    {
         for(int j=0;j<ConfMtx_Test.cols;j++)
         {
              if(i==j)
                  diag_Test=diag_Test+ConfMtx_Test.at<float>(i,j);
         }
    }
    for(int i=0;i<ConfMtx_Test.rows;i++)
    {
         for(int j=0;j<ConfMtx_Test.cols;j++)
         {
                  sum_Test=sum_Test+ConfMtx_Test.at<float>(i,j);
         }
    }
    Accuracy_Test=(diag_Test/sum_Test)*100;


    FILE *stream ;
    string salida="/home/ercmos/Dropbox/Master/TFM/Soft/workspace/Videos/Actions/SVM_Type="+QString::number(param.svm_type).toStdString()+"_KernelType="+QString::number(param.kernel_type).toStdString()+"_Degree="+QString::number(param.degree).toStdString()+"_Gamma=" + QString::number(param.gamma).toStdString() + "_Coef0="+QString::number(param.coef0).toStdString()+ "_C="+QString::number(param.C).toStdString()+"_nu="+QString::number(param.nu).toStdString()+"_p="+QString::number(param.p).toStdString()+ ".txt";
       if((stream = freopen(salida.c_str(), "a+", stdout)) == NULL)
          exit(-1);
    cout << "Matriz de confusion TRAIN: " << endl;
    cout << ConfMtx_Train << endl;
    cout << "Train Accuracy: " << Accuracy_Train << " %" << endl;
    cout << "Matriz de confusion TEST: " << endl;
    cout << ConfMtx_Test << endl;
    cout << "Test Accuracy: " << Accuracy_Test << " %" << endl;
    fclose(stream);
    exit(EXIT_SUCCESS);
//////////////////////////////////////////////////////////////////////////////////
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
    vector<Mat> vec_H, vec_L, blocks; //, vec_LTS;
    CameraMotion motion;
    string NAME;

    if (ui->ParametrosCheckBox->isChecked())
    {
        procesar.Tracker(ui->ShowTrack->isChecked(),ui->VideoFile->text().toStdString(), ui->ParametrosCheckBox->isChecked(), ui->StartFrame->text().toInt(),ui->EndFrame->text().toInt(),ui->TrajectoryLength->text().toInt(),ui->SamplingStride->text().toInt(),ui->NeighborhoodSize->text().toInt(),ui->SpatialCells->text().toInt(),ui->TrajectoryLength->text().toInt(),ui->ScaleNumber->text().toInt(),ui->InitialGap->text().toInt());
    }
    else
    {
        procesar.Tracker(ui->ShowTrack->isChecked(),ui->VideoFile->text().toStdString(), ui->ParametrosCheckBox->isChecked());
    }

    if ((ui->Homography_checkBox->isChecked()) && ((ui->Homo_X->text().toInt()!=1) || (ui->Homo_Y->text().toInt()!=1)))
    {
        motion.subdivide(ui->VideoFile->text().toStdString(),ui->Homo_Y->text().toInt(),ui->Homo_X->text().toInt(),blocks);

        for (int n=0; n<(ui->Homo_Y->text().toInt())*(ui->Homo_X->text().toInt());n++)
        {
                stringstream converter;
                converter << n;
                NAME=ui->VideoFile->text().toStdString().substr(0,ui->VideoFile->text().toStdString().find_last_of("."))+"_"+converter.str()+".avi";
                motion.get_homography_sequence(NAME,vec_H);
                for (int i=0;i<vec_H.size(); i++)
                {
                    Mat L = motion.compute_Liecoeffs (vec_H[i]);
                    vec_L.push_back(L);
                }
                motion.save_vectors (vec_L, NAME);
                motion.DOF_LIE(NAME,(ui->Homo_Y->text().toInt())*(ui->Homo_X->text().toInt()));
                remove(NAME.c_str());
        }
    }
    else
    {
        motion.get_homography_sequence(ui->VideoFile->text().toStdString(),vec_H);
        for (int i=0;i<vec_H.size(); i++)
        {
            Mat L = motion.compute_Liecoeffs (vec_H[i]);
            vec_L.push_back(L);
        }

        /* Output timeseries of Lie coefficients */
//        motion.construct_timeseries (vec_L, vec_LTS);
        /* save vectors in output files */
        motion.save_vectors (vec_L, ui->VideoFile->text().toStdString());
        motion.DOF_LIE(ui->VideoFile->text().toStdString(),1);
    }
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

    DenseTrack procesar;
    if (ui->ParametrosCheckBox->isChecked())
    {
        procesar.TrackerDirectory(ui->Homo_X->text().toInt(),ui->Homo_Y->text().toInt(), ui->ShowTrack->isChecked(),dirName.toStdString(), ui->ParametrosCheckBox->isChecked(), ui->StartFrame->text().toInt(),ui->EndFrame->text().toInt(),ui->TrajectoryLength->text().toInt(),ui->SamplingStride->text().toInt(),ui->NeighborhoodSize->text().toInt(),ui->SpatialCells->text().toInt(),ui->TrajectoryLength->text().toInt(),ui->ScaleNumber->text().toInt(),ui->InitialGap->text().toInt());
    }
    else
    {
        procesar.TrackerDirectory(ui->Homo_X->text().toInt(),ui->Homo_Y->text().toInt(), ui->ShowTrack->isChecked(),dirName.toStdString(), ui->ParametrosCheckBox->isChecked());
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

    BoW saco;
    saco.CrearDiccionarioAcciones(dirDiccName.toStdString());
}

void MainWindow::on_TrainSVMButton_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("DoF Train Files (*.txt)"),
                                                 "/home/ercmos",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    QString DicfullName = QFileDialog::getOpenFileName(this,
               tr("Open Diccionary"), "/home/ercmos", tr("Diccionary File (*.yml)"));

    Classifier SVM;
    CvSVMParams param = CvSVMParams();
/*
    param.svm_type = CvSVM::C_SVC;
    param.kernel_type = CvSVM::RBF; //CvSVM::RBF, CvSVM::LINEAR CvSVM::POLY, CvSVM::SIGMOID
    param.degree = 0; // for poly
    param.gamma = 10; // 20 for poly/rbf/sigmoid 20
    param.coef0 = 0; // for poly/sigmoid

    param.C = 100; // 7 for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
    param.nu = 0.0; // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
    param.p = 0.0; // for CV_SVM_EPS_SVR
*/
    //param.svm_type = 100;
    //param.kernel_type = 2; //CvSVM::RBF, CvSVM::LINEAR CvSVM::POLY, CvSVM::SIGMOID
    param.svm_type = atoi(ui->svm_type->text().toStdString().c_str());
    param.kernel_type = atoi(ui->kernel_type->text().toStdString().c_str()); //CvSVM::RBF, CvSVM::LINEAR CvSVM::POLY, CvSVM::SIGMOID
    param.degree = atof(ui->degree->text().toStdString().c_str()); // for poly
    param.gamma = atof(ui->gamma->text().toStdString().c_str()); // 20 for poly/rbf/sigmoid 20
    param.coef0 = atof(ui->coef0->text().toStdString().c_str()); // for poly/sigmoid

    param.C = atof(ui->C->text().toStdString().c_str()); // 7 for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
    param.nu= atof(ui->nu->text().toStdString().c_str()); // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
    param.p = atof(ui->p->text().toStdString().c_str()); // for CV_SVM_EPS_SVR

    param.class_weights = NULL; // for CV_SVM_C_SVC
/*    param.term_crit.type = CV_TERMCRIT_ITER +CV_TERMCRIT_EPS;
    param.term_crit.max_iter = 1000;
    param.term_crit.epsilon = 1e-6;
*/
    //param.class_weights=Mat(atoi(ui->class_weights->text().toStdString().c_str()));
    param.term_crit.type=atoi(ui->term_crit_type->text().toStdString().c_str());
    param.term_crit.max_iter=atoi(ui->term_crit_max_iter->text().toStdString().c_str());
    param.term_crit.epsilon=atof(ui->term_crit_epsilon->text().toStdString().c_str());

    Mat BoWFeatures,BoWLabels;
    BoW saco;
    saco.BoW_DOF_DE(dirName.toStdString(),DicfullName.toStdString(),BoWFeatures, BoWLabels);

    string Name = dirName.toStdString() + "/SVM.xml";

    ui->SVMFile->setText(QString::fromStdString(Name));

    SVM.svm_train(BoWFeatures,BoWLabels,Name, param);
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
    float diag=0, sum=0, Accuracy=0;
    for(int i=0;i<ConfMtx.rows;i++)
    {
         for(int j=0;j<ConfMtx.cols;j++)
         {
              if(i==j)
                  diag=diag+ConfMtx.at<float>(i,j);
         }
    }
    for(int i=0;i<ConfMtx.rows;i++)
    {
         for(int j=0;j<ConfMtx.cols;j++)
         {
                  sum=sum+ConfMtx.at<float>(i,j);
         }
    }
    Accuracy=(diag/sum)*100;
    cout << "Accuracy: " << Accuracy << " %" << endl;
}

void MainWindow::on_SVM_Parameters_Check_clicked()
{
    if (ui->SVM_Parameters_Check->isChecked())
        ui->SVM_Parameters->setEnabled(true);
    else
        ui->SVM_Parameters->setEnabled(false);
}

void MainWindow::on_Homography_checkBox_clicked()
{
    if (ui->Homography_checkBox->isChecked())
        ui->Homography->setEnabled(true);
    else
        ui->Homography->setEnabled(false);
}
