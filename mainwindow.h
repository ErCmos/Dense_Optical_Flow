#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <video_player.h>
#include <densetrack.h>
//#include <tracker.h>

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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_OpenVideo_clicked();

    void on_ShowTrack_clicked();

    void on_ParametrosCheckBox_clicked();

    void on_ProcesarButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
