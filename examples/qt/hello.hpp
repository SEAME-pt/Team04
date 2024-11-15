#include <qwidget.h>

#include <QWidget>

class MyWidget : public QWidget {
   public:
    MyWidget();
    ~MyWidget() override;
    MyWidget(const MyWidget&) = delete;
    auto operator=(const MyWidget&) -> MyWidget& = delete;
    MyWidget(MyWidget&&) = delete;
    auto operator=(MyWidget&&) -> MyWidget& = delete;
};
