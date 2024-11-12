#include "hello.hpp"
#include <QLabel>
#include <memory>
#include <qlabel.h>

MyWidget::MyWidget() {
    constexpr int LabelXPos = 100;
    constexpr int LabelYPos = 100;

    auto label = std::make_unique<QLabel>(this);

    label->move(LabelXPos, LabelYPos);
    setWindowTitle("Hello World");
};

MyWidget::~MyWidget() = default;
