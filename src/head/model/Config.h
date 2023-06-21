#pragma once
#include "../cpphead.h"

#define CONFIG Config::instance      

class Config {
public:
    static Config* instance;

    QString version = "0";
    QString versionCheckURL = "https://cuit.site.asyncraft.club/source/test/version.txt";
    QString downloadURLDir = "https://cuit.site.asyncraft.club/source/test/";

    QString SERVER_IP = "162.14.117.85";
    QString IMG_URL = "http://162.14.117.85:8888/frontExam/uploadExamPicture";
    int imgQuality = 20;
    int postInterval = 60;
    int requestWaitTime = 10000;
};

