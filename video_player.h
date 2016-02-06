#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <string.h>
//MessageBox
#include <QMessageBox>

class Video_Player
{
public:
    Video_Player();
    ~Video_Player();
    cv::VideoCapture Capturer(std::string fileName);
    void Player(cv::VideoCapture cap, std::string WindowName);
    int Player_DOF(std::string fileName, std::string WindowName);
};

#endif // VIDEO_PLAYER_H
