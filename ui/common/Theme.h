#ifndef UI_COMMON_THEME_H
#define UI_COMMON_THEME_H

#include <QString>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>

namespace Theme {

// ============================================================================
// SOFT LIGHT & GENTLE ROSE PALETTE (Nền trắng dịu & Đỏ hồng trầm)
// ============================================================================
inline constexpr const char* BgMain          = "#F5F6F8"; // Soft off-white canvas
inline constexpr const char* BgSidebar       = "#FFFFFF"; // Crisp white sidebar
inline constexpr const char* BgCard          = "#FFFFFF"; // Pure white card/table surface
inline constexpr const char* BgCardHover     = "#F9FAFB"; // Subtle hover surface
inline constexpr const char* BgElevated      = "#F3F4F6"; // Secondary controls & headers
inline constexpr const char* BgInput         = "#F9FAFB"; // Gentle input field fill

inline constexpr const char* BorderSubtle    = "#E5E7EB"; // Clean 1px hairline border
inline constexpr const char* BorderGold      = "#F59E0B"; // Warm amber outline
inline constexpr const char* BorderFocus     = "#E05D5D"; // Soft terracotta rose focus

inline constexpr const char* TextPrimary     = "#1E293B"; // Deep slate (softer than pure black)
inline constexpr const char* TextSecondary   = "#64748B"; // Balanced neutral slate gray
inline constexpr const char* TextMuted       = "#94A3B8"; // Muted caption / placeholder

inline constexpr const char* AccentCrimson   = "#E05D5D"; // Soft rose-coral brand accent
inline constexpr const char* AccentCrimsonHi = "#CC4B4B"; // Hover state
inline constexpr const char* AccentCoral     = "#E05D5D";
inline constexpr const char* AccentGold      = "#D97706"; // Warm caramel amber for prices/VIP
inline constexpr const char* AccentGoldSoft  = "#FEF3C7";
inline constexpr const char* AccentViolet    = "#6366F1";

inline constexpr const char* Success         = "#059669"; // Soft sage emerald
inline constexpr const char* Warning         = "#D97706";
inline constexpr const char* Danger          = "#DC2626";
inline constexpr const char* SeatNormal      = "#10B981";
inline constexpr const char* SeatVip         = "#D97706";
inline constexpr const char* SeatSelected    = "#E05D5D";
inline constexpr const char* SeatBooked      = "#E2E8F0";

// ============================================================================
// GLOBAL APPLICATION STYLESHEET
// ============================================================================
inline QString globalStylesheet() {
    return R"(
        QMainWindow, QStackedWidget {
            background-color: #F5F6F8;
            color: #1E293B;
            font-family: -apple-system, BlinkMacSystemFont, "SF Pro Text", "Segoe UI", "Inter", "Roboto", sans-serif;
            font-size: 13px;
        }
        QWidget {
            color: #1E293B;
            font-family: -apple-system, BlinkMacSystemFont, "SF Pro Text", "Segoe UI", "Inter", "Roboto", sans-serif;
        }
        QDialog, QMessageBox {
            background-color: #FFFFFF;
            color: #1E293B;
        }
        QMessageBox QLabel {
            color: #1E293B;
            font-size: 13px;
            background: transparent;
        }
        QMessageBox QPushButton {
            background-color: #E05D5D;
            color: #FFFFFF;
            border: none;
            border-radius: 6px;
            padding: 7px 18px;
            font-weight: 600;
            min-width: 84px;
        }
        QMessageBox QPushButton:hover {
            background-color: #CC4B4B;
        }

        /* Inputs */
        QLineEdit, QSpinBox, QDoubleSpinBox, QDateEdit, QTimeEdit, QComboBox, QTextEdit {
            background-color: #F9FAFB;
            color: #1E293B;
            border: 1px solid #D1D5DB;
            border-radius: 6px;
            padding: 7px 11px;
            font-size: 13px;
            selection-background-color: #FECACA;
            selection-color: #991B1B;
        }
        QLineEdit:hover, QSpinBox:hover, QDoubleSpinBox:hover, QDateEdit:hover, QTimeEdit:hover, QComboBox:hover {
            border: 1px solid #9CA3AF;
            background-color: #FFFFFF;
        }
        QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QDateEdit:focus, QTimeEdit:focus, QComboBox:focus, QTextEdit:focus {
            border: 1px solid #E05D5D;
            background-color: #FFFFFF;
        }
        QComboBox::drop-down {
            border: none;
            width: 26px;
        }
        QComboBox QAbstractItemView {
            background-color: #FFFFFF;
            color: #1E293B;
            border: 1px solid #E5E7EB;
            border-radius: 6px;
            selection-background-color: #FEF2F2;
            selection-color: #D14343;
            outline: 0;
            padding: 4px;
        }

        /* Tables */
        QTableWidget {
            background-color: #FFFFFF;
            alternate-background-color: #FAFBFD;
            color: #1E293B;
            border: 1px solid #E5E7EB;
            border-radius: 8px;
            gridline-color: #F1F5F9;
            font-size: 13px;
            selection-background-color: #FEF2F2;
            selection-color: #1E293B;
            outline: 0;
        }
        QTableWidget::item {
            padding: 8px 12px;
            border-bottom: 1px solid #F1F5F9;
        }
        QTableWidget::item:hover {
            background-color: #F8FAFC;
        }
        QTableWidget::item:selected {
            background-color: #FEF2F2;
            color: #1E293B;
        }
        QHeaderView {
            background-color: transparent;
        }
        QHeaderView::section {
            background-color: #F8FAFC;
            color: #64748B;
            font-weight: 600;
            font-size: 12px;
            padding: 10px 12px;
            border: none;
            border-bottom: 1px solid #E5E7EB;
            border-right: 1px solid #F1F5F9;
        }

        /* Scrollbars */
        QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 2px;
        }
        QScrollBar::handle:vertical {
            background: #CBD5E1;
            min-height: 28px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical:hover {
            background: #94A3B8;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical,
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: none;
            height: 0px;
        }
        QScrollBar:horizontal {
            background: transparent;
            height: 8px;
            margin: 2px;
        }
        QScrollBar::handle:horizontal {
            background: #CBD5E1;
            min-width: 28px;
            border-radius: 4px;
        }
        QScrollBar::handle:horizontal:hover {
            background: #94A3B8;
        }
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal,
        QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
            background: none;
            width: 0px;
        }

