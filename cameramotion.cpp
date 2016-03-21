#include "cameramotion.h"

#include <QMessageBox>

CameraMotion::CameraMotion()
{

}

CameraMotion::~CameraMotion()
{

}

using namespace cv;
using namespace std;

int CameraMotion::get_homography_sequence (std::string fileName, vector<Mat>& vec_H) {

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

  BriefDescriptorExtractor brief(32);
  Mat frame;

  vector<DMatch> matches;
#if CV_SSSE3
    BruteForceMatcher< HammingSeg<30,4> > desc_matcher;
#else
    BruteForceMatcher<Hamming> desc_matcher;
#endif

  vector<Point2f> train_pts, query_pts;
  vector<KeyPoint> train_kpts, query_kpts;
  vector<unsigned char> match_mask;

  Mat gray;

  Mat train_desc, query_desc;
  const int DESIRED_FTRS = 500;
  GridAdaptedFeatureDetector detector(new FastFeatureDetector(10, true), DESIRED_FTRS, 4, 4);
  Mat H;
  Mat H_prev = Mat::eye(3, 3, CV_64F);
  for (;;)
  {
    capture >> frame;
    if (frame.empty())
      break;

    cvtColor(frame, gray, CV_RGB2GRAY);
    //Find interest points
    detector.detect(gray, query_kpts);
    //Compute brief descriptors at each keypoint location
    brief.compute(gray, query_kpts, query_desc);

    if (!train_kpts.empty()){
      vector<KeyPoint> test_kpts;
      warpKeypoints(H_prev.inv(), query_kpts, test_kpts);

      Mat mask = windowedMatchingMask(test_kpts, train_kpts, 25, 25);
      desc_matcher.match(query_desc, train_desc, matches, mask);

      drawKeypoints(frame, test_kpts, frame, Scalar(255, 0, 0), DrawMatchesFlags::DRAW_OVER_OUTIMG);

      matches2points(train_kpts, query_kpts, matches, train_pts, query_pts);

      if (matches.size() > 5){
        H = findHomography(train_pts, query_pts, RANSAC, 4, match_mask);
        if (countNonZero(Mat(match_mask)) > 15) {
          H_prev = H;
        } else {
          resetH(H_prev);
          //drawMatchesRelative(train_kpts, query_kpts, matches, frame, match_mask);
        }
      } else
        resetH(H_prev);
    } else {
      H_prev = Mat::eye(3, 3, CV_64F);
      Mat out;
      //drawKeypoints(gray, query_kpts, out);
      frame = out;

    }
    vec_H.push_back(H_prev.reshape(0,9).t());
    //imshow("frame", frame);
    //waitKey(2);
    train_kpts = query_kpts;
    query_desc.copyTo(train_desc);
  }
}

//Uses computed homography H to warp original input points to new planar position
void CameraMotion::warpKeypoints(const Mat& H, const vector<KeyPoint>& in, vector<KeyPoint>& out)
{
  vector<Point2f> pts;
  keypoints2points(in, pts);
  vector<Point2f> pts_w(pts.size());
  Mat m_pts_w(pts_w);
  perspectiveTransform(Mat(pts), m_pts_w, H);
  points2keypoints(pts_w, out);
}

//Converts matching indices to xy points
void CameraMotion::matches2points(const vector<KeyPoint>& train, const vector<KeyPoint>& query,
  const std::vector<cv::DMatch>& matches, std::vector<cv::Point2f>& pts_train,
  std::vector<Point2f>& pts_query)
{

  pts_train.clear();
  pts_query.clear();
  pts_train.reserve(matches.size());
  pts_query.reserve(matches.size());

  size_t i = 0;
  for (; i < matches.size(); i++) {
    const DMatch & dmatch = matches[i];
    pts_query.push_back(query[dmatch.queryIdx].pt);
    pts_train.push_back(train[dmatch.trainIdx].pt);
  }
}

void CameraMotion::resetH(Mat&H)
{
  H = Mat::eye(3, 3, CV_64F);
}

