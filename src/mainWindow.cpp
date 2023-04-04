#include "head/cpphead.h"
//FWindow
FWindow::FWindow(QWidget* p)
    :QMainWindow(p)
    , ui(new UI(this)) {

    setCentralWidget(ui);

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
FWindow::UI::UI(QMainWindow* w)
    :QWidget(w)
    , w(w) {
    setLayout(initWindow());
}

QLayout* FWindow::UI::initWindow() {

    view = new QWebEngineView;
    view->load(QUrl("http://162.14.117.85/login"));

    FUNC::setTokenListener(view);

    lineEdit = new QLineEdit;
    lineEdit->setFixedHeight(50);

    pushbt = new QPushButton;
    pushbt->setFixedSize(50, 50);
    pushbt->setText("confirm");

    QObject::connect(pushbt, &QPushButton::clicked, [&] {
        QString str = lineEdit->text();
        view->load(QUrl(str));
        });

    QHBoxLayout* hLayout = new QHBoxLayout;
    QVBoxLayout* vLayout = new QVBoxLayout;
    hLayout->addWidget(lineEdit);
    hLayout->addWidget(pushbt);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(view);
    return vLayout;
}

void FWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    // ui->view->setGeometry(width() / 2, height() / 2, width() / 2, height() / 2); // 设置view的位置和大小
    // ui->view->setFixedSize(width(), height() - 50);
    // ui->view->move(0, 50);
    // ui->lineEdit->setFixedSize(width() - 50, 50);
    // ui->pushbt->move(width() - 50, 0);
}
