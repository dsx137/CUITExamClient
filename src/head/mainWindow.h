#pragma once
#include "include.h"

class FWindow:public QMainWindow {
public:
    class UI:public QWidget {
    private:
        QMainWindow* w;
    public:
        QWidget* mainwidget;
        QWebEngineView* webView;
        QLineEdit* urlBar;
        QPushButton* confirmURLButton;
        UI(QMainWindow* w);
        void initLayout();
        void initConnection();
    };
    FWindow(QWidget* p = nullptr);
    ~FWindow();
    UI* getUI();
    void resizeEvent(QResizeEvent* event);
private:
    UI* ui;
};

