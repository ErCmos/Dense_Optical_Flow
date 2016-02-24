#include "bow.h"

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

BoW::BoW()
{

}

BoW::~BoW()
{

}

//////////////// CrearDiccionario ///////////////////
void BoW::CrearDiccionario(std::string fileName, std::string dirName)
{
    Mat featuresUnclustered,LabelsUnclustered;
    string action;

    if(dirName.find_last_of("/") != std::string::npos)
            action = dirName.substr(dirName.find_last_of("/")+1);

    if(fileName.substr(fileName.find_first_of(".")+1) == "txt")
    {
        FILE * fp;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        string fullName=dirName+"/"+fileName;

        fp = fopen(fullName.c_str(), "r");
        if (fp == NULL)
           exit(EXIT_FAILURE);

    /*/////////////////////////////////////////////////////*/
        int ch, number_of_lines = 0, number_of_colums = 0;
        read = getline(&line, &len, fp);
        char * pch;
        pch = strtok (line,"\t");
        while (pch != NULL)
        {
          number_of_colums++;
          pch = strtok (NULL, "\t");
        }

        free(pch);
        rewind(fp); //rewind the file to count the lines
        do
        {
            ch = fgetc(fp);
            if(ch == '\n')
                number_of_lines++;
        } while (ch != EOF);

    /*////////////////////////////////////////////////////*/
        Mat MatLabels_TMP_SVM(number_of_lines,1,CV_32FC1);
        Mat MatDescriptor_TMP_SVM(number_of_lines,number_of_colums,CV_32FC1);

        std::map<float, std::string> actions;

        // Initialize the map
        actions.insert(std::make_pair(1, "boxing"));
        actions.insert(std::make_pair(2, "handclapping"));
        actions.insert(std::make_pair(3, "handwaving"));
        actions.insert(std::make_pair(4, "jogging"));
        actions.insert(std::make_pair(5, "running"));
        actions.insert(std::make_pair(6, "walking"));
        actions.insert(std::make_pair(7, "test_sequences"));

        int rows=0;
        int cols=0;
        rewind(fp);

        while ((read = getline(&line, &len, fp)) != -1)
        {
            //Extract parts separated by "\t"
            char * pch;
            pch = strtok (line,"\t");
            while (pch != NULL)
            {
              //printf ("%s\n",pch);
              float value=atof(pch);
              MatDescriptor_TMP_SVM.at<float>(rows,cols)=value;
              pch = strtok (NULL, "\t");
              ++cols;
            }

            free(pch);
            // Use the map
            std::map<float, std::string>::iterator it = actions.begin();
               while(it != actions.end())
               {
                   if (it->second == action)
                   {
                      MatLabels_TMP_SVM.at<float>(rows,0)=it->first;
                   }
                   it++;
               }
            ++rows;
            cols=0;
        }

        fclose(fp);
        if (line)
            free(line);

        //featuresUnclustered.push_back(MatDescriptor_TMP_SVM.reshape(1,1));   // One line per action.
        featuresUnclustered.push_back(MatDescriptor_TMP_SVM);
        LabelsUnclustered.push_back(MatLabels_TMP_SVM);

        //Just in case featuresUnclustered not defined as <float>
            Mat descriptor;
            Mat labels;

            featuresUnclustered.convertTo(descriptor, CV_32FC1);
            LabelsUnclustered.convertTo(labels, CV_32FC1);
        /*///////////////////////////////////////////////////////////*/
            FileStorage fs(fullName.substr(0,fullName.find_first_of(".")) + ".yml", FileStorage::WRITE);
            //fs << action.c_str() << featuresUnclustered;
            //fs << action.c_str() << descriptor;
            fs << "Action" << descriptor;
            fs.release();
    }
}
////////////////////////////////////////////////