        /* ToolTip */
        QToolTip {
            background-color: #1E293B;
            color: #FFFFFF;
            border: 1px solid #334155;
            padding: 6px 10px;
            border-radius: 6px;
        }
    )";
}

// ============================================================================
// REUSABLE COMPONENT STYLES
// ============================================================================
inline QString sidebarStyle() {
    return R"(
        QWidget#sidebar {
            background-color: #FFFFFF;
            border-right: 1px solid #E5E7EB;
        }
    )";
}

inline QString sidebarContainerStyle() {
    return sidebarStyle();
}

inline QString sidebarProfileCardStyle() {
    return R"(
        QFrame#profileCard {
            background-color: #F8FAFC;
            border: 1px solid #E5E7EB;
            border-radius: 8px;
        }
    )";
}

inline QString authCardStyle() {
    return R"(
        QFrame#authCard {
            background-color: #FFFFFF;
            border: 1px solid #E5E7EB;
            border-radius: 12px;
        }
    )";
}

inline QString luxuryCardStyle(const QString& selector = "QFrame") {
    return QString(R"(
        %1 {
            background-color: #FFFFFF;
            border: 1px solid #E5E7EB;
            border-radius: 10px;
        }
    )").arg(selector);
}

inline QString goldOutlineButtonStyle(int height = 38, int fontSize = 13) {
    return QString(R"(
        QPushButton {
            background-color: #FFFFFF;
            color: #334155;
            border: 1px solid #D1D5DB;
            border-radius: 6px;
            padding: 0 16px;
            min-height: %1px;
            font-size: %2px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #F8FAFC;
            color: #1E293B;
            border-color: #9CA3AF;
        }
        QPushButton:pressed {
            background-color: #F1F5F9;
        }
    )").arg(height).arg(fontSize);
}

