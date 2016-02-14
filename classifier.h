#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <opencv2/opencv.hpp>
#include "highgui.h"
#include "ml.h"

class Classifier
{
public:
    Classifier();
    ~Classifier();
    void svm(cv::Mat& trainingData, cv::Mat& trainingClasses, cv::Mat& testData, cv::Mat& testClasses);
private:
    float evaluate(cv::Mat& predicted, cv::Mat& actual);
    void plot_binary(cv::Mat& data, cv::Mat& classes, std::string name);
};

#endif // CLASSIFIER_H
