#pragma once
#include "include.h"

class FUNC {
public:
    static void screenGrab();
    static bool detectNetworkConnection();
    class NetstatRunnable: public QRunnable // 定义一个工作类，继承自QRunnable
    {
    public:
        NetstatRunnable(const QStringList& targetIP);
        void run() override; // 重写run()函数
    private:
        QStringList targetIPs; // 定义一个目标IP地址的字符串
        QString getProgramName(const QString& pid);
    };
};