//Takes a descriptor and turns it into an xy point
void CameraMotion::keypoints2points(const vector<KeyPoint>& in, vector<Point2f>& out)
{
  out.clear();
  out.reserve(in.size());
  for (size_t i = 0; i < in.size(); ++i)
    out.push_back(in[i].pt);
}

//Takes an xy point and appends that to a keypoint structure
void CameraMotion::points2keypoints(const vector<Point2f>& in, vector<KeyPoint>& out)
{
  out.clear();
  out.reserve(in.size());
  for (size_t i = 0; i < in.size(); ++i)
    out.push_back(KeyPoint(in[i], 1));
}

/*
 Compute Lie space coefficients from homographies: We are using second order
 approximation of M = log(H) = (H-I) + (-1/2) (H-I)'(H-I)
*/
Mat CameraMotion::compute_Liecoeffs (const Mat& h)
{

  /* MATLAB code
  lb(:,:,1) =   [0  0 1 ;0  0 0; 0 0 0]; %G1 -> (1 ;0)
  lb(:,:,2) =   [0  0 0 ;0  0 1; 0 0 0]; %G2 -> (0 ;1)
  lb(:,:,3) = 1*[0 -1 0 ;1  0 0; 0 0 0]; %G3 -> (-y;x)
  lb(:,:,4) = 1*[1  0 0 ;0  1 0; 0 0 0]; %G4 -> (x ;y)size(H_data,1)
  lb(:,:,5) = 1*[1  0 0 ;0 -1 0; 0 0 0]; %G5 -> (x ;-y)
  lb(:,:,6) = 1*[0  1 0 ;1  0 0; 0 0 0]; %G6 -> (y ;x)
  lb(:,:,7) = 1*[0  0 0 ;0  0 0; 1 0 0]; %G7 ->
  lb(:,:,8) = 1*[0  0 0 ;0  0 0; 0 1 0]; %G8 ->
  lb = reshape (lb,[], size(lb,3));

  k = size(H_data,1);
  alpha_data = zeros(k, 8);
  for i=1:k
    h = reshape(H_data(i,:), 3,3) - eye(3);
    h = reshape(h, 9, []);
    alpha_data(i,:) = h'*lb;
  end
  */
  double data[] = {
     0, 0, 0, 1, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 1, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 0,
     0, 0,-1, 0, 0, 1, 0, 0,
     0, 0, 0, 1,-1, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1,
     1, 0, 0, 0, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0};
  Mat lb = Mat(9, 8, h.type(), data).clone();
  Mat L = Mat::zeros(1, 8, h.type());
  Mat I = Mat::eye(3, 3, h.type());
  Mat res;
  /* 1st order approximation */
  /* res = h.reshape (0, 3) - I;*/
  /* 2nd order approximation */
  res = h.reshape(0, 3)-I + -(1/2)*(h.reshape(0,3)-I).t()*(h.reshape(0,3)-I);
  L = res.reshape(0,9).t() *lb;

  res.release();
  lb.release();
  return L;
}

/* Generate Time series of Lie coefficients - This would lead to 8 N-dimensional
vectors (CvMat of 1 row) where N is the total number of frames in the video. */
void CameraMotion::construct_timeseries (const vector<Mat>& vec_L, vector <Mat>& vec_LTS)
{
  int n = vec_L.size();
  int dim = vec_L.back().cols;
  /* column matrix to store time series */
  Mat x = Mat::zeros(n, 1, vec_L.back().type());
  /* reverse iteration to make sure that the first coefficient time series
  comes as first element in the vector vec_LTS() */
  for (int j=0; j<dim; j++){
    for (int i=0; i<n; i++)
      x.at<double>(i,0)= vec_L[i].at<double>(0, j);
    /* put matrix in vector - the transpose operation copies the data */
    vec_LTS.push_back(x.t());
  }
  x.release();
}

//void CameraMotion::save_vectors (const vector<Mat>& x, char *fname)
void CameraMotion::save_vectors (const vector<Mat>& x, std::string fileName)
{
  FILE *fp;
  string salida=fileName.substr(0,fileName.find_last_of("."))+".lie";

  int e_count;

  if((fp = fopen(salida.c_str(), "a+")) == NULL)
     exit(-1);

  for (int v_count=0; v_count < x.size(); v_count++)
  {
    for (e_count =0; e_count < x[v_count].cols-1; e_count++)
        fprintf(fp, "%2.5lf\t", x[v_count].at<double>(0, e_count));
        /* Print last element in the vector - to avoid trailing space */
    fprintf(fp, "%2.5lf\n", x[v_count].at<double>(0, e_count));
  }

  fclose(fp);
}

