#include "hello.hpp"

#include <qlabel.h>

#include <QLabel>
#include <memory>

MyWidget::MyWidget() {
    constexpr int LabelXPos = 100;
    constexpr int LabelYPos = 100;

    auto label = std::make_unique<QLabel>(this);

    label->move(LabelXPos, LabelYPos);
    setWindowTitle("Hello World");
};

MyWidget::~MyWidget() = default;
