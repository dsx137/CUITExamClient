#pragma once
#include <QtWidgets/QWidget>
#include <QMainWindow>
#include <QPushButton.h>
#include <QWebEngineView>
#include "abutton.h"


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
private:
    UI* ui;
};

