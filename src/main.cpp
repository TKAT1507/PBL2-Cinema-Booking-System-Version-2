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

    // Soft Light & Gentle Rose Palette
    QPalette appPalette;
    appPalette.setColor(QPalette::Window, QColor("#F5F6F8"));
    appPalette.setColor(QPalette::WindowText, QColor("#1E293B"));
    appPalette.setColor(QPalette::Base, QColor("#FFFFFF"));
    appPalette.setColor(QPalette::AlternateBase, QColor("#FAFBFD"));
    appPalette.setColor(QPalette::ToolTipBase, QColor("#1E293B"));
    appPalette.setColor(QPalette::ToolTipText, QColor("#FFFFFF"));
    appPalette.setColor(QPalette::Text, QColor("#1E293B"));
    appPalette.setColor(QPalette::PlaceholderText, QColor("#94A3B8"));
    appPalette.setColor(QPalette::Button, QColor("#FFFFFF"));
    appPalette.setColor(QPalette::ButtonText, QColor("#1E293B"));
    appPalette.setColor(QPalette::Highlight, QColor("#FEF2F2"));
    appPalette.setColor(QPalette::HighlightedText, QColor("#1E293B"));
    appPalette.setColor(QPalette::Disabled, QPalette::Text, QColor("#94A3B8"));
    appPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#94A3B8"));
    app.setPalette(appPalette);

    try {
        MainWindow mainWindow;
        mainWindow.show();
        return app.exec();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return -1;
    }
}
