#include <QApplication>
#include <QFont>
#include <QGridLayout>
#include <QLCDNumber>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>
#include <memory>

constexpr int LCD_DIGITS = 2;
constexpr int SLIDER_MIN = 0;
constexpr int SLIDER_MAX = 99;
constexpr int FONT_SIZE = 18;

class LCDRange : public QWidget {
   public:
    explicit LCDRange(QWidget *parent = nullptr);
};

LCDRange::LCDRange(QWidget *parent) : QWidget(parent) {
    auto lcd = std::make_unique<QLCDNumber>(LCD_DIGITS);
    lcd->setSegmentStyle(QLCDNumber::Filled);

    auto slider = std::make_unique<QSlider>(Qt::Horizontal);
    slider->setRange(SLIDER_MIN, SLIDER_MAX);
    slider->setValue(SLIDER_MIN);

    connect(slider.get(), &QSlider::valueChanged, lcd.get(),
            static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));

    auto layout = std::make_unique<QVBoxLayout>();
    layout->addWidget(lcd.release());
    layout->addWidget(slider.release());
    setLayout(layout.release());
}

class MyWidget : public QWidget {
   public:
    explicit MyWidget(QWidget *parent = nullptr);
};

MyWidget::MyWidget(QWidget *parent) : QWidget(parent) {
    auto quit = std::make_unique<QPushButton>(tr("Quit"));
    quit->setFont(QFont("Times", FONT_SIZE, QFont::Bold));
    connect(quit.get(), &QPushButton::clicked, qApp, &QApplication::quit);

    auto grid = std::make_unique<QGridLayout>();
    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 3; ++column) {
            auto lcdRange = std::make_unique<LCDRange>();
            grid->addWidget(lcdRange.release(), row, column);
        }
    }

    auto layout = std::make_unique<QVBoxLayout>();
    layout->addWidget(quit.release());
    layout->addLayout(grid.release());
    setLayout(layout.release());
}

auto main(int argc, char *argv[]) -> int {
    const QApplication app(argc, argv);

    MyWidget widget;
    widget.show();

    return QApplication::exec();
}
