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

    // bt = new tButton(w);
    // bt->resize(100, 100);
    // QObject::connect(bt, &QPushButton::clicked, bt, &tButton::testSlotFunc);
    view = new QWebEngineView(w);
    view->load(QUrl("https://dsx137.github.io/DNote"));

}

void FWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event); // 调用基类的方法
    // ui->view->setGeometry(width() / 2, height() / 2, width() / 2, height() / 2); // 设置view的位置和大小
    ui->view->setFixedSize(width(), height());
}