#ifndef TRACKER_H
#define TRACKER_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

#include <unistd.h>

using namespace cv;

namespace constantes
{
extern int start_frame;
extern int end_frame;
extern int scale_num;
extern const float scale_stride;

// parameters for descriptors
extern int patch_size;
extern int nxy_cell;
extern int nt_cell;
extern float epsilon;
extern const float min_flow;

// parameters for tracking
extern double quality;
extern int min_distance;
extern int init_gap;
extern int track_length ;

// parameters for rejecting trajectory
extern const float min_var;
extern const float max_var;
extern const float max_dis;
}

typedef struct {
    int x;       // top left corner
    int y;
    int width;
    int height;
}RectInfo;

typedef struct {
    int width;   // resolution of the video
    int height;
    int length;  // number of frames
}SeqInfo;

typedef struct {
    int length;  // length of the trajectory
    int gap;     // initialization gap for feature re-sampling
}TrackInfo;

typedef struct {
    int nBins;   // number of bins for vector quantization
    bool isHof;
    int nxCells; // number of cells in x direction
    int nyCells;
    int ntCells;
    int dim;     // dimension of the descriptor
    int height;  // size of the block for computing the descriptor
    int width;
}DescInfo;

// integral histogram for the descriptors
typedef struct {
    int height;
    int width;
    int nBins;
    float* desc;
}DescMat;

class Tracker
{
public:
    Tracker();
    ~Tracker();
    //int Trackeador(bool show_track, std::string fileName, bool parametros);

};

#endif // TRACKER_H
