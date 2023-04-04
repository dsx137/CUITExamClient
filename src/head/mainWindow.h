#pragma once
#include "include.h"

class FWindow:public QMainWindow {
public:
    class UI:public QWidget {
    private:
        QMainWindow* w;
    public:
        QWidget* mainwidget;
        QWebEngineView* view;
        QLineEdit* lineEdit;
        QPushButton* pushbt;
        UI(QMainWindow* w);
        QLayout* initWindow();
    };
    FWindow(QWidget* p = nullptr);
    ~FWindow();
    UI* getUI();
    void resizeEvent(QResizeEvent* event);
private:
    UI* ui;
};

