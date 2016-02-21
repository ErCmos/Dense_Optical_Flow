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
