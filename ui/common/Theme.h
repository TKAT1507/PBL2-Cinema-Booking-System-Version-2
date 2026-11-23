#ifndef THEME_H
#define THEME_H

#include <QString>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QFileInfo>
#include <QCoreApplication>

namespace Theme {

// ============================================================================
// SOFT MIDNIGHT & ELECTRIC CORAL PALETTE — DYNAMIC, SOFT & CINEMATIC
// ============================================================================
inline constexpr const char* BG_ROOT         = "#161827"; // Soft midnight canvas
inline constexpr const char* BG_SIDEBAR      = "#1C1F33"; // Soft twilight sidebar
inline constexpr const char* BG_SURFACE      = "#22263C"; // Floating soft cards
inline constexpr const char* BG_SURFACE_ALT  = "#1D2034"; // Gentle alternating row
inline constexpr const char* BG_ELEVATED     = "#2B304B"; // Secondary pill surfaces

inline constexpr const char* CORAL_PRIMARY   = "#FF6B6B"; // Electric soft coral
inline constexpr const char* CORAL_WARM      = "#FF8E72"; // Warm peach-coral
inline constexpr const char* INDIGO_PASTEL   = "#8C92FF"; // Dreamy periwinkle accent
inline constexpr const char* BUTTER_GOLD     = "#FFD166"; // Warm buttercream gold
inline constexpr const char* MINT_FRESH      = "#34D399"; // Fresh emerald mint

inline constexpr const char* TEXT_PRIMARY    = "#F3F4FA"; // Crisp soft white
inline constexpr const char* TEXT_SECONDARY  = "#A3ABC9"; // Soft cool periwinkle-gray
inline constexpr const char* TEXT_MUTED      = "#707899"; // Subtle muted slate

inline constexpr const char* BORDER_SUBTLE   = "#313654"; // Soft twilight hairline
inline constexpr const char* BORDER_STRONG   = "#4B5380"; // Interactive border

// ============================================================================
// GLOBAL APPLICATION STYLESHEET
// ============================================================================
inline QString globalStylesheet() {
    return R"(
        QMainWindow {
            background-color: #161827;
        }
        QWidget {
            font-family: "SF Pro Display", "Helvetica Neue", "Segoe UI", Arial, sans-serif;
            font-size: 13px;
            color: #F3F4FA;
        }

        /* Default Buttons — Electric Coral Pill */
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FF6B6B, stop:1 #FF8E72);
            color: #FFFFFF;
            border: none;
            border-radius: 12px;
            padding: 8px 20px;
            font-weight: bold;
            min-height: 34px;
            outline: none;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FF545A, stop:1 #FF7B5C);
        }
        QPushButton:pressed {
            background-color: #E34B54;
        }
        QPushButton:disabled {
            background: #252940;
            color: #656D8F;
            border: 1px solid #303654;
        }

        /* Tables — Soft Midnight Cards */
        QTableWidget {
            background-color: #1F2338;
            alternate-background-color: #242942;
            gridline-color: #2D3352;
            border: 1px solid #323859;
            border-radius: 16px;
            selection-background-color: #3D2B42;
            selection-color: #FFFFFF;
            color: #F3F4FA;
            outline: 0;
        }
        QTableWidget::item {
            padding: 8px 12px;
            border-bottom: 1px solid #2A2F4A;
        }
        QTableWidget::item:selected {
            background-color: #462D42;
            color: #FFFFFF;
        }
        QTableWidget::item:hover:!selected {
            background-color: #2A2F4C;
        }
        QHeaderView::section {
            background-color: #262B47;
            color: #B8C0E6;
            padding: 11px 12px;
            border: none;
            border-bottom: 2px solid #FF6B6B;
            border-right: 1px solid #31375A;
            font-weight: bold;
            font-size: 11px;
            text-transform: uppercase;
        }
        QHeaderView::section:last {
            border-right: none;
        }
        QTableCornerButton::section {
            background-color: #262B47;
            border: none;
            border-bottom: 2px solid #FF6B6B;
        }

