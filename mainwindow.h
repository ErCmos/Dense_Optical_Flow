#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <video_player.h>
#include <densetrack.h>
#include "classifier.h"
#include "bow.h"
#include "descriptors.h"
//#include <opencv2/features2d.hpp>
//#include "opencv2/xfeatures2d.hpp"
#include "ofde.h"
#include "cameramotion.h"

#include <QMessageBox>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //explicit MainWindow(QWidget *parent = 0);
    //explicit MainWindow(QStringList arguments, QWidget *parent = 0);
    explicit MainWindow(int argc, char *argv[], QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_OpenVideo_clicked();

    void on_ShowTrack_clicked();

    void on_ParametrosCheckBox_clicked();

    void on_ProcesarButton_clicked();

    void on_CrearDiccButton_clicked();

    void on_DOFDirectorioButton_clicked();

    void on_DOFDirDiccButton_clicked();

    void on_TrainSVMButton_clicked();

    void on_TestFileButton_clicked();

    void on_ClusterDiccButton_clicked();

    void on_SVM_Parameters_Check_clicked();

    void on_Homography_checkBox_clicked();

private:
    Ui::MainWindow *ui;
    void TestAutomatico();
};

#endif // MAINWINDOW_H
