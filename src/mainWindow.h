#pragma once
#include "include.h"

class FWindow:public QMainWindow {
public:
    class UI {
    public:
        QWidget* mainwidget;
        QWebEngineView* view;
        tButton* bt;
        UI(QMainWindow* w);
    };
    FWindow(QWidget* p = nullptr);
    ~FWindow();
    UI* getUI();
    void resizeEvent(QResizeEvent* event);
    void screenGrab();
private:
    UI* ui;
};

