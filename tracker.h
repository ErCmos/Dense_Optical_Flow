#ifndef TRACKER_H
#define TRACKER_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

#include <unistd.h>

using namespace cv;
/*
namespace constantes
{
static int start_frame = 0;
static int end_frame = INT_MAX;
static int scale_num = 8;
static const float scale_stride = sqrt(2);

// parameters for descriptors
static int patch_size = 32;
static int nxy_cell = 2;
static int nt_cell = 3;
static float epsilon = 0.05;
static const float min_flow = 0.4;

// parameters for tracking
static double quality = 0.001;
static int min_distance = 5;
static int init_gap = 1;
static int track_length = 15;

// parameters for rejecting trajectory
static const float min_var = sqrt(3);
static const float max_var = 50;
static const float max_dis = 20;
}
*/

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

enum clases{
    boxing,
    running,
    test_sequences,
    NumberOfTypes
};

class Tracker
{
public:
    Tracker();
    ~Tracker();
    //int Trackeador(bool show_track, std::string fileName, bool parametros);

};

#endif // TRACKER_H
