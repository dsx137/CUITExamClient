#pragma once
#include "../cpphead.h"

class MainWindow :public QMainWindow {
public:
    //UI
    class UI;

    //member variable
    UI* ui;
    Manager* manager;

    //declare
    MainWindow(QWidget* p = nullptr);
    ~MainWindow();
    void initConnection();

    //按F8触发screenGrab
    void keyPressEvent(QKeyEvent* event) override;
};

class MainWindow::UI :public QWidget {
private:
    QMainWindow* w;
public:
    QWidget* mainwidget;
    QWebEngineView* webView;
    QLineEdit* urlBar;
    QPushButton* confirmURLButton;
    UI(QMainWindow* w);
    void initLayout();
};