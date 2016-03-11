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
    Mat featuresUnclustered;
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
          pch = strtok (NULL, "\t\n");
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
        Mat MatDescriptor_TMP_SVM(number_of_lines,number_of_colums,CV_32FC1);

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
              pch = strtok (NULL, "\t\n");
              ++cols;
            }

            free(pch);

            ++rows;
            cols=0;
        }

        fclose(fp);
        if (line)
            free(line);

        //featuresUnclustered.push_back(MatDescriptor_TMP_SVM.reshape(1,1));   // One line per action.
        featuresUnclustered.push_back(MatDescriptor_TMP_SVM);

        //Just in case featuresUnclustered not defined as <float>
            Mat descriptor;

            featuresUnclustered.convertTo(descriptor, CV_32FC1);

        /*///////////////////////////////////////////////////////////*/
            FileStorage fs(fullName.substr(0,fullName.find_first_of(".")) + ".yml", FileStorage::WRITE);
            fs << "Action" << descriptor;
            fs.release();
    }
}
////////////////////////////////////////////////

//////////////// CrearDiccionariodeDirectorio ///////////////////
void BoW::CrearDiccionarioDirectorio(std::string dirName)
{
    Mat featuresUnclustered;

    std::string file;
    string action;
    int MxFeaturesPerFile=2000,MxFeaturesPerAction=25*MxFeaturesPerFile;

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
                if(Name.substr(Name.find_first_of(".")+1) == "txt" && featuresUnclustered.rows+1<MxFeaturesPerAction)
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
                        pch = strtok (NULL, "\t\n");
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
                    Mat MatDescriptor_TMP_SVM(MxFeaturesPerFile,number_of_colums,CV_32FC1);

                    if (number_of_lines<MxFeaturesPerFile)
                    {
                        MatDescriptor_TMP_SVM.rows=number_of_lines;
                    }

                    int rows=0;
                    int cols=0;
                    rewind(fp);

                    while ((read = getline(&line, &len, fp)) != -1 && rows+1<=MatDescriptor_TMP_SVM.rows)
                    {
                        //Extract parts separated by "\t"
                        char * pch;
                        pch = strtok (line,"\t");
                        while (pch != NULL)
                        {
                          float value=atof(pch);
                          MatDescriptor_TMP_SVM.at<float>(rows,cols)=value;
                          pch = strtok (NULL, "\t\n");
                          ++cols;
                        }

                        free(pch);

                        ++rows;
                        cols=0;
                    }

                    fclose(fp);
                    if (line)
                        free(line);

                    featuresUnclustered.push_back(MatDescriptor_TMP_SVM);
                }
            }
        }
    }

//Just in case featuresUnclustered not defined as <float>
    Mat descriptor;

    featuresUnclustered.convertTo(descriptor, CV_32FC1);

/*///////////////////////////////////////////////////////////*/
    FileStorage fs(dirName+"/"+action+".yml", FileStorage::WRITE);
    fs << action.c_str() << descriptor;
    fs.release();
}
////////////////////////////////////////////////

//////////////// CrearDiccionariodeDirectorio ///////////////////
void BoW::CrearDiccionarioAcciones(std::string dirName)
{
    std::string file;
    string action;
    Mat TrainingData;

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
                    cout << endl << "Añadiendo " << action << endl;
                    Mat vocabulary, vocabulary_f;
                    FileStorage fs(file.c_str(), FileStorage::READ);
                        fs[action.c_str()] >> vocabulary;
                        vocabulary.convertTo(vocabulary_f, CV_32FC1);
                        fs.release();

                    TrainingData.push_back(vocabulary_f);
                    cout << endl << action << " añadida" << endl;
                }
            }
        }
    }

    //Construct BOWKMeansTrainer
    //the number of bags
    int dictionarySize=4000;
    //define Term Criteria
    TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);
    //retries number
    int retries=1;
    //necessary flags
    int flags=KMEANS_PP_CENTERS;
    //Create the BoW (or BoF) trainer
    cout << endl << "Generando Diccionario" << endl;
    BOWKMeansTrainer bowTrainer(dictionarySize,tc,retries,flags);
    cout << endl << "Clusterizando Diccionario" << endl;
    clock_t begin=clock();
    Mat Dictionary=bowTrainer.cluster(TrainingData);
    clock_t end=clock();    
    cout << "Time elapsed: " << double((end-begin)/ CLOCKS_PER_SEC) << " ms"<< endl;
    
    cout << endl << "Guardando Diccionario" << endl;
    begin=clock();
    FileStorage fs(dirName+"/"+"Actions_Clustered_4000.yml", FileStorage::WRITE);
    end=clock();    
    cout << "Time elapsed: " << double((end-begin)/ CLOCKS_PER_SEC) << " s"<< endl;
    fs << "Actions" << Dictionary;
    fs.release();
}
////////////////////////////////////////////////

