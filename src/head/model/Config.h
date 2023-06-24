#pragma once
#include "../cpphead.h"    

class Config {
public:
    inline static bool needUpdate = false;
    inline static QString version = "0";
    inline static QString versionCheckURL = "";
    inline static QString downloadURLDir = "";
    inline static QString tokenKey = "CUITQKLGC2031123";

    inline static QString SERVER_IP = "162.14.117.85";
    inline static QString IMG_URL = "http://162.14.117.85:8888/frontExam/uploadExamPicture";
    // inline static QString IMG_URL = "http://localhost:8000/Img";
    inline static int imgQuality = 30;
    inline static int imgWidth = 1920;
    inline static int imgHeight = 1080;
    inline static int postInterval = 5 * 60 * 1000;
    inline static int requestWaitTime = 10 * 1000;
};