inline QString successButtonStyle(int height = 38, int fontSize = 13) {
    return QString(R"(
        QPushButton {
            background-color: #059669;
            color: #FFFFFF;
            border: none;
            border-radius: 6px;
            padding: 0 18px;
            min-height: %1px;
            font-size: %2px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #047857;
        }
        QPushButton:pressed {
            background-color: #065F46;
        }
        QPushButton:disabled {
            background-color: #E5E7EB;
            color: #9CA3AF;
        }
    )").arg(height).arg(fontSize);
}

inline QString sidebarNavButtonStyle(bool active = false) {
    if (active) {
        return R"(
            QPushButton {
                background-color: #FEF2F2;
                color: #D14343;
                border: none;
                border-left: 3px solid #E05D5D;
                border-radius: 6px;
                padding: 10px 14px;
                font-size: 13px;
                font-weight: 600;
                text-align: left;
            }
        )";
    }
    return R"(
        QPushButton {
            background-color: transparent;
            color: #64748B;
            border: none;
            border-left: 3px solid transparent;
            border-radius: 6px;
            padding: 10px 14px;
            font-size: 13px;
            font-weight: 500;
            text-align: left;
        }
        QPushButton:hover {
            background-color: #F3F4F6;
            color: #1E293B;
        }
        QPushButton:pressed {
            background-color: #E5E7EB;
        }
    )";
}

inline QString sidebarSubtleButtonStyle() {
    return R"(
        QPushButton {
            background-color: #F9FAFB;
            color: #475569;
            border: 1px solid #E5E7EB;
            border-radius: 6px;
            padding: 8px 14px;
            font-size: 12px;
            font-weight: 500;
        }
        QPushButton:hover {
            background-color: #F3F4F6;
            color: #1E293B;
            border-color: #D1D5DB;
        }
    )";
}

inline QString sidebarDangerButtonStyle() {
    return R"(
        QPushButton {
            background-color: #FEF2F2;
            color: #DC2626;
            border: 1px solid #FECACA;
            border-radius: 6px;
            padding: 8px 14px;
            font-size: 12px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #FEE2E2;
            color: #B91C1C;
            border-color: #FCA5A5;
        }
    )";
}

inline QString primaryButtonStyle(int height = 38, int fontSize = 13) {
    return QString(R"(
        QPushButton {
            background-color: #E05D5D;
            color: #FFFFFF;
            border: none;
            border-radius: 6px;
            padding: 0 18px;
            min-height: %1px;
            font-size: %2px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #CC4B4B;
        }
        QPushButton:pressed {
            background-color: #B83B3B;
        }
        QPushButton:disabled {
            background-color: #E5E7EB;
            color: #9CA3AF;
        }
    )").arg(height).arg(fontSize);
}

inline QString goldButtonStyle(int height = 38, int fontSize = 13) {
    return QString(R"(
        QPushButton {
            background-color: #D97706;
            color: #FFFFFF;
            border: none;
            border-radius: 6px;
            padding: 0 18px;
            min-height: %1px;
            font-size: %2px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #B45309;
        }
        QPushButton:pressed {
            background-color: #92400E;
        }
    )").arg(height).arg(fontSize);
}

inline QString secondaryButtonStyle(int height = 38, int fontSize = 13) {
    return QString(R"(
        QPushButton {
            background-color: #FFFFFF;
            color: #334155;
            border: 1px solid #D1D5DB;
            border-radius: 6px;
            padding: 0 16px;
            min-height: %1px;
            font-size: %2px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #F8FAFC;
            color: #1E293B;
            border-color: #9CA3AF;
        }
        QPushButton:pressed {
            background-color: #F1F5F9;
        }
    )").arg(height).arg(fontSize);
}

