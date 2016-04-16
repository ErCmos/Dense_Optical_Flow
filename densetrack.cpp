#include "densetrack.h"
#include "initialize.h"
#include "descriptors.h"
#include "opticalflow.h"
#include "cameramotion.h"

#include <QMessageBox>

DenseTrack::DenseTrack()
{

}

DenseTrack::~DenseTrack()
{

}

using namespace cv;
using namespace constantes;
using namespace std;

int DenseTrack::Tracker(bool show_track, std::string fileName, bool parametros, int start_frame, int end_frame, int track_length, int min_distance, int patch_size, int nxy_cell, int nt_cell, int scale_num, int init_gap)
{
    char *video = new char[fileName.length() + 1];
    strcpy(video, fileName.c_str());

    VideoCapture capture;
    capture.open(video);

    if(!capture.isOpened())
    {
        QMessageBox msgBox;
        msgBox.setText("Could not initialize capturing..");
        //msgBox.setInformativeText("Do you want to save your changes?");
        //msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        //msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        //printf( "Could not initialize capturing..\n" );
        return -1;
    }
    int frame_num = 0;
    TrackInfo trackInfo;
    DescInfo hogInfo, hofInfo, mbhInfo;

    char *salida = new char;

    InitTrackInfo(&trackInfo, track_length, init_gap);
    InitDescInfo(&hogInfo, 8, false, patch_size, nxy_cell, nt_cell);
    InitDescInfo(&hofInfo, 9, true, patch_size, nxy_cell, nt_cell);
    InitDescInfo(&mbhInfo, 8, false, patch_size, nxy_cell, nt_cell);

    SeqInfo seqInfo;
    InitSeqInfo(&seqInfo, video);

    if(parametros)
        seqInfo.length = end_frame - start_frame + 1;

    if(show_track)
        namedWindow("DenseTrack", 0);

    Mat image, prev_grey, grey;

        std::vector<float> fscales(0);
        std::vector<Size> sizes(0);

        std::vector<Mat> prev_grey_pyr(0), grey_pyr(0), flow_pyr(0);
        std::vector<Mat> prev_poly_pyr(0), poly_pyr(0); // for optical flow

        std::vector<std::list<Track> > xyScaleTracks;
        int init_counter = 0; // indicate when to detect new feature points
        while(true) {
            Mat frame;
            int i, j, c;

            // get a new frame
            capture >> frame;
            if(frame.empty())
                break;

            if(frame_num < start_frame || frame_num > end_frame) {
                frame_num++;
                continue;
            }

            if(frame_num == start_frame) {
                image.create(frame.size(), CV_8UC3);
                grey.create(frame.size(), CV_8UC1);
                prev_grey.create(frame.size(), CV_8UC1);

                InitPry(frame, fscales, sizes); //fscales Scale Factors correspond to sizes images from the original frame

                BuildPry(sizes, CV_8UC1, prev_grey_pyr);    //Generate the necesary images and structures for each scale
                BuildPry(sizes, CV_8UC1, grey_pyr);

                BuildPry(sizes, CV_32FC2, flow_pyr);
                BuildPry(sizes, CV_32FC(5), prev_poly_pyr);
                BuildPry(sizes, CV_32FC(5), poly_pyr);

                xyScaleTracks.resize(constantes::scale_num);

                frame.copyTo(image);                        //Copy actual frame to image
                cvtColor(image, prev_grey, CV_BGR2GRAY);    //copy image as grey in prev_grey

                for(int iScale = 0; iScale < constantes::scale_num; iScale++) {
                    if(iScale == 0)
                        prev_grey.copyTo(prev_grey_pyr[0]); //Original image
                    else
                        resize(prev_grey_pyr[iScale-1], prev_grey_pyr[iScale], prev_grey_pyr[iScale].size(), 0, 0, INTER_LINEAR);

                    // dense sampling feature points
                    std::vector<Point2f> points(0);
                    DenseSample(prev_grey_pyr[iScale], points, quality, min_distance);

                    // save the feature points
                    std::list<Track>& tracks = xyScaleTracks[iScale];
                    for(i = 0; i < points.size(); i++)
                        tracks.push_back(Track(points[i], trackInfo, hogInfo, hofInfo, mbhInfo));
                }

                // compute polynomial expansion
                my::FarnebackPolyExpPyr(prev_grey, prev_poly_pyr, fscales, 7, 1.5);

                frame_num++;
                continue;
            }//First Frame
            //Next Frames
            init_counter++;
            frame.copyTo(image);
            cvtColor(image, grey, CV_BGR2GRAY);

            // compute optical flow for all scales once
            my::FarnebackPolyExpPyr(grey, poly_pyr, fscales, 7, 1.5);
            my::calcOpticalFlowFarneback(prev_poly_pyr, poly_pyr, flow_pyr, 10, 2);

            for(int iScale = 0; iScale < constantes::scale_num; iScale++) {
                if(iScale == 0)
                    grey.copyTo(grey_pyr[0]);
                else
                    resize(grey_pyr[iScale-1], grey_pyr[iScale], grey_pyr[iScale].size(), 0, 0, INTER_LINEAR);

                int width = grey_pyr[iScale].cols;
                int height = grey_pyr[iScale].rows;

                // compute the integral histograms
                DescMat* hogMat = InitDescMat(height+1, width+1, hogInfo.nBins);
                HogComp(prev_grey_pyr[iScale], hogMat->desc, hogInfo);

                DescMat* hofMat = InitDescMat(height+1, width+1, hofInfo.nBins);
                HofComp(flow_pyr[iScale], hofMat->desc, hofInfo);

                DescMat* mbhMatX = InitDescMat(height+1, width+1, mbhInfo.nBins);
                DescMat* mbhMatY = InitDescMat(height+1, width+1, mbhInfo.nBins);
                MbhComp(flow_pyr[iScale], mbhMatX->desc, mbhMatY->desc, mbhInfo);

                // track feature points in each scale separately
                std::list<Track>& tracks = xyScaleTracks[iScale];
                for (std::list<Track>::iterator iTrack = tracks.begin(); iTrack != tracks.end();) {
                    int index = iTrack->index;
                    Point2f prev_point = iTrack->point[index];
                    int x = std::min<int>(std::max<int>(cvRound(prev_point.x), 0), width-1);
                    int y = std::min<int>(std::max<int>(cvRound(prev_point.y), 0), height-1);

                    Point2f point;
                    point.x = prev_point.x + flow_pyr[iScale].ptr<float>(y)[2*x];
                    point.y = prev_point.y + flow_pyr[iScale].ptr<float>(y)[2*x+1];

                    if(point.x <= 0 || point.x >= width || point.y <= 0 || point.y >= height) {
                        iTrack = tracks.erase(iTrack);
                        continue;
                    }

                    // get the descriptors for the feature point
                    RectInfo rect;
                    GetRect(prev_point, rect, width, height, hogInfo);
                    GetDesc(hogMat, rect, hogInfo, iTrack->hog, index);
                    GetDesc(hofMat, rect, hofInfo, iTrack->hof, index);
                    GetDesc(mbhMatX, rect, mbhInfo, iTrack->mbhX, index);
                    GetDesc(mbhMatY, rect, mbhInfo, iTrack->mbhY, index);
                    iTrack->addPoint(point);

                    // draw the trajectories at the first scale
                    if(show_track == 1 && iScale == 0)
                        DrawTrack(iTrack->point, iTrack->index, fscales[iScale], image);

                    // if the trajectory achieves the maximal length
                    if(iTrack->index >= trackInfo.length) {
                        std::vector<Point2f> trajectory(trackInfo.length+1);
                        for(int i = 0; i <= trackInfo.length; ++i)
                            trajectory[i] = iTrack->point[i]*fscales[iScale];

                        float mean_x(0), mean_y(0), var_x(0), var_y(0), length(0);

                        if(IsValid(trajectory, mean_x, mean_y, var_x, var_y, length))
                        {
                                FILE *stream ;
                                string salida=fileName.substr(0,fileName.find_last_of("."))+".txt";
                                   if((stream = freopen(salida.c_str(), "a+", stdout)) == NULL)
                                      exit(-1);

                                //   freopen( "stdin.log", "w", stdin );
                                //   freopen( "stdout.log", "w", stdout );
                                //   freopen( "stderr.log", "w", stderr );
                                //   stream = freopen("CON", "w", stdout); //To return to console

                                printf("%d\t%f\t%f\t%f\t%f\t%f\t%f\t", frame_num, mean_x, mean_y, var_x, var_y, length, fscales[iScale]);
                                // for spatio-temporal pyramid
                                printf("%f\t", std::min<float>(std::max<float>(mean_x/float(seqInfo.width), 0), 0.999));
                                printf("%f\t", std::min<float>(std::max<float>(mean_y/float(seqInfo.height), 0), 0.999));
                                printf("%f\t", std::min<float>(std::max<float>((frame_num - trackInfo.length/2.0 - start_frame)/float(seqInfo.length), 0), 0.999));

                                // output the trajectory
                                for (int i = 0; i < trackInfo.length; ++i)
                                     printf("%f\t%f\t", trajectory[i].x,trajectory[i].y);

                                PrintDesc(iTrack->hog, hogInfo, trackInfo);
                                PrintDesc(iTrack->hof, hofInfo, trackInfo);
                                PrintDesc(iTrack->mbhX, mbhInfo, trackInfo);
                                PrintDesc(iTrack->mbhY, mbhInfo, trackInfo);
                                printf("\n");

                                fclose(stream);
                        }

                        //free(salida);
                        iTrack = tracks.erase(iTrack);
                        continue;
                    }
                    ++iTrack;
                }
                ReleDescMat(hogMat);
                ReleDescMat(hofMat);
                ReleDescMat(mbhMatX);
                ReleDescMat(mbhMatY);

                if(init_counter != trackInfo.gap)
                    continue;

                // detect new feature points every initGap frames
                std::vector<Point2f> points(0);
                for(std::list<Track>::iterator iTrack = tracks.begin(); iTrack != tracks.end(); iTrack++)
                    points.push_back(iTrack->point[iTrack->index]);

                DenseSample(grey_pyr[iScale], points, quality, min_distance);
                // save the new feature points
                for(i = 0; i < points.size(); i++)
                    tracks.push_back(Track(points[i], trackInfo, hogInfo, hofInfo, mbhInfo));
            }

            init_counter = 0;
            grey.copyTo(prev_grey);
            for(i = 0; i < constantes::scale_num; i++) {
                grey_pyr[i].copyTo(prev_grey_pyr[i]);
                poly_pyr[i].copyTo(prev_poly_pyr[i]);
            }

            frame_num++;

            //if( show_track == 1 ) {
            if(show_track) {
                imshow( "DenseTrack", image);
                c = cvWaitKey(3);
                if((char)c == 27) break;
            }
        }

        //if( show_track == 1 )
        if(show_track)
            destroyWindow("DenseTrack");

        return 0;
    delete [] video;
    free(salida);
}