//////////////// CrearparaClasificadorDirectorio ///////////////////
void BoW::BoW_DOF_DE(std::string dirName, std::string VocabularyName, cv::Mat& BoWFeatures, cv::Mat& BoWLabels)
{
    Mat vocabulary, EuclDist, Cdata;

    FileStorage fs(VocabularyName.c_str(), FileStorage::READ);
        fs["Actions"] >> vocabulary;
          vocabulary.convertTo(Cdata, CV_32FC1);
        fs.release();

    std::string file;
    string action;

    clock_t dir_begin=clock();
    DIR *pDIR;
    struct dirent *entry;
    if(pDIR=opendir(dirName.c_str()))
    {
        while(entry=readdir((pDIR)))
        {
            if(entry->d_type & DT_DIR && strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,".."))
            {
                DIR *pDIR2;
                struct dirent *entry2;
                string directorio=dirName+"/"+entry->d_name;
                if(pDIR2=opendir(directorio.c_str()))
                {
                    while(entry2=readdir((pDIR2)))
                    {
                        if(strcmp(entry2->d_name,".") != 0 && strcmp(entry2->d_name,".."))
                        {
                            if(directorio.find_last_of("/") != std::string::npos)
                                    action = directorio.substr(directorio.find_last_of("/")+1);
                            cout << "Procesing action " << action << endl;
                            clock_t file_begin=clock();
                            string Name=entry2->d_name;
                            if(Name.substr(Name.find_first_of(".")+1) == "txt")
                            {
                                file=directorio+"/"+Name;
                                const char *fileName = file.c_str();
                                cout << "Procesing file " << file << endl;
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
                                  pch = strtok (NULL, "\t\n");
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
                                //Mat MatLabels_TMP_SVM(number_of_lines,1,CV_32FC1);
                                Mat MatDescriptor_TMP_SVM(number_of_lines,number_of_colums,CV_32FC1);

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
                                      float value=atof(pch);
                                      MatDescriptor_TMP_SVM.at<float>(rows,cols)=value;
                                      pch = strtok (NULL, "\t\n");
                                      ++cols;
                                    }

                                    free(pch);

                                    ++rows;
                                    cols=0;
                                }

                                fclose(fp);
                                if (line)
                                    free(line);

            ///////////////////////////////////////////////////////////

                                    EuclDist = Mat::zeros(Cdata.rows,1,CV_32FC1);
                                    Mat HistEuclDist = Mat::zeros(1,EuclDist.rows,CV_32FC1);
                                    //Busco la Distancia Euclídea entre cada fila de descriptor y cada palabra del vocabulario
                                    for (int i=0; i<MatDescriptor_TMP_SVM.rows; ++i)
                                    {
                                        for (int j=0; j<Cdata.rows; ++j)
                                        {
                                            float dist=norm(MatDescriptor_TMP_SVM.row(i),Cdata.row(j),NORM_L2);
                                            EuclDist.at<float>(0,j)=dist;
                                        }
                                        double minVal,maxVal;
                                        int minIndx,maxIndx;
                                        minMaxLoc(SparseMat(EuclDist),&minVal,&maxVal,&minIndx,&maxIndx);
                                        ++HistEuclDist.at<float>(0,minIndx);
                                    }
                                    float NormEuclDist=sum(HistEuclDist)[0];
                                    Mat NormHistEuclDist=HistEuclDist.clone();
                                    for (int k=0; k<=HistEuclDist.cols;++k)
                                    {
                                        NormHistEuclDist.at<float>(0,k)=NormHistEuclDist.at<float>(0,k)/NormEuclDist;
                                    }

            ///////////////////////////////////////////////////////////

                                BoWFeatures.push_back(NormHistEuclDist);

                                std::map<float, std::string> actions;

                                // Initialize the map
                                actions.insert(std::make_pair(1, "boxing"));
                                actions.insert(std::make_pair(2, "handclapping"));
                                actions.insert(std::make_pair(3, "handwaving"));
                                actions.insert(std::make_pair(4, "jogging"));
                                actions.insert(std::make_pair(5, "running"));
                                actions.insert(std::make_pair(6, "walking"));
                                actions.insert(std::make_pair(7, "test_sequences"));

                                // Use the map
                                std::map<float, std::string>::iterator it = actions.begin();
                                   while(it != actions.end())
                                   {
                                       if (it->second == action)
                                       {
                                          BoWLabels.push_back(it->first);
                                       }
                                       it++;
                                   }
                            }
                            clock_t file_end=clock();
                            cout << "Time elapsed by file procesing: " << double((file_end-file_begin)/ CLOCKS_PER_SEC) << " s"<< endl;
                        }
                    }
                }
                closedir(pDIR2);
            }
        }
    }
    closedir(pDIR);
    clock_t dir_end=clock();
    cout << "Time elapsed by directory procesing: " << double((dir_end-dir_begin)/ CLOCKS_PER_SEC) << " s"<< endl;
}
////////////////////////////////////////////////
