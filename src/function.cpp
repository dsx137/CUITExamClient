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

bool FUNC::detectNetworkConnection()
{
    QStringList targetIPs = {
        "162.14.117.85",
        "127.0.0.1"
    };
    NetstatRunnable* runnable = new NetstatRunnable(targetIPs); // 创建一个NetstatRunnable对象，传入接口名称和目标IP地址
    QThreadPool::globalInstance()->start(runnable); // 使用QThreadPool类的全局实例来启动NetstatRunnable对象

    return true;
}

FUNC::NetstatRunnable::NetstatRunnable(const QStringList& targetIPs) // 构造函数，初始化成员变量
    : targetIPs(targetIPs) {}

void FUNC::NetstatRunnable::run() {
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

QString FUNC::NetstatRunnable::getProgramName(const QString& pid) {
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