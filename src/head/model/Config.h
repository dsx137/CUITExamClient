#pragma once
#include "../cpphead.h"    

class Config {
public:

    inline static QString version = "0";
    inline static QString versionCheckURL = "https://cuit.site.asyncraft.club/source/test/version.txt";
    inline static QString downloadURLDir = "https://cuit.site.asyncraft.club/source/test/";
    inline static QString tokenKey = "CUITQKLGC2031123";


    inline static QString SERVER_IP = "162.14.117.85";
    inline static QString IMG_URL = "http://162.14.117.85:8888/frontExam/uploadExamPicture";
    // inline static QString IMG_URL = "http://localhost:8000/Img";
    inline static int imgQuality = 20;
    inline static int postInterval = 5 * 60 * 1000;
    inline static int requestWaitTime = 10000;
};