inline QString dangerOutlineButtonStyle(int height = 38, int fontSize = 13) {
    return QString(R"(
        QPushButton {
            background-color: #FEF2F2;
            color: #DC2626;
            border: 1px solid #FECACA;
            border-radius: 6px;
            padding: 0 16px;
            min-height: %1px;
            font-size: %2px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #FEE2E2;
            color: #B91C1C;
            border-color: #FCA5A5;
        }
    )").arg(height).arg(fontSize);
}

inline QString sectionTitleStyle() {
    return R"(
        QLabel {
            color: #1E293B;
            font-size: 20px;
            font-weight: 700;
            background: transparent;
        }
    )";
}

inline QString sectionEyebrowStyle() {
    return R"(
        QLabel {
            color: #64748B;
            font-size: 12px;
            font-weight: 500;
            background: transparent;
        }
    )";
}

inline QString cardFrameStyle(const QString& objectName = "cinemaCard") {
    return QString(R"(
        QFrame#%1 {
            background-color: #FFFFFF;
            border: 1px solid #E5E7EB;
            border-radius: 10px;
        }
    )").arg(objectName);
}

inline QString ageBadgeStyle(int age) {
    QString bg = "#ECFDF5";
    QString fg = "#059669";
    QString border = "#A7F3D0";
    if (age >= 18) {
        bg = "#FEF2F2";
        fg = "#DC2626";
        border = "#FECACA";
    } else if (age >= 16) {
        bg = "#FFF7ED";
        fg = "#EA580C";
        border = "#FED7AA";
    } else if (age >= 13) {
        bg = "#FFFBEB";
        fg = "#D97706";
        border = "#FDE68A";
    }
    return QString(R"(
        QLabel {
            background-color: %1;
            color: %2;
            border: 1px solid %3;
            font-size: 11px;
            font-weight: 600;
            padding: 3px 9px;
            border-radius: 5px;
        }
    )").arg(bg, fg, border);
}

inline QString ageBadgeText(int age, bool detailed = false) {
    if (age <= 0) {
        return detailed ? "P · Mọi lứa tuổi" : "P";
    }
    return detailed ? QString("C%1 · Từ %1 tuổi").arg(age)
                    : QString("C%1 (%1+)").arg(age);
}

inline QString softPrimaryButtonStyle(int height = 36, int fontSize = 13) {
    return QString(R"(
        QPushButton {
            background-color: #FEF2F2;
            color: #D14343;
            border: 1px solid #FCA5A5;
            border-radius: 6px;
            padding: 0 16px;
            min-height: %1px;
            font-size: %2px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #E05D5D;
            color: #FFFFFF;
            border-color: #E05D5D;
        }
        QPushButton:pressed {
            background-color: #CC4B4B;
            color: #FFFFFF;
        }
    )").arg(height).arg(fontSize);
}