//////////////// CrearDiccionariodeDirectorio ///////////////////
void BoW::CrearDiccionarioDirectorio(std::string dirName)
{
    Mat featuresUnclustered,LabelsUnclustered;

    std::string file;
    string action;

    if(dirName.find_last_of("/") != std::string::npos)
            action = dirName.substr(dirName.find_last_of("/")+1);

    DIR *pDIR;
    struct dirent *entry;
    if(pDIR=opendir(dirName.c_str()))
    {
        while(entry=readdir((pDIR)))
        {
            if(strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,".."))
            {
                string Name=entry->d_name;
                if(Name.substr(Name.find_first_of(".")+1) == "txt")
                {
                    file=dirName+"/"+Name;
                    const char *fileName = file.c_str();

                    FILE * fp;
                    char * line = NULL;
                    size_t len = 0;
                    ssize_t read;

                    fp = fopen(fileName, "r");
                    if (fp == NULL)
                       exit(EXIT_FAILURE);

                /*/////////////////////////////////////////////////////*/
                    int ch, number_of_lines = 0, number_of_colums = 0;
                    read = getline(&line, &len, fp);
                    char * pch;
                    pch = strtok (line,"\t");
                    while (pch != NULL)
                    {
                      number_of_colums++;
                      pch = strtok (NULL, "\t");
                    }
                    free(pch);
                    rewind(fp); //rewind the file to count the lines
                    do
                    {
                        ch = fgetc(fp);
                        if(ch == '\n')
                            number_of_lines++;
                    } while (ch != EOF);

                /*////////////////////////////////////////////////////*/
                    Mat MatLabels_TMP_SVM(number_of_lines,1,CV_32FC1);
                    Mat MatDescriptor_TMP_SVM(number_of_lines,number_of_colums,CV_32FC1);

                    std::map<float, std::string> actions;

                    // Initialize the map
                    actions.insert(std::make_pair(1, "boxing"));
                    actions.insert(std::make_pair(2, "handclapping"));
                    actions.insert(std::make_pair(3, "handwaving"));
                    actions.insert(std::make_pair(4, "jogging"));
                    actions.insert(std::make_pair(5, "running"));
                    actions.insert(std::make_pair(6, "walking"));
                    actions.insert(std::make_pair(7, "test_sequences"));

                    int rows=0;
                    int cols=0;
                    rewind(fp);

                    while ((read = getline(&line, &len, fp)) != -1)
                    {
                        //Extract parts separated by "\t"
                        char * pch;
                        pch = strtok (line,"\t");
                        while (pch != NULL)
                        {
                          //printf ("%s\n",pch);
                          float value=atof(pch);
                          MatDescriptor_TMP_SVM.at<float>(rows,cols)=value;
                          pch = strtok (NULL, "\t");
                          ++cols;
                        }

                        free(pch);
                        // Use the map
                        std::map<float, std::string>::iterator it = actions.begin();
                           while(it != actions.end())
                           {
                               if (it->second == action)
                               {
                                  MatLabels_TMP_SVM.at<float>(rows,0)=it->first;
                               }
                               it++;
                           }
                        ++rows;
                        cols=0;
                    }

                    fclose(fp);
                    if (line)
                        free(line);

                    //featuresUnclustered.push_back(MatDescriptor_TMP_SVM.reshape(1,1));   // One line per action.
                    featuresUnclustered.push_back(MatDescriptor_TMP_SVM);
                    LabelsUnclustered.push_back(MatLabels_TMP_SVM);
                }
            }
        }
    }

//Just in case featuresUnclustered not defined as <float>
    Mat descriptor;
    Mat labels;

    featuresUnclustered.convertTo(descriptor, CV_32FC1);
    LabelsUnclustered.convertTo(labels, CV_32FC1);
/*///////////////////////////////////////////////////////////*/
    FileStorage fs(dirName+"/"+action+".yml", FileStorage::WRITE);
    //fs << action.c_str() << featuresUnclustered;
    fs << action.c_str() << descriptor;
    fs.release();

/*  En caso de querer hacer el cluster aquí
    //Construct BOWKMeansTrainer
    //the number of bags
    int dictionarySize=10;
    //define Term Criteria
    TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);
    //retries number
    int retries=1;
    //necessary flags
    int flags=KMEANS_PP_CENTERS;
    //Create the BoW (or BoF) trainer
    BOWKMeansTrainer bowTrainer(dictionarySize,tc,retries,flags);
    //BOWKMeansTrainer bowTrainer(dictionarySize);
    bowTrainer.add(featuresUnclustered);
    //cluster the feature vectors
    Mat dictionary=bowTrainer.cluster(featuresUnclustered);
    Mat dictionary_labels=bowTrainer.cluster(LabelsUnclustered);
    //Mat dictionary=bowTrainer.cluster();
    //store the vocabulary
    //FileStorage fs("diccionario.yml", FileStorage::WRITE);
    FileStorage fs(dirName+"/"+action+".yml", FileStorage::WRITE);
    //fs << action.c_str() << featuresUnclustered;
    fs << action.c_str() << dictionary;
    fs.release();
    FileStorage fs2(dirName+"/"+action+"_Labels.yml", FileStorage::WRITE);
    //fs2 << action.c_str() << LabelsUnclustered;
    fs2 << action.c_str() << dictionary_labels;
    fs2.release();
*/
}
////////////////////////////////////////////////