void CameraMotion::DOF_LIE (std::string fileName, int level)
{
  FILE *f_DOF,*f_LIE,*f_OUT;
  string n_DOF;
  if (level==1)
  {
        n_DOF=fileName.substr(0,fileName.find_last_of("."))+".txt";
  }
  else
  {
        n_DOF=fileName.substr(0,fileName.find_last_of("_"))+".txt";
  }

  string n_LIE=fileName.substr(0,fileName.find_last_of("."))+".lie";
  string n_OUT=fileName.substr(0,fileName.find_last_of("."))+".out";

  if ((f_DOF=fopen (n_DOF.c_str(),"rt"))==NULL)
  {
      QMessageBox msgBox;
      msgBox.setText("Could not initialize TXT file..");
      //msgBox.setInformativeText("Do you want to save your changes?");
      //msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
      //msgBox.setDefaultButton(QMessageBox::Save);
      int ret = msgBox.exec();
      //printf( "Could not initialize capturing..\n" );
      exit(-1);
  }
  if ((f_LIE=fopen (n_LIE.c_str(),"rt"))==NULL)
  {
      QMessageBox msgBox;
      msgBox.setText("Could not initialize LIE file..");
      //msgBox.setInformativeText("Do you want to save your changes?");
      //msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
      //msgBox.setDefaultButton(QMessageBox::Save);
      int ret = msgBox.exec();
      //printf( "Could not initialize capturing..\n" );
      exit(-1);
  }
  if ((f_OUT=fopen (n_OUT.c_str(),"a+"))==NULL)
  {
      QMessageBox msgBox;
      msgBox.setText("Could not initialize OUT file..");
      //msgBox.setInformativeText("Do you want to save your changes?");
      //msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
      //msgBox.setDefaultButton(QMessageBox::Save);
      int ret = msgBox.exec();
      //printf( "Could not initialize capturing..\n" );
      exit(-1);
  }

  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  /*/////////////////////////////////////////////////////*/
      int ch, number_of_lines = 0, number_of_colums = 0;
      read = getline(&line, &len, f_LIE);
      char * pch;
      pch = strtok (line,"\t");
      while (pch != NULL)
      {
        number_of_colums++;
        pch = strtok (NULL, "\t\n");
      }

      free(pch);
      rewind(f_LIE); //rewind the file to count the lines
      do
      {
          ch = fgetc(f_LIE);
          if(ch == '\n')
              number_of_lines++;
      } while (ch != EOF);

  /*////////////////////////////////////////////////////*/
      Mat MatDescriptor_LIE(number_of_lines,number_of_colums,CV_32FC1);

      int rows=0;
      int cols=0;
      rewind(f_LIE);

  while ((read = getline(&line, &len, f_LIE)) != -1)
  {
      //Extract parts separated by "\t"
      char * pch;
      pch = strtok (line,"\t");
      while (pch != NULL)
      {
        float value=atof(pch);
        MatDescriptor_LIE.at<float>(rows,cols)=value;
        pch = strtok (NULL, "\t\n");
        ++cols;
      }
      free(pch);
      ++rows;
      cols=0;
  }

  rows=0;
  cols=0;
  float frame=0;

  while ((read=getline(&line,&len,f_DOF)) != -1)
  {
      //Extract parts separated by "\t"
      char * pch;
      pch = strtok (line,"\t");
      while (pch != NULL)
      {
          float value=atof(pch);
          if (cols==0)
          {
              frame=value-1; //Matrix LIE start (0,0)
          }
          fprintf(f_OUT,"%.7f\t",value);
          pch = strtok (NULL, "\t\n");
          ++cols;
      }
      fprintf(f_OUT,"%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",MatDescriptor_LIE.at<float>(frame,0),MatDescriptor_LIE.at<float>(frame,1),MatDescriptor_LIE.at<float>(frame,2),MatDescriptor_LIE.at<float>(frame,3),MatDescriptor_LIE.at<float>(frame,4),MatDescriptor_LIE.at<float>(frame,5),MatDescriptor_LIE.at<float>(frame,6),MatDescriptor_LIE.at<float>(frame,7));
      free(pch);
      ++rows;
      cols=0;
  }
  fclose(f_DOF);
  fclose(f_LIE);
  fclose(f_OUT);
  remove(n_DOF.c_str());
  remove(n_LIE.c_str());
  rename(n_OUT.c_str(), n_DOF.c_str());
}

