#include "ofde.h"

using namespace cv;
using namespace std;

OFDE::OFDE()
{

}

OFDE::~OFDE()
{

}

void OFDE::compute(const vector<Mat> &images, vector<vector<KeyPoint> > &keypoints, vector<Mat> &descriptors) const
{
    this->computeImpl(images,keypoints,descriptors);
}

void OFDE::computeImpl(const vector<Mat> &images, vector<vector<KeyPoint> > &keypoints, vector<Mat> &descriptors) const
{

}
