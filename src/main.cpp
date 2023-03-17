#include "include.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    FWindow w;
    w.show();
    w.screenGrab();
    return a.exec();
}