        /* Inputs — Soft Rounded Wells */
        QLineEdit, QSpinBox, QDoubleSpinBox, QComboBox, QDateEdit, QTimeEdit {
            background-color: #1A1D2F;
            color: #F3F4FA;
            border: 1.5px solid #33395A;
            border-radius: 11px;
            padding: 6px 14px;
            min-height: 26px;
            selection-background-color: #FF6B6B;
            selection-color: #FFFFFF;
        }
        QLineEdit:hover, QSpinBox:hover, QDoubleSpinBox:hover, QComboBox:hover, QDateEdit:hover, QTimeEdit:hover {
            border-color: #4F5785;
            background-color: #20243A;
        }
        QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QComboBox:focus, QDateEdit:focus, QTimeEdit:focus {
            border: 1.5px solid #FF6B6B;
            background-color: #222740;
        }
        QLineEdit:read-only {
            background-color: #171A2A;
            color: #848DB0;
            border-color: #2B304D;
        }

        QComboBox QAbstractItemView {
            background-color: #22263C;
            color: #F3F4FA;
            selection-background-color: #FF6B6B;
            selection-color: #FFFFFF;
            border: 1px solid #3D446B;
            border-radius: 10px;
            outline: 0;
            padding: 4px;
        }
        QComboBox QAbstractItemView::item {
            min-height: 30px;
            padding: 4px 12px;
            border-radius: 7px;
        }

        /* Calendar Popup */
        QCalendarWidget QWidget {
            background-color: #1F2338;
            color: #F3F4FA;
        }
        QCalendarWidget QWidget#qt_calendar_navigationbar {
            background-color: #292E4D;
            border-bottom: 1px solid #FF6B6B;
            padding: 6px;
        }
        QCalendarWidget QToolButton {
            background-color: transparent;
            color: #F3F4FA;
            font-weight: bold;
            padding: 4px 10px;
            border-radius: 8px;
        }
        QCalendarWidget QToolButton:hover {
            background-color: #FF6B6B;
        }
        QCalendarWidget QMenu {
            background-color: #22263C;
            color: #F3F4FA;
            border: 1px solid #3D446B;
        }
        QCalendarWidget QAbstractItemView:enabled {
            background-color: #1F2338;
            color: #F3F4FA;
            selection-background-color: #FF6B6B;
            selection-color: #FFFFFF;
        }
        QCalendarWidget QAbstractItemView:disabled {
            color: #555D80;
        }

        /* Labels & GroupBox */
        QLabel {
            color: #F3F4FA;
            background: transparent;
        }
        QGroupBox {
            background-color: #22263C;
            border: 1px solid #323859;
            border-radius: 16px;
            margin-top: 16px;
            padding: 18px 16px 16px 16px;
            color: #FFD166;
            font-weight: bold;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 16px;
            padding: 0 8px;
            color: #FFD166;
        }

        /* Dialogs & Message Boxes */
        QDialog {
            background-color: #1D2136;
            color: #F3F4FA;
        }
        QMessageBox {
            background-color: #1F2338;
            color: #F3F4FA;
        }
        QMessageBox QLabel {
            color: #F3F4FA;
            font-size: 13px;
        }
        QMessageBox QPushButton {
            min-width: 90px;
            min-height: 34px;
            padding: 6px 18px;
            border-radius: 10px;
        }

        /* TextEdit & ScrollAreas */
        QTextEdit {
            background-color: #1A1D2F;
            color: #F3F4FA;
            border: 1px solid #323859;
            border-radius: 12px;
            padding: 12px;
        }
        QScrollArea {
            background: transparent;
            border: none;
        }

        /* Scrollbars — Soft Minimal Pill */
        QScrollBar:vertical {
            background-color: transparent;
            width: 8px;
            margin: 2px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background-color: #394066;
            border-radius: 4px;
            min-height: 32px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #FF6B6B;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        QScrollBar:horizontal {
            background-color: transparent;
            height: 8px;
            margin: 2px;
            border-radius: 4px;
        }
        QScrollBar::handle:horizontal {
            background-color: #394066;
            border-radius: 4px;
            min-width: 32px;
        }
        QScrollBar::handle:horizontal:hover {
            background-color: #FF6B6B;
        }
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px;
        }

        QToolTip {
            background-color: #2B304D;
            color: #F3F4FA;
            border: 1px solid #FF6B6B;
            padding: 6px 10px;
            border-radius: 8px;
        }
    )";
}