//////////////// CrearDiccionariodeDirectorio ///////////////////
void BoW::CrearDiccionarioAcciones(std::string dirName)
{
    std::string file;
    string action;
    Mat TrainingData,TrainingLabels;

    std::map<float, std::string> actions;

    // Initialize the map
    actions.insert(std::make_pair(1, "boxing"));
    actions.insert(std::make_pair(2, "handclapping"));
    actions.insert(std::make_pair(3, "handwaving"));
    actions.insert(std::make_pair(4, "jogging"));
    actions.insert(std::make_pair(5, "running"));
    actions.insert(std::make_pair(6, "walking"));
    actions.insert(std::make_pair(7, "test_sequences"));

    DIR *pDIR;
    struct dirent *entry;
    if(pDIR=opendir(dirName.c_str()))
    {
        while(entry=readdir((pDIR)))
        {
            if(strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,".."))
            {
                string Name=entry->d_name;
                if(Name.substr(Name.find_first_of(".")+1) == "yml")
                {
                    file=dirName+"/"+Name;
                    action=Name.substr(0,Name.find_first_of("."));
                    Mat vocabulary, vocabulary_f;
                    FileStorage fs(file.c_str(), FileStorage::READ);
                        fs[action.c_str()] >> vocabulary;
                        vocabulary.convertTo(vocabulary_f, CV_32FC1);
                        fs.release();

                    Mat labels(vocabulary.rows,1,CV_32FC1);
                    int i=0;
                    while (i<=vocabulary.rows)
                    {
                        std::map<float, std::string>::iterator it = actions.begin();
                           while(it != actions.end())
                           {
                               if (it->second == action)
                               {
                                  labels.at<float>(i,0)=it->first;
                               }
                               it++;
                           }
                        i++;
                    }
                    TrainingData.push_back(vocabulary_f);
                    TrainingLabels.push_back(labels);
                }
            }
        }
    }

    Mat DataLabeled;
    hconcat(TrainingData,TrainingLabels,DataLabeled);
    //concat data with labels to avoid unmach them

    //Construct BOWKMeansTrainer
    //the number of bags
    int dictionarySize=10;
    //define Term Criteria
    TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);
    //retries number
    int retries=1;
    //necessary flags
    int flags=KMEANS_PP_CENTERS;
    //Create the BoW (or BoF) trainer
    BOWKMeansTrainer bowTrainer(dictionarySize,tc,retries,flags);
    //BOWKMeansTrainer bowTrainer(dictionarySize);
//    bowTrainer.add(TrainingData);
    //cluster the feature vectors
//    Mat dictionary=bowTrainer.cluster(TrainingData);
//    Mat dictionary_labels=bowTrainer.cluster(TrainingLabels);
    Mat LabeledDictionary=bowTrainer.cluster(DataLabeled);
    //Mat dictionary=bowTrainer.cluster();
    //store the vocabulary
    //FileStorage fs("diccionario.yml", FileStorage::WRITE);
//    FileStorage fs(dirName+"/"+"Actions_Clustered.yml", FileStorage::WRITE);
    FileStorage fs(dirName+"/"+"LabeledActions_Clustered.yml", FileStorage::WRITE);
    //fs << action.c_str() << featuresUnclustered;
    //fs << "Actions" << dictionary;
    fs << "LabeledActions" << LabeledDictionary;
    fs.release();
/*
    FileStorage fs2(dirName+"/"+"Labels_Clustered.yml", FileStorage::WRITE);
    //fs2 << action.c_str() << LabelsUnclustered;
    //fs2 << "Labels" << dictionary_labels;
    fs2 << "Labels" << TrainingLabels;
    fs2.release();

    Mat result;
    hconcat(TrainingData,TrainingLabels,result);
    FileStorage fs3(dirName+"/"+"Concat.yml", FileStorage::WRITE);
    //fs2 << action.c_str() << LabelsUnclustered;
    //fs2 << "Labels" << dictionary_labels;
    fs3 << "Concat" << result;
    fs3.release();
    Mat Ndata(result.rows,result.cols-1,CV_32FC1),Nlabe(result.rows,1,CV_32FC1);
    Nlabe=result.colRange(result.cols-1,result.cols);
    Ndata=result.colRange(0,result.cols-1);

    FileStorage fs4(dirName+"/"+"NewData.yml", FileStorage::WRITE);
    //fs << action.c_str() << featuresUnclustered;
    //fs << "Actions" << dictionary;
    fs4 << "Actions" << Ndata;
    fs4.release();
    FileStorage fs5(dirName+"/"+"NewLabels.yml", FileStorage::WRITE);
    //fs2 << action.c_str() << LabelsUnclustered;
    //fs2 << "Labels" << dictionary_labels;
    fs5 << "Labels" << Nlabe;
    fs5.release();
*/
}
////////////////////////////////////////////////

