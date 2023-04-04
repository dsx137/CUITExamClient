#include "head/cpphead.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    FWindow* w = new FWindow;
    w->showMaximized();
    return a.exec();
}