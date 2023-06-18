#pragma once

#include "../cpphead.h"

class Updater : public QObject {
public:
    Updater(QObject* p = nullptr);
    QNetworkReply* getRequest(const QString& url);
    void checkUpdate();
};
