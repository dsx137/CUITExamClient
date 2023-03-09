#include "mainWindow.h"


//FWindow
FWindow::FWindow(QWidget* p)
    :QMainWindow(p)
    , ui(new UI(this)) {
}
FWindow::~FWindow() {
    delete ui;
}
FWindow::UI* FWindow::getUI() {
    return ui;
}


//UI
FWindow::UI::UI(QMainWindow* w) {
    w->resize(800, 600);

    bt = new tButton(w);
    bt->resize(100, 100);
    QObject::connect(bt, &QPushButton::clicked, bt, &tButton::testSlotFunc);
}