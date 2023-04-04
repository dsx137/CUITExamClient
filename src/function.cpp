#include "head/cpphead.h"

void FUNC::screenGrab() {
    QScreen* screen = QGuiApplication::primaryScreen();
    if (!screen) {
        qWarning() << "No screen found";
        return;
    }

    // Grab the current screen image
    QPixmap pixmap = screen->grabWindow(0);

    //send the image to server
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QNetworkRequest request(QUrl("http://localhost:8000/Img"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "image/jpeg");
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "JPG", 20);
    buffer.close();
    manager->post(request, data);
}

