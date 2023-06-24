#include "head/cpphead.h"

Manager::Manager(QObject* p)
    :QObject(p),
    w((MainWindow*)p),
    timer(new QTimer(this)),
    networkAccessManager(new QNetworkAccessManager(this)) {
    initConnection();
    timer->start(Config::postInterval);
}


void Manager::initConnection() {
    QObject::connect(timer, &QTimer::timeout, [=] {screenGrab();});
    QObject::connect(networkAccessManager, &QNetworkAccessManager::finished, [=](QNetworkReply* reply) {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "error";
        }
        qDebug() << "hello" << QString::fromUtf8(reply->readAll());
        });
    // QObject::connect(timer, &QTimer::timeout, &Manager::detectNetworkConnection);
}

void Manager::getToken() {
    w->ui->webView->page()->runJavaScript("localStorage.getItem('CUITAccessToken')", [&](const QVariant& result) {
        if (!result.isValid()) return;
        this->token = Crypt::tokenDecrypt(result.toString());
        qDebug() << "getToken";
        });
}

void Manager::getPlanId() {
    QUrl url = w->ui->webView->url();
    QRegularExpression regex("(\\d+)$");
    QRegularExpressionMatch match = regex.match(url.toString());
    if (match.hasMatch()) {
        QString planId = match.captured(1);
        this->planIds.insert(planId);
        qDebug() << "getPlanId: " << planId;
    }
    // w->ui->webView->page()->runJavaScript("Array.from(document.getElementsByClassName(\"problem - item\")).map(i => ({startTime: i.getElementsByClassName(\"total - time\")[0].innerText,endTime: i.getElementsByClassName(\"end - time\")[0].innerText,state: Array.from(i.getElementsByClassName(\"s - btn\")).map(j=>j.innerText),planId: i.getElementsByClassName(\"title\")[0].href.split('/').pop()}))", [&](const QVariant& result) {
    //     if (!result.isValid()) return;
    //     QJsonDocument jsonDoc = QJsonDocument::fromVariant(result);
    //     QJsonArray jsonArray = jsonDoc.array();
    //     for (auto i : jsonArray) {
    //         QJsonObject jsonObject = i.toObject();
    //         if (jsonObject["state"].toArray().contains("已开放") && jsonObject["state"].toArray().contains("未提交")) {
    //             QString planId = jsonObject["planId"].toString();
    //             this->planIds.append(planId);
    //             qDebug() << "getPlanId: " << planId;
    //             return;
    //         }
    //     }
    //     qDebug() << "getPlanId";
    //     });
}

RETURNCODE Manager::screenGrab() {
    if (token.isEmpty() || planIds.isEmpty()) {
        return RETURNCODE::FAILURE;
    }
    QScreen* screen = QGuiApplication::primaryScreen();
    if (!screen) {
        qWarning() << "No screen found";
        return RETURNCODE::FAILURE;
    }

    QString timeDate = QString::number(QDateTime::currentMSecsSinceEpoch());
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QImageWriter writer(&buffer, "JPEG");
    writer.setQuality(Config::imgQuality);
    writer.setCompression(0);
    writer.write(screen->grabWindow(0).toImage().scaled(Config::imgWidth, Config::imgHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    buffer.close();

    for (auto planId : planIds) {
        QUrlQuery query;
        query.addQueryItem("planId", planId);
        QString boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
        QNetworkRequest request = QChain(QNetworkRequest())
            ->setUrl(QUrl(Config::IMG_URL + "?" + query.toString()))
            ->setRawHeader("Content-Type", ("multipart/form-data; boundary=" + boundary).toUtf8())
            ->setRawHeader("x-token", token.toUtf8())
            ->setRawHeader("Connection", "keep-alive")
            ->setRawHeader("Accept", "*/*")
            ->data();

        QHttpPart filePart = QChain(QHttpPart())
            ->setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"" + timeDate + ".png\""))
            ->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"))
            ->setBody(buffer.data())
            ->data();

        QHttpMultiPart* multiPart = QChain(new QHttpMultiPart(QHttpMultiPart::FormDataType))
            ->setBoundary(boundary.toUtf8())
            ->append(filePart)
            ->get();

        networkAccessManager->post(request, multiPart);
    }

    return RETURNCODE::SUCCESS;
}



//Deprecated

bool Manager::detectNetworkConnection()
{
    QStringList targetIPs = {
        Config::SERVER_IP,
        "127.0.0.1"
    };
    NetstatRunnable* runnable = new NetstatRunnable(targetIPs); // 创建一个NetstatRunnable对象，传入接口名称和目标IP地址
    QThreadPool::globalInstance()->start(runnable); // 使用QThreadPool类的全局实例来启动NetstatRunnable对象

    return true;
}

Manager::NetstatRunnable::NetstatRunnable(const QStringList& targetIPs) // 构造函数，初始化成员变量
    : targetIPs(targetIPs) {}

void Manager::NetstatRunnable::run() {
    QProcess process; // 创建一个QProcess对象
    process.start("netstat", QStringList() << "-a" << "-n" << "-o" << "-p" << "tcp"); // 执行netstat命令，传入参数
    process.waitForFinished(); // 等待命令执行完成
    QString output = process.readAllStandardOutput(); // 读取命令的标准输出
    QStringList lines = output.split("\n"); // 按换行符分割输出
    for (const QString& line : lines) { // 遍历每行输出
        if (line.contains("ESTABLISHED")) { // 如果包含ESTABLISHED字样，表示有活跃的连接
            QStringList columns = line.simplified().split(" "); // 按空格分割每行输出
            QString remoteAddress = columns.at(2); // 获取远程地址列
            bool foundTargetIP = false;
            for (const QString& targetIP : targetIPs) {
                if (remoteAddress.startsWith(targetIP)) { // 如果远程地址以目标IP开头
                    foundTargetIP = true;
                    break;
                }
            }
            if (!foundTargetIP) { // 如果远程地址不以目标IP开头
                // qDebug() << remoteAddress << "\n";
                // qDebug() << "hello\n"; // 输出hello
                // QProcess::execute(QString("taskkill /PID %1 /F").arg(columns.at(4))); // 结束对应进程
                qDebug() << getProgramName(columns.at(4)) << "\n";
            }
        }
    }
}

QString Manager::NetstatRunnable::getProgramName(const QString& pid) {
    QProcess process;
    process.start("tasklist", QStringList() << "/fi" << QString("PID eq %1").arg(pid));
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    QStringList lines = output.split("\n");
    if (lines.length() >= 3) {
        QStringList columns = lines.at(3).simplified().split(" ");
        QString processName = columns.at(0);
        std::string s = processName.toStdString();
        return processName;
    }
    else {
        return "Unknown";
    }
}