// ============================================================================
// BUTTON STYLES — SOFT PILLS & ELECTRIC CORAL ENERGY
// ============================================================================
inline QString primaryButtonStyle(int minHeight = 40, int fontSize = 13) {
    return QString(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FF6B6B, stop:1 #FF8E72);
            color: #FFFFFF;
            font-size: %1px;
            font-weight: bold;
            border: none;
            border-radius: 12px;
            min-height: %2px;
            padding: 6px 22px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FF545A, stop:1 #FF7B5C);
        }
        QPushButton:pressed {
            background-color: #E34B54;
        }
        QPushButton:disabled {
            background: #252940;
            color: #656D8F;
            border: 1px solid #303654;
        }
    )").arg(fontSize).arg(minHeight);
}

inline QString secondaryButtonStyle(int minHeight = 40, int fontSize = 13) {
    return QString(R"(
        QPushButton {
            background-color: #2A2F4A;
            color: #DCE0F5;
            font-size: %1px;
            font-weight: 600;
            border: 1px solid #3D446B;
            border-radius: 12px;
            min-height: %2px;
            padding: 6px 20px;
        }
        QPushButton:hover {
            background-color: #343A5C;
            border-color: #7C83FD;
            color: #FFFFFF;
        }
        QPushButton:pressed {
            background-color: #22263C;
        }
        QPushButton:disabled {
            background-color: #202438;
            color: #5C6485;
            border-color: #2C314D;
        }
    )").arg(fontSize).arg(minHeight);
}

inline QString successButtonStyle(int minHeight = 40, int fontSize = 13) {
    return QString(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #10B981, stop:1 #14B8A6);
            color: #FFFFFF;
            font-size: %1px;
            font-weight: bold;
            border: none;
            border-radius: 12px;
            min-height: %2px;
            padding: 6px 22px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #059669, stop:1 #0D9488);
        }
        QPushButton:pressed {
            background-color: #047857;
        }
        QPushButton:disabled {
            background: #252940;
            color: #656D8F;
            border: 1px solid #303654;
        }
    )").arg(fontSize).arg(minHeight);
}

inline QString goldOutlineButtonStyle(int minHeight = 38, int fontSize = 13) {
    return QString(R"(
        QPushButton {
            background-color: #282B4D;
            color: #A5ABFF;
            font-size: %1px;
            font-weight: bold;
            border: 1px solid #4D5399;
            border-radius: 12px;
            min-height: %2px;
            padding: 6px 18px;
        }
        QPushButton:hover {
            background-color: #333763;
            border-color: #8C92FF;
            color: #FFFFFF;
        }
        QPushButton:pressed {
            background-color: #202340;
        }
    )").arg(fontSize).arg(minHeight);
}

inline QString dangerOutlineButtonStyle(int minHeight = 38, int fontSize = 13) {
    return QString(R"(
        QPushButton {
            background-color: #332130;
            color: #FF8C94;
            font-size: %1px;
            font-weight: bold;
            border: 1px solid #6E3647;
            border-radius: 12px;
            min-height: %2px;
            padding: 6px 18px;
        }
        QPushButton:hover {
            background-color: #4A283A;
            border-color: #FF6B6B;
            color: #FFFFFF;
        }
        QPushButton:pressed {
            background-color: #2B1B29;
        }
    )").arg(fontSize).arg(minHeight);
}

