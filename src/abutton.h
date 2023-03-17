#pragma once
#include "include.h"

class tButton:public QPushButton {
public:
    tButton(QWidget* p = nullptr)
        :QPushButton(p) {
    };


    void testSlotFunc() {
        // setText("fuck");
        // .screenGrab();
    }
};
