#pragma once
#include <QtWidgets/QWidget>
#include <QMainWindow>
#include <QPushButton.h>
#include "abutton.h"


class FWindow:public QMainWindow {
public:
    class UI {
    public:
        QWidget* mainwidget;
        tButton* bt;
        UI(QMainWindow* w);
    };
    FWindow(QWidget* p = nullptr);
    ~FWindow();
    UI* getUI();
private:
    UI* ui;
};