// ============================================================================
// SEAT MAP STYLES (CUSTOMER & CASHIER) - SOFT LIGHT MODE
// ============================================================================
inline QString seatButtonStyle(int status, bool isSelected = false) {
    if (isSelected) {
        return R"(
            QPushButton {
                background-color: #E05D5D;
                color: #FFFFFF;
                border: 1.5px solid #CC4B4B;
                border-radius: 6px;
                font-size: 11px;
                font-weight: 700;
            }
            QPushButton:hover {
                background-color: #CC4B4B;
            }
        )";
    }
    switch (status) {
        case 0: // Normal Available
            return R"(
                QPushButton {
                    background-color: #FFFFFF;
                    color: #334155;
                    border: 1.5px solid #CBD5E1;
                    border-radius: 6px;
                    font-size: 11px;
                    font-weight: 600;
                }
                QPushButton:hover {
                    background-color: #F1F5F9;
                    border-color: #64748B;
                    color: #0F172A;
                }
            )";
        case 1: // Normal Booked
            return R"(
                QPushButton {
                    background-color: #E2E8F0;
                    color: #94A3B8;
                    border: 1px solid #CBD5E1;
                    border-radius: 6px;
                    font-size: 10px;
                    font-weight: 500;
                }
            )";
        case 2: // VIP Available
            return R"(
                QPushButton {
                    background-color: #FFFBEB;
                    color: #B45309;
                    border: 1.5px solid #F59E0B;
                    border-radius: 6px;
                    font-size: 11px;
                    font-weight: 700;
                }
                QPushButton:hover {
                    background-color: #FEF3C7;
                    border-color: #D97706;
                    color: #92400E;
                }
            )";
        case 3: // VIP Booked
            return R"(
                QPushButton {
                    background-color: #F1F5F9;
                    color: #94A3B8;
                    border: 1px solid #CBD5E1;
                    border-radius: 6px;
                    font-size: 10px;
                    font-weight: 500;
                }
            )";
        default:
            return "";
    }
}

inline QString seatLegendSwatchStyle(int type) {
    switch (type) {
        case 0: // Normal available
            return "background-color: #FFFFFF; border: 1.5px solid #CBD5E1; border-radius: 4px;";
        case 1: // Normal booked
            return "background-color: #E2E8F0; border: 1px solid #CBD5E1; border-radius: 4px;";
        case 2: // VIP available
            return "background-color: #FFFBEB; border: 1.5px solid #F59E0B; border-radius: 4px;";
        case 3: // VIP booked
            return "background-color: #F1F5F9; border: 1px solid #CBD5E1; border-radius: 4px;";
        case 4: // Selected
            return "background-color: #E05D5D; border: 1.5px solid #CC4B4B; border-radius: 4px;";
        default:
            return "";
    }
}

inline QString screenBannerStyle() {
    return R"(
        QLabel {
            background-color: #F1F5F9;
            color: #475569;
            font-weight: 600;
            font-size: 12px;
            border-top: 3px solid #94A3B8;
            border-bottom: 1px solid #E2E8F0;
            border-left: 1px solid #E2E8F0;
            border-right: 1px solid #E2E8F0;
            border-radius: 6px;
        }
    )";
}

// ============================================================================
// IMAGE HELPERS (ROUNDED POSTER CROP & SMART PATH RESOLUTION)
// ============================================================================
inline QPixmap loadPosterPixmap(const QString& rawPath) {
    if (rawPath.trimmed().isEmpty()) return QPixmap();

    QStringList candidates;
    candidates << rawPath;

    QFileInfo fi(rawPath);
    if (fi.isRelative()) {
        QString appDir = QCoreApplication::applicationDirPath();
        candidates << QDir(appDir).filePath(rawPath);
        candidates << QDir(appDir).filePath("../" + rawPath);
        candidates << QDir(appDir).filePath("../../" + rawPath);
        candidates << QDir::current().filePath(rawPath);
    }

    for (const QString& candidate : candidates) {
        if (QFileInfo::exists(candidate)) {
            QPixmap px(candidate);
            if (!px.isNull()) return px;
        }
    }
    return QPixmap();
}

inline QPixmap roundedCoverPixmap(const QPixmap& src, int targetW, int targetH, int radius = 6) {
    if (src.isNull() || targetW <= 0 || targetH <= 0) return QPixmap();

    QPixmap scaled = src.scaled(targetW, targetH, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    int x = (scaled.width() - targetW) / 2;
    int y = (scaled.height() - targetH) / 2;
    QPixmap cropped = scaled.copy(x, y, targetW, targetH);

    QPixmap dest(targetW, targetH);
    dest.fill(Qt::transparent);

    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QPainterPath path;
    path.addRoundedRect(0, 0, targetW, targetH, radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, cropped);
    painter.end();

    return dest;
}

} // namespace Theme

#endif // UI_COMMON_THEME_H
