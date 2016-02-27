#include "classifier.h"

Classifier::Classifier()
{

}

Classifier::~Classifier()
{

}

using namespace std;
using namespace cv;

bool plotSupportVectors=true;
//int numTrainingPoints=200;
//int numTestPoints=2000;
int size=200;
//int eq=0;


//void Classifier::svm(cv::Mat& trainingData, cv::Mat& trainingClasses, cv::Mat& testData, cv::Mat& testClasses)
void Classifier::svm_train(cv::Mat& trainingData, cv::Mat& trainingClasses, std::string fileName)
{
    CvSVMParams param = CvSVMParams();

    param.svm_type = CvSVM::C_SVC;
    param.kernel_type = CvSVM::RBF; //CvSVM::RBF, CvSVM::LINEAR ...
    param.degree = 0; // for poly
    param.gamma = 20; // for poly/rbf/sigmoid
    param.coef0 = 0; // for poly/sigmoid

    param.C = 7; // for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
    param.nu = 0.0; // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
    param.p = 0.0; // for CV_SVM_EPS_SVR

    param.class_weights = NULL; // for CV_SVM_C_SVC
    param.term_crit.type = CV_TERMCRIT_ITER +CV_TERMCRIT_EPS;
    param.term_crit.max_iter = 1000;
    param.term_crit.epsilon = 1e-6;

    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

    //cout << "Clases: " << trainingClasses << endl;

    // SVM training (use train auto for OpenCV>=2.0)
    CvSVM svm(trainingData, trainingClasses, cv::Mat(), cv::Mat(), params);

    svm.save(fileName.c_str());
//    cout << "data=" << trainingClasses.data << endl;
//    CvSVM svm2(testData, testClasses, cv::Mat(), cv::Mat(), params);
/*
    cv::Mat predicted(testClasses.rows, 1, CV_32FC1);

    for(int i = 0; i < testData.rows; i++) {
        cv::Mat sample = testData.row(i);

        float x = sample.at<float>(0,0);
        float y = sample.at<float>(0,1);

        predicted.at<float>(i, 0) = svm.predict(sample);
    }

    cout << "Accuracy_{SVM} = " << evaluate(predicted, testClasses) << endl;
    plot_binary(testData, predicted, "Predictions SVM");

    // plot support vectors
    if(plotSupportVectors) {
        cv::Mat plot_sv(size, size, CV_8UC3);
        plot_sv.setTo(cv::Scalar(255.0,255.0,255.0));

        int svec_count = svm.get_support_vector_count();
        for(int vecNum = 0; vecNum < svec_count; vecNum++) {
            const float* vec = svm.get_support_vector(vecNum);
            cv::circle(plot_sv, Point(vec[0]*size, vec[1]*size), 3 , CV_RGB(0, 0, 0));
        }
    cv::imshow("Support Vectors", plot_sv);
    }
*/
}

void Classifier::svm_test(cv::Mat& testData, cv::Mat& testClasses, std::string fileName)
{
    // SVM load
    CvSVM svm;

    svm.load(fileName.c_str());

    float pred=svm.predict(testData);

    cv::Mat predicted(testClasses.rows, 1, CV_32FC1);

    for(int i = 0; i < testData.rows; i++)
    {
        cv::Mat sample = testData.row(i);

        float x = sample.at<float>(0,0);
        float y = sample.at<float>(0,1);

        predicted.at<float>(i, 0) = svm.predict(sample);
    }

    cout << "Accuracy_{SVM} = " << evaluate(predicted, testClasses) << endl;
    plot_binary(testData, predicted, "Predictions SVM");
}

// accuracy
float Classifier::evaluate(cv::Mat& predicted, cv::Mat& actual)
{
        assert(predicted.rows == actual.rows);
        int t = 0;
        int f = 0;
        for(int i = 0; i < actual.rows; i++) {
                float p = predicted.at<float>(i,0);
                float a = actual.at<float>(i,0);
                if((p >= 0.0 && a >= 0.0) || (p <= 0.0 &&  a <= 0.0)) {
                        t++;
                } else {
                        f++;
                }
        }
        return (t * 1.0) / (t + f);
}

// plot data and class
void Classifier::plot_binary(cv::Mat& data, cv::Mat& classes, string name)
{
        cv::Mat plot(size, size, CV_8UC3);
        plot.setTo(cv::Scalar(255.0,255.0,255.0));
        for(int i = 0; i < data.rows; i++) {

                float x = data.at<float>(i,0) * size;
                float y = data.at<float>(i,1) * size;

                if(classes.at<float>(i, 0) > 0) {
                        cv::circle(plot, Point(x,y), 2, CV_RGB(255,0,0),1);
                } else {
                        cv::circle(plot, Point(x,y), 2, CV_RGB(0,255,0),1);
                }
        }
        cv::imshow(name, plot);
}
