#include "head/cpphead.h"

/*-------------MainWindow--------------*/

MainWindow::MainWindow(QWidget* p)
    :QMainWindow(p)
    , ui(new UI(this)),
    manager(new Manager(this)) {
    initConnection();
    setCentralWidget(ui);
}
MainWindow::~MainWindow() {
    delete manager;
    delete ui;
}

void MainWindow::initConnection() {

    QObject::connect(ui->webView, &QWebEngineView::urlChanged, [=](const QUrl& url) {
        ui->urlBar->setText(url.toString());
        manager->getPlanId();
        if (url == QUrl("http://162.14.117.85/index"))
            manager->getToken();
        });


    //load url when confirmURLButton clicked
    QObject::connect(ui->confirmURLButton, &QPushButton::clicked, [&] {
        ui->webView->load(QUrl(ui->urlBar->text()));
        });

}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_F8) {
        manager->screenGrab();
    }
    // else if (event->key() == Qt::Key_F9)
    // {
    //     Config::IMG_URL = Config::IMG_URL == "http://localhost:8000/Img" ? "http://162.14.117.85:8888/frontExam/uploadExamPicture" : "http://localhost:8000/Img";
    // }

}


//---------------------------UI------------------------------


MainWindow::UI::UI(QMainWindow* w)
    :QWidget(w)
    , w(w) {
    initLayout();
}

void MainWindow::UI::initLayout() {

    webView = new QWebEngineView;
    webView->load(QUrl("http://162.14.117.85/login"));

    urlBar = QChain(new QLineEdit)
        ->setFixedHeight(50)
        ->setStyleSheet("border: 1px solid gray;")
        ->setReadOnly(true)
        ->setFont(QFont("Microsoft YaHei", 20))
        ->get();

    confirmURLButton = QChain(new QPushButton)
        ->setFixedSize(100, 50)
        ->setText("Refresh")
        ->get();

    setLayout(
        QChain(new QVBoxLayout)
        ->addLayout(
            QChain(new QHBoxLayout)
            ->addWidget(urlBar)
            ->addWidget(confirmURLButton)
            ->get()
        )
        ->addWidget(webView)
        ->get()
    );
}