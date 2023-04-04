#include "head/cpphead.h"

void FUNC::screenGrab() {
    QScreen* screen = QGuiApplication::primaryScreen();
    if (!screen) {
        qWarning() << "No screen found";
        return;
    }

    // Grab the current screen image
    QPixmap pixmap = screen->grabWindow(0);

    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QNetworkRequest request(QUrl("http://localhost:8000/Img"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "image/jpeg");
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "JPG", 10);
    buffer.close();
    manager->post(request, data);
}

void FUNC::setTokenListener(QWebEngineView* view) {
    QObject::connect(view, &QWebEngineView::loadFinished, [=](bool ok) {
        if (ok && view->url() == QUrl("http://162.14.117.85/index")) {
            view->page()->runJavaScript("localStorage.getItem('CUITAccessToken')", [=](const QVariant& result) {
                QString token = result.toString();
                qDebug() << "Token: " << token;
                });
        }
        });
}