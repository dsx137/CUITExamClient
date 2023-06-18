#pragma once

template<typename T>
class QChain {
private:
    T* p;
public:
    QChain(T* p) : p(p) {}
    ~QChain() {};
    T* get() { return p; };

    template<typename T1>
    QChain* addChild(T1* child) { child->setParent(p);return this; };
    template<typename T1>
    QChain* setParent(T1* parent) { p->setParent(parent);return this; };
    template<typename T1>
    QChain* setLayout(T1* layout) { p->setLayout(layout);return this; };
    template<typename T1>
    QChain* setText(const T1& text) { p->setText(text);return this; };
    template<typename T1>
    QChain* addWidget(T1* widget) { p->addWidget(widget);return this; };
    template<typename T1>
    QChain* addLayout(T1* layout) { p->addLayout(layout);return this; };
    template<typename T1>
    QChain* setStyleSheet(const T1& styleSheet) { p->setStyleSheet(styleSheet);return this; };
    template<typename T1>
    QChain* setFont(const T1& font) { p->setFont(font);return this; };
    template<typename T1>
    QChain* setAlignment(const T1& alignment) { p->setAlignment(alignment);return this; };
    template<typename T1>
    QChain* setValidator(const T1& validator) { p->setValidator(validator);return this; };

    QChain* setFixedSize(int w, int h) { p->setFixedSize(w, h);return this; };
    QChain* setFixedHeight(int h) { p->setFixedHeight(h);return this; };
    QChain* setFixedWidth(int w) { p->setFixedWidth(w);return this; };
    QChain* setReadOnly(bool ro) { p->setReadOnly(ro);return this; };
    QChain* setSpacing(int spacing) { p->setSpacing(spacing);return this; };
    QChain* setContentMargins(int left, int top, int right, int bottom) { p->setContentMargins(left, top, right, bottom);return this; };

    QChain* operator->() { return this; };
};