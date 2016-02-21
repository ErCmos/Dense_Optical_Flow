#ifndef BOW_H
#define BOW_H

#include "stdio.h"
//#include "stdafx.h"
//#include <opencv2/cv.h>
//#include <QString>
//#include <QDir>
//#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include "opencv2/xfeatures2d.hpp"
#include "dirent.h"
#include "classifier.h"
#include "QVector"

/**
 * @brief The BoW class Clase para definir un Bag of Words
 * @author ErCmos
 */
class BoW
{
public:
    /**
     * @brief BoW Constructor y destructor del BoW
     */
    BoW();
    ~BoW();
    //////////////// CrearDiccionario ///////////////////
    void CrearDiccionario(std::string file, std::string dir);
    void CrearDiccionarioDirectorio(std::string dir);
    void CrearDiccionarioAcciones(std::string dir);
    //void CrearDiccionario(std::string descriptor);
    ////////////////////////////////////////////////

    //////////////// TageadorDiccionario ///////////////////
    void TageadorDiccionario(std::string dirName, std::string fileName);
    ////////////////////////////////////////////////
};

#endif // BOW_H
