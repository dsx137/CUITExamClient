#include "head/cpphead.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    FWindow w;
    w.show();
    return a.exec();
}