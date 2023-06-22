#pragma once
#include "../cpphead.h"
class MainWindow;
class Manager : public QObject {
public:
    MainWindow* w;
    QTimer* timer;
    QNetworkAccessManager* networkAccessManager;;

    QString token;
    QString planId;

    Manager(QObject* p = nullptr);

    void initConnection();
    void getToken();
    void getPlanId();

    RETURNCODE screenGrab();
    bool detectNetworkConnection();
    class NetstatRunnable : public QRunnable // 定义一个工作类，继承自QRunnable
    {
    public:
        NetstatRunnable(const QStringList& targetIP);
        void run() override; // 重写run()函数
    private:
        QStringList targetIPs; // 定义一个目标IP地址的字符串
        QString getProgramName(const QString& pid);
    };
};