// function for splitting image into multiple blocks. rowDivisor and colDivisor specify the number of blocks in rows and cols respectively
int CameraMotion::subdivide(std::string fileName, const int rowDivisor, const int colDivisor, std::vector<cv::Mat> &blocks)
{
    char *video = new char[fileName.length() + 1];
    strcpy(video, fileName.c_str());

    VideoCapture capture;
    capture.open(video);

    /* Checking if the image was passed correctly */
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

    Mat img;
    Size S;
    for (;;)
    {
        capture >> img;
        if (img.empty())
          break;
        S = Size ((int) img.cols / colDivisor,
                        (int) img.rows / rowDivisor) ;
        /* Cloning the image to another for visualization later, if you do not want to visualize the result just comment every line related to visualization */
        cv::Mat maskImg = img.clone();
        /* Checking if the clone image was cloned correctly */
        if(!maskImg.data || maskImg.empty())
            std::cerr << "Problem Loading Image" << std::endl;

        // check if divisors fit to image dimensions
        if(img.cols % colDivisor == 0 && img.rows % rowDivisor == 0)
        {
            for(int y = 0; y < img.cols; y += img.cols / colDivisor)
            {
                for(int x = 0; x < img.rows; x += img.rows / rowDivisor)
                {
                    blocks.push_back(img(cv::Rect(y, x, (img.cols / colDivisor), (img.rows / rowDivisor))).clone());
                    //rectangle(maskImg, Point(y, x), Point(y + (maskImg.cols / colDivisor) - 1, x + (maskImg.rows / rowDivisor) - 1), CV_RGB(255, 0, 0), 1); // visualization

                    //imshow("Image", maskImg); // visualization
                    //waitKey(0); // visualization
                }
            }
        }else if(img.cols % colDivisor != 0)
        {
            cerr << "Error: Please use another divisor for the column split." << endl;
            exit(1);
        }else if(img.rows % rowDivisor != 0)
        {
            cerr << "Error: Please use another divisor for the row split." << endl;
            exit(1);
        }
    }

    VideoWriter outputVideo;
    string NAME;
    //Size S = Size((int) capture.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
    //              (int) capture.get(CV_CAP_PROP_FRAME_HEIGHT));
    // Transform from int to char via Bitwise operators
    int ex = static_cast<int>(capture.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form
    //char EXT[] = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};
    //cout << "Input codec type: " << EXT << endl;
    //int ll=capture.get(CV_CAP_PROP_FRAME_COUNT);

    for (int k=0; k<(colDivisor*rowDivisor); k++)
    {
        stringstream converter;
        converter << k;
        NAME=fileName.substr(0,fileName.find_last_of("."))+"_"+converter.str()+".avi";
        outputVideo.open(NAME.c_str(), CV_FOURCC('X','V','I','D'), capture.get(CV_CAP_PROP_FPS), S, true);
        if(!outputVideo.isOpened())
        {
            QMessageBox msgBox;
            msgBox.setText("Could not initialize recording..");
            //msgBox.setInformativeText("Do you want to save your changes?");
            //msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            //msgBox.setDefaultButton(QMessageBox::Save);
            int ret = msgBox.exec();
            //printf( "Could not initialize capturing..\n" );
            return -1;
        }
        for (int l=0; l<blocks.size(); l=l+(colDivisor*rowDivisor))
        {
            outputVideo << blocks.at(k+l);
        }
        outputVideo.release();
    }
    capture.release();
    return EXIT_SUCCESS;
}
