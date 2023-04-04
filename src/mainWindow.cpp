#include "head/cpphead.h"
//FWindow
FWindow::FWindow(QWidget* p)
    :QMainWindow(p)
    , ui(new UI(this)) {

    setCentralWidget(ui);

    QTimer* timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, &FUNC::screenGrab);
    timer->start(2000);
}
FWindow::~FWindow() {
    delete ui;
}
FWindow::UI* FWindow::getUI() {
    return ui;
}

//UI
FWindow::UI::UI(QMainWindow* w)
    :QWidget(w)
    , w(w) {
    setLayout(init());
}

QLayout* FWindow::UI::init() {

    view = new QWebEngineView;
    view->load(QUrl("http://162.14.117.85/login"));

    FUNC::setTokenListener(view);

    lineEdit = QChain(new QLineEdit)
        ->setFixedHeight(50)
        ->setStyleSheet("border: 1px solid gray;")
        ->get();

    QObject::connect(view, &QWebEngineView::loadStarted, [=] {
        lineEdit->setText(view->url().toString());
        });

    pushbtn = QChain(new QPushButton)
        ->setFixedSize(50, 50)
        ->setText("confirm")
        ->get();

    QObject::connect(pushbtn, &QPushButton::clicked, [&] {
        QString str = lineEdit->text();
        view->load(QUrl(str));
        });

    QHBoxLayout* hLayout = QChain(new QHBoxLayout)
        ->addWidget(lineEdit)
        ->addWidget(pushbtn)
        ->get();
    QVBoxLayout* vLayout = QChain(new QVBoxLayout)
        ->addLayout(hLayout)
        ->addWidget(view)
        ->get();

    return vLayout;
}

void FWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    // ui->view->setGeometry(width() / 2, height() / 2, width() / 2, height() / 2); // 设置view的位置和大小
    // ui->view->setFixedSize(width(), height() - 50);
    // ui->view->move(0, 50);
    // ui->lineEdit->setFixedSize(width() - 50, 50);
    // ui->pushbtn->move(width() - 50, 0);
}
