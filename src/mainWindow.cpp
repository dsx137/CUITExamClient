#include "head/cpphead.h"

//FWindow
FWindow::FWindow(QWidget* p)
    :QMainWindow(p)
    , ui(new UI(this)) {

    setCentralWidget(ui);

    QTimer* timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, &FUNC::screenGrab);
    QObject::connect(timer, &QTimer::timeout, &FUNC::detectNetworkConnection);
    timer->start(2000);
}
FWindow::~FWindow() {
    delete ui;
}
FWindow::UI* FWindow::getUI() {
    return ui;
}

//------------------------------------------------------------

//UI
FWindow::UI::UI(QMainWindow* w)
    :QWidget(w)
    , w(w) {
    initLayout();
    initConnection();
}

void FWindow::UI::initLayout() {

    webView = new QWebEngineView;
    webView->load(QUrl("http://162.14.117.85/login"));

    urlBar = QChain(new QLineEdit)
        ->setFixedHeight(50)
        ->setStyleSheet("border: 1px solid gray;")
        ->setReadOnly(true)
        ->setFont(QFont("Microsoft YaHei", 20))
        ->get();

    confirmURLButton = QChain(new QPushButton)
        ->setFixedSize(50, 50)
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

void FWindow::UI::initConnection() {

    //set urlBar text
    QObject::connect(webView, &QWebEngineView::urlChanged, [=](const QUrl& url) {
        urlBar->setText(url.toString());
        });

    //get token when loadFinished
    QObject::connect(webView, &QWebEngineView::loadFinished, [=](bool ok) {
        if (ok && webView->url() == QUrl("http://162.14.117.85/index")) {
            webView->page()->runJavaScript("localStorage.getItem('CUITAccessToken')", [=](const QVariant& result) {
                QString token = result.toString();
                qDebug() << "Token: " << token;
                });
        }
        });

    //load url when confirmURLButton clicked
    QObject::connect(confirmURLButton, &QPushButton::clicked, [&] {
        webView->load(QUrl(urlBar->text()));
        });
}

//------------------------------------------------------------


//event

void FWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
}
