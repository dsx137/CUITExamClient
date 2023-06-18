#include "head/cpphead.h"

Updater::Updater(QObject* p)
    :QObject(p) {

}

QNetworkReply* Updater::getRequest(const QString& url) {
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QNetworkReply* reply = manager->get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QTimer::singleShot(CONFIG->requestWaitTime, &loop, SLOT(quit()));
    loop.exec();

    if (!reply->isFinished()) {
        QMessageBox::critical(nullptr, "错误", "连接服务器超时，请检查网络连接");
        exit(0);
    }

    return reply;
}

void Updater::checkUpdate() {
    QString currentPath = QCoreApplication::applicationDirPath();

    QNetworkReply* reply = getRequest(CONFIG->versionCheckURL);

    QByteArray data = reply->readAll();
    QString newVersion(data);
    if (newVersion.toInt() > CONFIG->version.toInt()) {
        QMessageBox* updateMsgBox = new QMessageBox(QMessageBox::Information, "更新", "检测到新版本，即将下载安装包……");

        QTimer::singleShot(3000, updateMsgBox, SLOT(accept()));

        updateMsgBox->exec();

        QString timeDate = QString::number(QDateTime::currentMSecsSinceEpoch() / 1000);

        QNetworkReply* reply = getRequest(CONFIG->downloadURLDir + newVersion + ".exe");

        QByteArray data = reply->readAll();

        QFile installer(QDir::tempPath() + "/" + timeDate + "_installer.exe");
        installer.open(QIODevice::WriteOnly);
        installer.write(data);
        installer.close();

        //install
        QFile batFile(QDir::tempPath() + "/" + timeDate + "_installer.bat");
        batFile.open(QIODevice::WriteOnly);
        batFile.write("@echo off\n");
        batFile.write("%~d0\n");
        batFile.write("cd %temp%\n");
        batFile.write("timeout /t 5 /nobreak\n");
        batFile.write(("rd /s /q \"" + currentPath + "\"\n").toUtf8());
        batFile.write("timeout /t 5 /nobreak\n");
        batFile.write(("\"" + installer.fileName() + "\"\n").toUtf8());
        batFile.write(("echo hello >> " + timeDate + ".txt").toUtf8());
        batFile.close();

        std::string pt = batFile.fileName().toStdString();

        QProcess::startDetached(batFile.fileName());
        exit(0);
    }
    else {
        qDebug() << "no new version";
    }
}