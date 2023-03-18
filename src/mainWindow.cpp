#include "head/cpphead.h"

//FWindow
FWindow::FWindow(QWidget* p)
    :QMainWindow(p)
    , ui(new UI(this)) {
    QTimer* timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, [&] {FUNC::screenGrab();});
    timer->start(2000);
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

    view = new QWebEngineView(w);
    view->load(QUrl("http://exam.cuit.edu.cn/"));

    lineEdit = new QLineEdit(w);
    lineEdit->resize(100, 100);

    pushbt = new QPushButton(w);
    pushbt->setFixedSize(50, 50);
    pushbt->setText("confirm");

    QObject::connect(pushbt, &QPushButton::clicked, [&] {
        QString str = lineEdit->text();
        view->load(QUrl(str));
        });
}

void FWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    // ui->view->setGeometry(width() / 2, height() / 2, width() / 2, height() / 2); // 设置view的位置和大小
    ui->view->setFixedSize(width(), height());
    ui->lineEdit->setFixedSize(width() - 50, 50);
    ui->pushbt->move(width() - 50, 0);
}

