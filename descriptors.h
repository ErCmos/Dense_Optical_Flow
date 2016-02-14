#ifndef DESCRIPTORS
#define DESCRIPTORS

#include "tracker.h"

// get the rectangle for computing the descriptor
void GetRect(const Point2f& point, RectInfo& rect, const int width, const int height, const DescInfo& descInfo);
// compute integral histograms for the whole image
void BuildDescMat(const Mat& xComp, const Mat& yComp, float* desc, const DescInfo& descInfo);
// get a descriptor from the integral histogram
void GetDesc(const DescMat* descMat, RectInfo& rect, DescInfo descInfo, std::vector<float>& desc, const int index);
// for HOG descriptor
void HogComp(const Mat& img, float* desc, DescInfo& descInfo);
// for HOF descriptor
void HofComp(const Mat& flow, float* desc, DescInfo& descInfo);
// for MBH descriptor
void MbhComp(const Mat& flow, float* descX, float* descY, DescInfo& descInfo);
// check whether a trajectory is valid or not
bool IsValid(std::vector<Point2f>& track, float& mean_x, float& mean_y, float& var_x, float& var_y, float& length);
// detect new feature points in an image without overlapping to previous points
void DenseSample(const Mat& grey, std::vector<Point2f>& points, const double quality, const int min_distance);

void InitPry(const Mat& frame, std::vector<float>& scales, std::vector<Size>& sizes);

void BuildPry(const std::vector<Size>& sizes, const int type, std::vector<Mat>& grey_pyr);

void DrawTrack(const std::vector<Point2f>& point, const int index, const float scale, Mat& image);

void PrintDesc(std::vector<float>& desc, DescInfo& descInfo, TrackInfo& trackInfo);
//char * PrintDesc(std::vector<float>& desc, DescInfo& descInfo, TrackInfo& trackInfo);
//float PrintDesc(std::vector<float>& desc, DescInfo& descInfo, TrackInfo& trackInfo);


#endif // DESCRIPTORS