// ============================================================================
// SIDEBAR STYLES (Soft Midnight Twilight + Electric Coral Pill)
// ============================================================================
inline QString sidebarContainerStyle() {
    return R"(
        QWidget#sidebarContainer {
            background-color: #1C1F33;
            border-right: 1px solid #2D3252;
        }
    )";
}

inline QString sidebarProfileCardStyle() {
    return R"(
        QFrame#profileCard {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #2E2747, stop:1 #382538);
            border: 1px solid #4E3C66;
            border-radius: 14px;
        }
    )";
}

inline QString sidebarNavButtonStyle(bool active) {
    if (active) {
        return R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FF6B6B, stop:1 #FF8E72);
                color: #FFFFFF;
                border: none;
                border-radius: 12px;
                font-size: 13px;
                font-weight: bold;
                min-height: 44px;
                padding: 6px 16px;
                text-align: left;
            }
        )";
    }
    return R"(
        QPushButton {
            background-color: transparent;
            color: #A3ABC9;
            border: none;
            border-radius: 12px;
            font-size: 13px;
            font-weight: 600;
            min-height: 44px;
            padding: 6px 16px;
            text-align: left;
        }
        QPushButton:hover {
            background-color: #272B47;
            color: #FFFFFF;
        }
        QPushButton:pressed {
            background-color: #303556;
        }
    )";
}

// ============================================================================
// CINEMA SCREEN & SEAT MAP STYLES (Soft Midnight Auditorium)
// ============================================================================
inline QString screenBannerStyle() {
    return R"(
        QLabel {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #7C83FD, stop:0.5 #FF6B6B, stop:1 #FFD166);
            color: #FFFFFF;
            border: none;
            border-radius: 16px;
            font-weight: bold;
            letter-spacing: 5px;
            font-size: 12px;
        }
    )";
}

inline QString seatLegendSwatchStyle(int type) {
    // 0: standard free, 1: standard booked, 2: vip free, 3: vip booked, 4: selected
    switch (type) {
        case 0: return "background-color: #2B304D; border-radius: 6px; border: 1.5px solid #4D5685;";
        case 1: return "background-color: #191C2C; border-radius: 6px; border: 1.5px solid #292E47;";
        case 2: return "background-color: #3B311E; border-radius: 6px; border: 1.5px solid #E6B84C;";
        case 3: return "background-color: #292128; border-radius: 6px; border: 1.5px solid #4A3442;";
        default: return "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #FF6B6B, stop:1 #FF8E72); border-radius: 6px; border: 1.5px solid #FFFFFF;";
    }
}

inline QString seatButtonStyle(int status, bool selected = false) {
    if (selected) {
        return R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #FF6B6B, stop:1 #FF8E72);
                color: #FFFFFF;
                border: 2px solid #FFFFFF;
                border-radius: 10px;
                font-weight: bold;
                font-size: 10px;
                padding: 0px;
            }
        )";
    }
    if (status == 0) { // Standard free — Soft Midnight Periwinkle
        return R"(
            QPushButton {
                background-color: #2B304D;
                color: #DCE2FF;
                border: 1.5px solid #4D5685;
                border-radius: 10px;
                font-weight: bold;
                font-size: 10px;
                padding: 0px;
            }
            QPushButton:hover {
                background-color: #373D63;
                border: 1.5px solid #FF6B6B;
                color: #FFFFFF;
            }
        )";
    }
    if (status == 1) { // Standard booked — Soft Dimmed Navy
        return R"(
            QPushButton {
                background-color: #191C2C;
                color: #4D5475;
                border: 1px solid #272B42;
                border-radius: 10px;
                font-weight: 600;
                font-size: 10px;
                padding: 0px;
            }
        )";
    }
    if (status == 2) { // VIP free — Warm Buttercream Gold
        return R"(
            QPushButton {
                background-color: #3B311E;
                color: #FFE08A;
                border: 1.5px solid #E6B84C;
                border-radius: 10px;
                font-weight: bold;
                font-size: 10px;
                padding: 0px;
            }
            QPushButton:hover {
                background-color: #4F4126;
                border: 1.5px solid #FFE8A3;
                color: #FFF8DC;
            }
        )";
    }
    // VIP booked (status == 3)
    return R"(
        QPushButton {
            background-color: #292128;
            color: #705866;
            border: 1px solid #453340;
            border-radius: 10px;
            font-weight: 600;
            font-size: 10px;
            padding: 0px;
        }
    )";
}

