#include "tracker.h"
#include <QMessageBox>
//#include "densetrack.h"
//#include "initialize.h"
//#include "descriptors.h"
//#include "opticalflow.h"

namespace constantes
{
extern int start_frame = 0;
extern int end_frame = INT_MAX;
extern int scale_num = 8;
extern const float scale_stride = sqrt(2);

// parameters for descriptors
extern int patch_size = 32;
extern int nxy_cell = 2;
extern int nt_cell = 3;
extern float epsilon = 0.05;
extern const float min_flow = 0.4;

// parameters for tracking
extern double quality = 0.001;
extern int min_distance = 5;
extern int init_gap = 1;
extern int track_length = 15;

// parameters for rejecting trajectory
extern const float min_var = sqrt(3);
extern const float max_var = 50;
extern const float max_dis = 20;
}

/*
using namespace cv;

//int DenseTrack::Tracker(bool show_track, std::string fileName, bool parametros, int FrameInicio, int FrameFinal)
//int DenseTrack::Tracker(bool show_track, std::string fileName, bool parametros, int start_frame, int end_frame, int track_length, int min_distance, int patch_size, int nxy_cell, int nt_cell, int scale_num, int init_gap)
int Tracker::Trackeador(bool show_track, std::string fileName, bool parametros)
{
    char *video = new char[fileName.length() + 1];
    strcpy(video, fileName.c_str());

    VideoCapture capture;
    capture.open(video);

    if(!capture.isOpened())
    {
        QMessageBox msgBox;
        msgBox.setText("Could not initialize capturing..");
        //msgBox.setInformativeText("Do you want to save your changes?");
        //msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        //msgBox.setDefaultButton(QMessageBox::Save);
        //int ret = msgBox.exec();
        //printf( "Could not initialize capturing..\n" );
        return -1;
    }
    int frame_num = 0;
    TrackInfo trackInfo;
    DescInfo hogInfo, hofInfo, mbhInfo;

    InitTrackInfo(&trackInfo, track_length, init_gap);
    InitDescInfo(&hogInfo, 8, false, patch_size, nxy_cell, nt_cell);
    InitDescInfo(&hofInfo, 9, true, patch_size, nxy_cell, nt_cell);
    InitDescInfo(&mbhInfo, 8, false, patch_size, nxy_cell, nt_cell);

    SeqInfo seqInfo;
    InitSeqInfo(&seqInfo, video);

    if(parametros)
        seqInfo.length = end_frame - start_frame + 1;


    delete [] video;
}
*/
