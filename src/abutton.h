#include <QPushButton.h>

// using namespace FW;
class tButton:public QPushButton {
public:
    tButton(QWidget* p = nullptr)
        :QPushButton(p) {
    };


    void testSlotFunc() {
        setText("fuck");
    }
};
