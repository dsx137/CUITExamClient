#pragma once
#include "include.h"

template<typename T>
class QChain {
private:
    T* p;
public:
    QChain(T* p): p(p) {}
    ~QChain() {};
    T* get() { return p; };

    QChain* setLayout(QLayout*) { p->setLayout(p);return this; };
    QChain* setText(const QString& text) { p->setText(text);return this; };
    QChain* addWidget(QWidget* widget) { p->addWidget(widget);return this; };
    QChain* addLayout(QLayout* layout) { p->addLayout(layout);return this; };
    QChain* setFixedSize(int w, int h) { p->setFixedSize(w, h);return this; };
    QChain* setFixedHeight(int h) { p->setFixedHeight(h);return this; };
    QChain* setFixedWidth(int w) { p->setFixedWidth(w);return this; };
    QChain* setStyleSheet(const QString& styleSheet) { p->setStyleSheet(styleSheet);return this; };
    QChain* setReadOnly(bool ro) { p->setReadOnly(ro);return this; };
    QChain* setFont(const QFont& font) { p->setFont(font);return this; };

    QChain* operator->() { return this; };

};