#ifndef CAMERAMOTION_H
#define CAMERAMOTION_H

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

#include <iostream>
#include <vector>
#include <stdio.h>

class CameraMotion
{
public:
    CameraMotion();
    ~CameraMotion();
    int get_homography_sequence (std::string fileName, std::vector<cv::Mat>& vec_H);
    void warpKeypoints(const cv::Mat& H, const std::vector<cv::KeyPoint>& in, std::vector<cv::KeyPoint>& out);
    void matches2points(const std::vector<cv::KeyPoint>& train, const std::vector<cv::KeyPoint>& query, const std::vector<cv::DMatch>& matches, std::vector<cv::Point2f>& pts_train, std::vector<cv::Point2f>& pts_query);
    void resetH(cv::Mat&H);
    void keypoints2points(const std::vector<cv::KeyPoint>& in, std::vector<cv::Point2f>& out);
    void points2keypoints(const std::vector<cv::Point2f>& in, std::vector<cv::KeyPoint>& out);
    cv::Mat compute_Liecoeffs (const cv::Mat& h);
    void construct_timeseries (const std::vector<cv::Mat>& vec_L, std::vector <cv::Mat>& vec_LTS);
    //void save_vectors (const std::vector<cv::Mat>& x, char *fname);
    void save_vectors (const std::vector<cv::Mat>& x, std::string fileName);
    void DOF_LIE (std::string fileName, int level);
    int subdivide(std::string fileName, const int rowDivisor, const int colDivisor, std::vector<cv::Mat> &blocks);
};

#endif // CAMERAMOTION_H
