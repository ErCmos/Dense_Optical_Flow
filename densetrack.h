#ifndef DENSETRACK_H
#define DENSETRACK_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

#include <unistd.h>
#include <tracker.h>
//#include "bow.h"
#include "dirent.h"

using namespace cv;

class Track
{
public:
    std::vector<Point2f> point;
    std::vector<float> hog;
    std::vector<float> hof;
    std::vector<float> mbhX;
    std::vector<float> mbhY;
    int index;

    Track(const Point2f& point_, const TrackInfo& trackInfo, const DescInfo& hogInfo,
          const DescInfo& hofInfo, const DescInfo& mbhInfo)
        : point(trackInfo.length+1), hog(hogInfo.dim*trackInfo.length),
          hof(hofInfo.dim*trackInfo.length), mbhX(mbhInfo.dim*trackInfo.length), mbhY(mbhInfo.dim*trackInfo.length)
    {
        index = 0;
        point[0] = point_;
    }

    void addPoint(const Point2f& point_)
    {
        index++;
        point[index] = point_;
    }
};

class DenseTrack
{
public:
    DenseTrack();
    ~DenseTrack();    
    //int Tracker(bool show_track, std::string fileName, bool parametros, int FrameInicio, int FrameFinal);
    //int Tracker(bool show_track, std::string fileName, bool parametros, int start_frame=0, int end_frame=INT_MAX, int track_length = 15, int min_distance = 5, int patch_size = 23, int nxy_cell = 2, int nt_cell = 3, int scale_num = 8, int init_gap = 1);
    int Tracker(bool show_track, std::string fileName, bool parametros, int start_frame=0, int end_frame=INT_MAX, int track_length = 15, int min_distance = 5, int patch_size = 23, int nxy_cell = 2, int nt_cell = 3, int scale_num = 8, int init_gap = 1);
    int TrackerDirectory(int Homo_X, int Homo_Y, bool show_track, std::string dirName, bool parametros, int start_frame=0, int end_frame=INT_MAX, int track_length = 15, int min_distance = 5, int patch_size = 23, int nxy_cell = 2, int nt_cell = 3, int scale_num = 8, int init_gap = 1);
    //int Tracker(bool show_track, std::string fileName, bool parametros);
};

#endif // DENSETRACK_H
