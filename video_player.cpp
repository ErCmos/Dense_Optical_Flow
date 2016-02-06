#include "video_player.h"

Video_Player::Video_Player()
{

}

Video_Player::~Video_Player()
{

}

using namespace std;
using namespace cv;

////////////////////////////////Inicializa Capturador/////////////////////////
VideoCapture Video_Player::Capturer(std::string fileName)
{
    VideoCapture cap;
    cap.open(fileName);
    return cap;
}
//////////////////////////////////////////////////////////////////////////////

/////////////////////// VISUALIZACIÓN /////////////////////////////////////////////////

void Video_Player::Player(VideoCapture cap, string WindowName)
{
    cv::Mat frame;
    if (cap.isOpened())
    {
        for (;;) //while (1)
        {
            cap >> frame;
            if (frame.empty())
                break;
            imshow(WindowName,frame);
            if(cv::waitKey(30) >= 0) break;
        }
    }
}
/////////////////////// FIN VISUALIZACIÓN /////////////////////////////////////////////

