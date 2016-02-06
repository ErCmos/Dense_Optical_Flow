#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <opencv2/opencv.hpp>
#include <string.h>

class Video_Player
{
public:
    Video_Player();
    ~Video_Player();
    cv::VideoCapture Capturer(std::string fileName);
    void Player(cv::VideoCapture cap, std::string WindowName);
};

#endif // VIDEO_PLAYER_H
