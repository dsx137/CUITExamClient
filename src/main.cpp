#include "mainWindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    FWindow w;
    w.show();
    return a.exec();
}