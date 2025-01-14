#include <QApplication>
#include "qapplication.h"
#include <gtest/gtest.h>

auto main(int argc, char **argv) -> int {
    QApplication const app(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
