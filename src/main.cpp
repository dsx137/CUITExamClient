#include "head/cpphead.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow* w = new MainWindow;
    Updater* updater = new Updater;
    updater->checkUpdate();
    w->showMaximized();
    return a.exec();
}