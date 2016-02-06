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

///////////////////////////Visualiza Dense_Optical_Flow////////////////////
int Video_Player::Player_DOF(std::string filename, std::string WindowName)
{
    IplImage* image = 0;
    IplImage* prev_image = 0;
    CvCapture* capture = 0;

    int show = 1;
    int frameNum = 0;

    char *cstr = new char[filename.length() + 1];
    strcpy(cstr, filename.c_str());
    char *cstr2 = new char[WindowName.length() + 1];
    strcpy(cstr2, WindowName.c_str());
    //char* video = filename;
    //capture = cvCreateFileCapture(video);
    capture = cvCreateFileCapture(cstr);

    if( !capture ) {
        QMessageBox msgBox;
        msgBox.setText("Could not initialize capturing..");
        //msgBox.setInformativeText("Do you want to save your changes?");
        //msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        //msgBox.setDefaultButton(QMessageBox::Save);
        //int ret = msgBox.exec();
        //printf( "Could not initialize capturing..\n" );
        return -1;
    }

    if( show == 1 ){
        cvNamedWindow( cstr2, 0 );
    }

    while( true ) {
        IplImage* frame = 0;
        int i, j, c;

        // get a new frame
        frame = cvQueryFrame( capture );
        if( !frame )
            break;

        if( !image ) {
            image =  cvCreateImage( cvSize(frame->width,frame->height), 8, 3 );
            image->origin = frame->origin;
        }

        cvCopy( frame, image, 0 );

        if( show == 1 ) {
            cvShowImage( cstr2, image);
            c = cvWaitKey(3);
            if((char)c == 27) break;
        }

        std::cerr << "The " << frameNum << "-th frame" << std::endl;
        frameNum++;
    }

    if( show == 1 ){
        cvDestroyWindow(cstr2);
    delete [] cstr;
    delete [] cstr2;
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////////
