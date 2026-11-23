#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include "ui/common/MainWindow.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Use Fusion style for consistent, crisp modern rendering across macOS/Windows/Linux
    if (QStyleFactory::keys().contains("Fusion")) {
        app.setStyle(QStyleFactory::create("Fusion"));
    }

    // Soft Midnight & Electric Coral Palette
    QPalette midnightPalette;
    midnightPalette.setColor(QPalette::Window, QColor("#161827"));
    midnightPalette.setColor(QPalette::WindowText, QColor("#F3F4FA"));
    midnightPalette.setColor(QPalette::Base, QColor("#1E2236"));
    midnightPalette.setColor(QPalette::AlternateBase, QColor("#23273E"));
    midnightPalette.setColor(QPalette::ToolTipBase, QColor("#2B304D"));
    midnightPalette.setColor(QPalette::ToolTipText, QColor("#F3F4FA"));
    midnightPalette.setColor(QPalette::Text, QColor("#F3F4FA"));
    midnightPalette.setColor(QPalette::PlaceholderText, QColor("#747D9E"));
    midnightPalette.setColor(QPalette::Button, QColor("#282C45"));
    midnightPalette.setColor(QPalette::ButtonText, QColor("#F3F4FA"));
    midnightPalette.setColor(QPalette::Highlight, QColor("#FF6B6B"));
    midnightPalette.setColor(QPalette::HighlightedText, QColor("#FFFFFF"));
    midnightPalette.setColor(QPalette::Disabled, QPalette::Text, QColor("#606785"));
    midnightPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#606785"));
    app.setPalette(midnightPalette);

    try {
        MainWindow mainWindow;
        mainWindow.show();
        return app.exec();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return -1;
    }
}