// ============================================================================
// AGE RATING BADGES (Soft Glowing Dark-Pastel Pills)
// ============================================================================
inline QString ageBadgeText(int age, bool verbose = true) {
    if (age == 0) return verbose ? "P • Mọi lứa tuổi" : "P";
    return verbose ? QString("C%1 • Từ %1 tuổi").arg(age) : QString("C%1").arg(age);
}

inline QString ageBadgeStyle(int age) {
    if (age == 0) {
        return "color: #34D399; font-size: 11px; font-weight: bold; background-color: #1B3635; border: 1px solid #2A6E5C; border-radius: 8px; padding: 3px 10px;";
    } else if (age <= 13) {
        return "color: #FFD166; font-size: 11px; font-weight: bold; background-color: #3A3122; border: 1px solid #8A6D2F; border-radius: 8px; padding: 3px 10px;";
    } else if (age <= 16) {
        return "color: #FFB07C; font-size: 11px; font-weight: bold; background-color: #3D2824; border: 1px solid #9E5238; border-radius: 8px; padding: 3px 10px;";
    }
    return "color: #FF8C94; font-size: 11px; font-weight: bold; background-color: #3D2130; border: 1px solid #A83A50; border-radius: 8px; padding: 3px 10px;";
}

// ============================================================================
// TYPOGRAPHY & CARD HELPERS
// ============================================================================
inline QString sectionTitleStyle() {
    return "color: #F3F4FA; font-size: 21px; font-weight: bold; letter-spacing: 0.3px; background: transparent; border: none;";
}

inline QString sectionEyebrowStyle() {
    return "color: #FF7675; font-size: 11px; font-weight: bold; letter-spacing: 2px; background: transparent; border: none;";
}

inline QString authCardStyle() {
    return R"(
        QFrame#authCard {
            background-color: #20243A;
            border: 1px solid #353C61;
            border-radius: 22px;
        }
    )";
}

inline QString luxuryCardStyle(const QString& objectName = "luxuryCard") {
    return QString(R"(
        QFrame#%1 {
            background-color: #22263C;
            border: 1px solid #343A5C;
            border-radius: 20px;
        }
    )").arg(objectName);
}

// ============================================================================
// POSTER IMAGE HELPERS
// ============================================================================
inline QPixmap loadPosterPixmap(const QString& posterPath) {
    QPixmap pixmap;
    if (posterPath.isEmpty()) return pixmap;
    if (QFileInfo::exists(posterPath)) {
        pixmap.load(posterPath);
    } else {
        QString alt1 = "posters/" + QFileInfo(posterPath).fileName();
        if (QFileInfo::exists(alt1)) {
            pixmap.load(alt1);
        } else {
            QString alt2 = QCoreApplication::applicationDirPath() + "/" + posterPath;
            if (QFileInfo::exists(alt2)) {
                pixmap.load(alt2);
            }
        }
    }
    return pixmap;
}

inline QPixmap roundedCoverPixmap(const QPixmap& src, int w, int h, int radius = 14) {
    if (src.isNull() || w <= 0 || h <= 0) return QPixmap();
    QPixmap scaled = src.scaled(w, h, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    int x = (scaled.width() - w) / 2;
    int y = (scaled.height() - h) / 2;
    QPixmap cropped = scaled.copy(x, y, w, h);

    QPixmap out(w, h);
    out.fill(Qt::transparent);
    QPainter painter(&out);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    QPainterPath path;
    path.addRoundedRect(0, 0, w, h, radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, cropped);
    return out;
}

} // namespace Theme

#endif // THEME_H