int DenseTrack::TrackerDirectory(int Homo_X, int Homo_Y, bool show_track, std::string dirName, bool parametros, int start_frame, int end_frame, int track_length, int min_distance, int patch_size, int nxy_cell, int nt_cell, int scale_num, int init_gap)
{
    std::string fileName;
    string action;

    if(dirName.find_last_of("/") != std::string::npos)
            action = dirName.substr(dirName.find_last_of("/")+1);

    DIR *pDIR;
    struct dirent *entry;
    clock_t dir_begin=clock();
    if(pDIR=opendir(dirName.c_str()))
    {
        while(entry=readdir((pDIR)))
        {
            if(strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,".."))
            {
                string Name=entry->d_name;
                if(Name.substr(Name.find_first_of(".")+1) == "avi")
                {
                    clock_t file_begin=clock();
                    fileName=dirName+"/"+Name;
                    char *video = new char[fileName.length() + 1];
                    strcpy(video, fileName.c_str());

                    VideoCapture capture;
                    capture.open(video);

                    if(!capture.isOpened())
                    {
                        QMessageBox msgBox;
                        msgBox.setText("Could not initialize capturing..");
                        //msgBox.setInformativeText("Do you want to save your changes?");
                        //msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
                        //msgBox.setDefaultButton(QMessageBox::Save);
                        int ret = msgBox.exec();
                        //printf( "Could not initialize capturing..\n" );
                        return -1;
                    }
                    int frame_num = 0;
                    TrackInfo trackInfo;
                    DescInfo hogInfo, hofInfo, mbhInfo;

                    char *salida = new char;

                    InitTrackInfo(&trackInfo, track_length, init_gap);
                    InitDescInfo(&hogInfo, 8, false, patch_size, nxy_cell, nt_cell);
                    InitDescInfo(&hofInfo, 9, true, patch_size, nxy_cell, nt_cell);
                    InitDescInfo(&mbhInfo, 8, false, patch_size, nxy_cell, nt_cell);

                    SeqInfo seqInfo;
                    InitSeqInfo(&seqInfo, video);

                    if(parametros)
                        seqInfo.length = end_frame - start_frame + 1;

                    if(show_track)
                        namedWindow("DenseTrack", 0);

                    Mat image, prev_grey, grey;

                        std::vector<float> fscales(0);
                        std::vector<Size> sizes(0);

                        std::vector<Mat> prev_grey_pyr(0), grey_pyr(0), flow_pyr(0);
                        std::vector<Mat> prev_poly_pyr(0), poly_pyr(0); // for optical flow

                        std::vector<std::list<Track> > xyScaleTracks;
                        int init_counter = 0; // indicate when to detect new feature points
                        while(true) {
                            Mat frame;
                            int i, j, c;

                            // get a new frame
                            capture >> frame;
                            if(frame.empty())
                                break;

                            if(frame_num < start_frame || frame_num > end_frame) {
                                frame_num++;
                                continue;
                            }

                            if(frame_num == start_frame) {
                                image.create(frame.size(), CV_8UC3);
                                grey.create(frame.size(), CV_8UC1);
                                prev_grey.create(frame.size(), CV_8UC1);

                                InitPry(frame, fscales, sizes); //fscales Scale Factors correspond to sizes images from the original frame

                                BuildPry(sizes, CV_8UC1, prev_grey_pyr);    //Generate the necesary images and structures for each scale
                                BuildPry(sizes, CV_8UC1, grey_pyr);

                                BuildPry(sizes, CV_32FC2, flow_pyr);
                                BuildPry(sizes, CV_32FC(5), prev_poly_pyr);
                                BuildPry(sizes, CV_32FC(5), poly_pyr);

                                xyScaleTracks.resize(constantes::scale_num);

                                frame.copyTo(image);                        //Copy actual frame to image
                                cvtColor(image, prev_grey, CV_BGR2GRAY);    //copy image as grey in prev_grey

                                for(int iScale = 0; iScale < constantes::scale_num; iScale++) {
                                    if(iScale == 0)
                                        prev_grey.copyTo(prev_grey_pyr[0]); //Original image
                                    else
                                        resize(prev_grey_pyr[iScale-1], prev_grey_pyr[iScale], prev_grey_pyr[iScale].size(), 0, 0, INTER_LINEAR);

                                    // dense sampling feature points
                                    std::vector<Point2f> points(0);
                                    DenseSample(prev_grey_pyr[iScale], points, quality, min_distance);

                                    // save the feature points
                                    std::list<Track>& tracks = xyScaleTracks[iScale];
                                    for(i = 0; i < points.size(); i++)
                                        tracks.push_back(Track(points[i], trackInfo, hogInfo, hofInfo, mbhInfo));
                                }

                                // compute polynomial expansion
                                my::FarnebackPolyExpPyr(prev_grey, prev_poly_pyr, fscales, 7, 1.5);

                                frame_num++;
                                continue;
                            }//First Frame
                            //Next Frames
                            init_counter++;
                            frame.copyTo(image);
                            cvtColor(image, grey, CV_BGR2GRAY);

                            // compute optical flow for all scales once
                            my::FarnebackPolyExpPyr(grey, poly_pyr, fscales, 7, 1.5);
                            my::calcOpticalFlowFarneback(prev_poly_pyr, poly_pyr, flow_pyr, 10, 2);

                            for(int iScale = 0; iScale < constantes::scale_num; iScale++) {
                                if(iScale == 0)
                                    grey.copyTo(grey_pyr[0]);
                                else
                                    resize(grey_pyr[iScale-1], grey_pyr[iScale], grey_pyr[iScale].size(), 0, 0, INTER_LINEAR);

                                int width = grey_pyr[iScale].cols;
                                int height = grey_pyr[iScale].rows;

                                // compute the integral histograms
                                DescMat* hogMat = InitDescMat(height+1, width+1, hogInfo.nBins);
                                HogComp(prev_grey_pyr[iScale], hogMat->desc, hogInfo);

                                DescMat* hofMat = InitDescMat(height+1, width+1, hofInfo.nBins);
                                HofComp(flow_pyr[iScale], hofMat->desc, hofInfo);

                                DescMat* mbhMatX = InitDescMat(height+1, width+1, mbhInfo.nBins);
                                DescMat* mbhMatY = InitDescMat(height+1, width+1, mbhInfo.nBins);
                                MbhComp(flow_pyr[iScale], mbhMatX->desc, mbhMatY->desc, mbhInfo);

                                // track feature points in each scale separately
                                std::list<Track>& tracks = xyScaleTracks[iScale];
                                for (std::list<Track>::iterator iTrack = tracks.begin(); iTrack != tracks.end();) {
                                    int index = iTrack->index;
                                    Point2f prev_point = iTrack->point[index];
                                    int x = std::min<int>(std::max<int>(cvRound(prev_point.x), 0), width-1);
                                    int y = std::min<int>(std::max<int>(cvRound(prev_point.y), 0), height-1);

                                    Point2f point;
                                    point.x = prev_point.x + flow_pyr[iScale].ptr<float>(y)[2*x];
                                    point.y = prev_point.y + flow_pyr[iScale].ptr<float>(y)[2*x+1];

                                    if(point.x <= 0 || point.x >= width || point.y <= 0 || point.y >= height) {
                                        iTrack = tracks.erase(iTrack);
                                        continue;
                                    }

                                    // get the descriptors for the feature point
                                    RectInfo rect;
                                    GetRect(prev_point, rect, width, height, hogInfo);
                                    GetDesc(hogMat, rect, hogInfo, iTrack->hog, index);
                                    GetDesc(hofMat, rect, hofInfo, iTrack->hof, index);
                                    GetDesc(mbhMatX, rect, mbhInfo, iTrack->mbhX, index);
                                    GetDesc(mbhMatY, rect, mbhInfo, iTrack->mbhY, index);
                                    iTrack->addPoint(point);

                                    // draw the trajectories at the first scale
                                    if(show_track == 1 && iScale == 0)
                                        DrawTrack(iTrack->point, iTrack->index, fscales[iScale], image);

                                    // if the trajectory achieves the maximal length
                                    if(iTrack->index >= trackInfo.length) {
                                        std::vector<Point2f> trajectory(trackInfo.length+1);
                                        for(int i = 0; i <= trackInfo.length; ++i)
                                            trajectory[i] = iTrack->point[i]*fscales[iScale];

                                        float mean_x(0), mean_y(0), var_x(0), var_y(0), length(0);

                                        if(IsValid(trajectory, mean_x, mean_y, var_x, var_y, length))
                                        {
                                                FILE *stream ;
                                                string salida=fileName.substr(0,fileName.find_last_of("."))+".txt";
                                                   if((stream = freopen(salida.c_str(), "a+", stdout)) == NULL)
                                                      exit(-1);

                                                //   freopen( "stdin.log", "w", stdin );
                                                //   freopen( "stdout.log", "w", stdout );
                                                //   freopen( "stderr.log", "w", stderr );
                                                //   stream = freopen("CON", "w", stdout); //To return to console

                                                printf("%d\t%f\t%f\t%f\t%f\t%f\t%f\t", frame_num, mean_x, mean_y, var_x, var_y, length, fscales[iScale]);
                                                // for spatio-temporal pyramid
                                                printf("%f\t", std::min<float>(std::max<float>(mean_x/float(seqInfo.width), 0), 0.999));
                                                printf("%f\t", std::min<float>(std::max<float>(mean_y/float(seqInfo.height), 0), 0.999));
                                                printf("%f\t", std::min<float>(std::max<float>((frame_num - trackInfo.length/2.0 - start_frame)/float(seqInfo.length), 0), 0.999));

                                                // output the trajectory
                                                for (int i = 0; i < trackInfo.length; ++i)
                                                     printf("%f\t%f\t", trajectory[i].x,trajectory[i].y);

                                                PrintDesc(iTrack->hog, hogInfo, trackInfo);
                                                PrintDesc(iTrack->hof, hofInfo, trackInfo);
                                                PrintDesc(iTrack->mbhX, mbhInfo, trackInfo);
                                                PrintDesc(iTrack->mbhY, mbhInfo, trackInfo);
                                                printf("\n");

                                                fclose(stream);
                                        }

                                        iTrack = tracks.erase(iTrack);
                                        continue;
                                    }
                                    ++iTrack;
                                }
                                ReleDescMat(hogMat);
                                ReleDescMat(hofMat);
                                ReleDescMat(mbhMatX);
                                ReleDescMat(mbhMatY);

                                if(init_counter != trackInfo.gap)
                                    continue;

                                // detect new feature points every initGap frames
                                std::vector<Point2f> points(0);
                                for(std::list<Track>::iterator iTrack = tracks.begin(); iTrack != tracks.end(); iTrack++)
                                    points.push_back(iTrack->point[iTrack->index]);

                                DenseSample(grey_pyr[iScale], points, quality, min_distance);
                                // save the new feature points
                                for(i = 0; i < points.size(); i++)
                                    tracks.push_back(Track(points[i], trackInfo, hogInfo, hofInfo, mbhInfo));
                            }

                            init_counter = 0;
                            grey.copyTo(prev_grey);
                            for(i = 0; i < constantes::scale_num; i++) {
                                grey_pyr[i].copyTo(prev_grey_pyr[i]);
                                poly_pyr[i].copyTo(prev_poly_pyr[i]);
                            }

                            frame_num++;

                            //if( show_track == 1 ) {
                            if(show_track) {
                                imshow( "DenseTrack", image);
                                c = cvWaitKey(3);
                                if((char)c == 27) break;
                            }
                        }

                        //if( show_track == 1 )
                        if(show_track)
                            destroyWindow("DenseTrack");

                        //return 0;
                    delete [] video;
                    free(salida);

                    clock_t file_end=clock();
                    cout << "Time elapsed by file procesing: " << double((file_end-file_begin)/ CLOCKS_PER_SEC) << " s"<< endl;

                    /////////////////////Homography & Slicing Calculation///////////////////////
                    vector<Mat> blocks,vec_H, vec_L; //, vec_LTS;
                    CameraMotion motion;

                    if ((Homo_X!=1) || (Homo_Y!=1))
                    {
                        motion.subdivide(fileName,Homo_Y,Homo_X,blocks);
                        for (int n=0; n<(Homo_Y*Homo_X);n++)
                        {
                            vector<Mat> outputVideo;
                     /////////////////////////////////////
                                for (int l=0; l<blocks.size(); l=l+(Homo_Y*Homo_X))
                                {
                                   outputVideo.push_back(blocks.at(n+l));
                                }
                                /*
                                for (int jj=0;jj<outputVideo.size();++jj)
                                {
                                    imshow("1",outputVideo.at(jj));
                                    if(cv::waitKey(30) >= 0) break;
                                    //waitKey(20); // waits to display frame
                                }
                                */
                                motion.get_homography_sequence(outputVideo,vec_H);
                                for (int i=0;i<vec_H.size(); i++)
                                {
                                    Mat L = motion.compute_Liecoeffs (vec_H[i]);
                                    vec_L.push_back(L);
                                }
                                motion.save_vectors (vec_L, fileName);
                                motion.DOF_LIE(fileName);
                  ////////////////////////////////////////////////
                        }
                    }
                    else
                    {
                        //motion.get_homography_sequence(fileName,vec_H);
                        char *video = new char[fileName.length() + 1];
                        strcpy(video, fileName.c_str());

                        VideoCapture capture;
                        capture.open(video);
                        Mat frame;
                        vector<Mat> outputVideo;

                        if(!capture.isOpened())
                        {
                            QMessageBox msgBox;
                            msgBox.setText("Could not initialize capturing..");
                            //msgBox.setInformativeText("Do you want to save your changes?");
                            //msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
                            //msgBox.setDefaultButton(QMessageBox::Save);
                            int ret = msgBox.exec();
                            //printf( "Could not initialize capturing..\n" );
                            return -1;
                        }
                        for(;;)
                        {
                            capture >> frame;
                            Mat tmp=frame.clone();  //Clone the frame or allways points the same one
                            if (frame.empty())
                                break;
                            outputVideo.push_back(tmp);
                        }
                        motion.get_homography_sequence(outputVideo,vec_H);
                        for (int i=0;i<vec_H.size(); i++)
                        {
                            Mat L = motion.compute_Liecoeffs (vec_H[i]);
                            vec_L.push_back(L);
                        }

                        /* Output timeseries of Lie coefficients */
                //        motion.construct_timeseries (vec_L, vec_LTS);
                        /* save vectors in output files */
                        motion.save_vectors (vec_L, fileName);
                        motion.DOF_LIE(fileName);
                    }
                    ////////////////////////////////////////////////////////////////////////////
                }
            }
        }
    }
    clock_t dir_end=clock();
    cout << "Time elapsed by directory procesing: " << double((dir_end-dir_begin)/ CLOCKS_PER_SEC) << " s"<< endl;
    return 0;
}