//////////////// CrearparaClasificadorDirectorio ///////////////////
void BoW::BoW_Clasificador(std::string dirName, std::string VocabularyName)
{
    Mat featuresUnclustered,LabelsUnclustered;

    std::string file;
    string action;

    if(dirName.find_last_of("/") != std::string::npos)
            action = dirName.substr(dirName.find_last_of("/")+1);

    DIR *pDIR;
    struct dirent *entry;
    if(pDIR=opendir(dirName.c_str()))
    {
        while(entry=readdir((pDIR)))
        {
            if(strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,".."))
            {
                string Name=entry->d_name;
                if(Name.substr(Name.find_first_of(".")+1) == "txt")
                {
                    file=dirName+"/"+Name;
                    const char *fileName = file.c_str();

                    FILE * fp;
                    char * line = NULL;
                    size_t len = 0;
                    ssize_t read;

                    fp = fopen(fileName, "r");
                    if (fp == NULL)
                       exit(EXIT_FAILURE);

                /*/////////////////////////////////////////////////////*/
                    int ch, number_of_lines = 0, number_of_colums = 0;
                    read = getline(&line, &len, fp);
                    char * pch;
                    pch = strtok (line,"\t");
                    while (pch != NULL)
                    {
                      number_of_colums++;
                      pch = strtok (NULL, "\t");
                    }
                    free(pch);
                    rewind(fp); //rewind the file to count the lines
                    do
                    {
                        ch = fgetc(fp);
                        if(ch == '\n')
                            number_of_lines++;
                    } while (ch != EOF);

                /*////////////////////////////////////////////////////*/
                    Mat MatLabels_TMP_SVM(number_of_lines,1,CV_32FC1);
                    Mat MatDescriptor_TMP_SVM(number_of_lines,number_of_colums,CV_32FC1);

                    std::map<float, std::string> actions;

                    // Initialize the map
                    actions.insert(std::make_pair(1, "boxing"));
                    actions.insert(std::make_pair(2, "handclapping"));
                    actions.insert(std::make_pair(3, "handwaving"));
                    actions.insert(std::make_pair(4, "jogging"));
                    actions.insert(std::make_pair(5, "running"));
                    actions.insert(std::make_pair(6, "walking"));
                    actions.insert(std::make_pair(7, "test_sequences"));

                    int rows=0;
                    int cols=0;
                    rewind(fp);

                    while ((read = getline(&line, &len, fp)) != -1)
                    {
                        //Extract parts separated by "\t"
                        char * pch;
                        pch = strtok (line,"\t");
                        while (pch != NULL)
                        {
                          //printf ("%s\n",pch);
                          float value=atof(pch);
                          MatDescriptor_TMP_SVM.at<float>(rows,cols)=value;
                          pch = strtok (NULL, "\t");
                          ++cols;
                        }

                        free(pch);
                        // Use the map
                        std::map<float, std::string>::iterator it = actions.begin();
                           while(it != actions.end())
                           {
                               if (it->second == action)
                               {
                                  MatLabels_TMP_SVM.at<float>(rows,0)=it->first;
                               }
                               it++;
                           }
                        ++rows;
                        cols=0;
                    }

                    fclose(fp);
                    if (line)
                        free(line);

                    //featuresUnclustered.push_back(MatDescriptor_TMP_SVM.reshape(1,1));   // One line per action.
                    featuresUnclustered.push_back(MatDescriptor_TMP_SVM);
                    LabelsUnclustered.push_back(MatLabels_TMP_SVM);
                }
            }
        }
    }

//Just in case featuresUnclustered not defined as <float>
    Mat descriptor;
    Mat labels;

    featuresUnclustered.convertTo(descriptor, CV_32FC1);
    LabelsUnclustered.convertTo(labels, CV_32FC1);
/*///////////////////////////////////////////////////////////*/

    Mat vocabulary, vocabulary_f, EuclDist;

    FileStorage fs(VocabularyName.c_str(), FileStorage::READ);
        fs["LabeledActions"] >> vocabulary;
        vocabulary.convertTo(vocabulary_f, CV_32FC1);
        fs.release();
        Mat Cdata=vocabulary_f.colRange(0,vocabulary_f.cols-1);

    float min=0, index=0;
    EuclDist = Mat::zeros(1,vocabulary_f.size,CV_32FC1);
    Mat HistEuclDist = Mat::zeros(featuresUnclustered.rows,EuclDist.rows,CV_32FC1);
    //Busco la Distancia Euclídea entre cada fila de featuresUnclustered y cada palabra del vocabulario
    for (int i=0; i<=featuresUnclustered.rows; ++i)
    {
        for (int j=0; j<=vocabulary_f.rows-1; ++j)
        {
            float dist=norm(featuresUnclustered.row(i),Cdata.row(j),NORM_L2);
            EuclDist.at<float>(j-1,1)=dist;
            cout << endl << "j=" << j << "dist=" << dist << endl;
        }
        cout << endl << endl << "EuclDist= " << EuclDist << endl;
        double minVal,maxVal;
        int minIndx,maxIndx;
        minMaxLoc(SparseMat(EuclDist),&minVal,&maxVal,&minIndx,&maxIndx);
        //index=0;
        ++HistEuclDist.at<float>(minIndx,1);
    }
}
////////////////////////////////////////////////
