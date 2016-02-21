#ifndef OFDE_H
#define OFDE_H

#include "cv.hpp"
#include "stdio.h"

class OFDE : public cv::DescriptorExtractor
{
public:
    OFDE();
    ~OFDE();
    void compute(const std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint> > &keypoints, std::vector<cv::Mat> &descriptors) const;
protected:
    void computeImpl(const std::vector<cv::Mat> &images, std::vector<std::vector<cv::KeyPoint> > &keypoints, std::vector<cv::Mat> &descriptors) const;
};

#endif // OFDE_H
