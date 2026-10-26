#include "SeatSelectionWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QScrollArea>
#include "models/SeatMap.h"

SeatSelectionWidget::SeatSelectionWidget(ShowtimeManager* showtimeManager, TicketManager* ticketManager, QWidget *parent)
    : QWidget(parent), showtimeManager(showtimeManager), ticketManager(ticketManager)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 12, 15, 12);
    mainLayout->setSpacing(10);

    // 1. Legend
    QHBoxLayout *legendLayout = new QHBoxLayout();
    legendLayout->setAlignment(Qt::AlignCenter);
    legendLayout->setSpacing(14);

    auto createLegendItem = [this](const QString& text, const QString& style) {
        QWidget* item = new QWidget(this);
        QHBoxLayout* l = new QHBoxLayout(item);
        l->setContentsMargins(2, 2, 2, 2);
        l->setSpacing(6);
        QLabel* badge = new QLabel(item);
        badge->setFixedSize(18, 18);
        badge->setStyleSheet(style);
        QLabel* lbl = new QLabel(text, item);
        lbl->setStyleSheet("color: #FFFFFF; font-size: 12px; font-weight: bold;");
        l->addWidget(badge);
        l->addWidget(lbl);
        return item;
    };

    legendLayout->addWidget(createLegendItem("Thường trống", "background-color: #2E8B57; border-radius: 4px; border: 1px solid #1E5E3A;"));
    legendLayout->addWidget(createLegendItem("Thường đã đặt", "background-color: #3D3D5C; border-radius: 4px; border: 1px solid #28283D;"));
    legendLayout->addWidget(createLegendItem("VIP trống", "background-color: #E6B042; border-radius: 4px; border: 1px solid #B88626;"));
    legendLayout->addWidget(createLegendItem("VIP đã đặt", "background-color: #7A1916; border-radius: 4px; border: 1px solid #52100E;"));
    legendLayout->addWidget(createLegendItem("Đang chọn", "background-color: #2980B9; border-radius: 4px; border: 1px solid #00D2D3;"));
    mainLayout->addLayout(legendLayout);

    // 2. Simulated Screen Bar (Red Gradient)
    QLabel *lblScreen = new QLabel("--- MÀN HÌNH CHIẾU PHIM ---", this);
    lblScreen->setAlignment(Qt::AlignCenter);
    lblScreen->setFixedHeight(36);
    lblScreen->setStyleSheet(R"(
        QLabel {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B22824, stop:1 #7D1916);
            color: #FFFFFF;
            border: 1px solid #E6B042;
            border-radius: 6px;
            font-weight: bold;
            letter-spacing: 4px;
            font-size: 13px;
        }
    )");
    mainLayout->addWidget(lblScreen);

    // 3. Scroll Area for Seat Grid
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { background: transparent; border: none; }");

    gridContainer = new QWidget(scrollArea);
    gridContainer->setStyleSheet("background: transparent;");
    seatGrid = new QGridLayout(gridContainer);
    seatGrid->setSpacing(5);
    seatGrid->setAlignment(Qt::AlignCenter);

    scrollArea->setWidget(gridContainer);
    mainLayout->addWidget(scrollArea, 1);

    // 4. Bottom Info and Actions
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(15);

    lblSelectedInfo = new QLabel("Ghế đã chọn: 0 ghế", this);
    lblSelectedInfo->setStyleSheet("font-size: 15px; font-weight: bold; color: #F5C542;");

    QString btnStyle = R"(
        QPushButton {
            background-color: #9E221E;
            color: #FFFFFF;
            font-size: 14px;
            font-weight: bold;
            border-radius: 6px;
            border: none;
            min-height: 38px;
            padding: 6px 20px;
        }
        QPushButton:hover {
            background-color: #B52A26;
            border: 1px solid #E6B042;
        }
    )";

    btnContinue = new QPushButton("Tiếp Tục ->", this);
    btnContinue->setCursor(Qt::PointingHandCursor);
    btnContinue->setStyleSheet(R"(
        QPushButton {
            background-color: #28A745;
            color: #FFFFFF;
            font-size: 14px;
            font-weight: bold;
            border-radius: 6px;
            border: none;
            min-height: 38px;
            padding: 6px 20px;
        }
        QPushButton:hover {
            background-color: #34C759;
            border: 1px solid #FFFFFF;
        }
    )");
    
    btnBack = new QPushButton("<- Quay Lại", this);
    btnBack->setCursor(Qt::PointingHandCursor);
    btnBack->setStyleSheet(btnStyle);

    bottomLayout->addWidget(lblSelectedInfo);
    bottomLayout->addStretch();
    bottomLayout->addWidget(btnBack);
    bottomLayout->addWidget(btnContinue);

    mainLayout->addLayout(bottomLayout);

    connect(btnContinue, &QPushButton::clicked, this, &SeatSelectionWidget::onContinue);
    connect(btnBack, &QPushButton::clicked, this, &SeatSelectionWidget::goBack);
}

void SeatSelectionWidget::loadSeatMap(const QString& maSuatChieu) {
    currentMaSuatChieu = maSuatChieu;
    
    // Clear old grid items
    QLayoutItem* item;
    while ((item = seatGrid->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }
    selectedSeats = DynamicArray<SelectedSeat>();
    updateSelectedInfo();

    auto seatMap = showtimeManager->findSeatMap(maSuatChieu.toStdString());
    if (!seatMap) {
        QMessageBox::critical(this, "Lỗi", "Không tìm thấy sơ đồ ghế cho suất này!");
        return;
    }

    int rows = seatMap->getSoHang();
    int cols = seatMap->getSoCot();

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QString seatName = QString(QChar('A' + r)) + QString::number(c + 1);
            QPushButton *btn = new QPushButton(seatName, gridContainer);
            // STANDARD SEAT BUTTON SIZE: 38x36 px, prevents overflow on A14, K15
            btn->setFixedSize(38, 36);
            
            btn->setProperty("row", r);
            btn->setProperty("col", c);

            int status = seatMap->getSeatStatus(r, c);
            btn->setProperty("status", status);

            updateSeatVisuals(btn, r, c, status);
            
            if (status == 1 || status == 3) {
                btn->setEnabled(false); // Booked or pending
                btn->setCursor(Qt::ForbiddenCursor);
            } else {
                btn->setCursor(Qt::PointingHandCursor);
                connect(btn, &QPushButton::clicked, this, &SeatSelectionWidget::onSeatClicked);
            }

            seatGrid->addWidget(btn, r, c);
        }
    }
}

void SeatSelectionWidget::updateSeatVisuals(QPushButton* btn, int row, int col, int status) {
    (void)row;
    (void)col;
    if (status == 0) { // Thuong trong
        btn->setStyleSheet(R"(
            QPushButton {
                background-color: #2E8B57;
                color: #FFFFFF;
                border: 1px solid #1E5E3A;
                border-radius: 4px;
                font-weight: bold;
                font-size: 10px;
                padding: 0;
            }
            QPushButton:hover {
                border: 1px solid #FFFFFF;
            }
        )");
    } else if (status == 1) { // Thuong da dat
        btn->setStyleSheet(R"(
            QPushButton {
                background-color: #3D3D5C;
                color: #888899;
                border: 1px solid #28283D;
                border-radius: 4px;
                font-weight: bold;
                font-size: 10px;
                padding: 0;
            }
        )");
    } else if (status == 2) { // VIP trong
        btn->setStyleSheet(R"(
            QPushButton {
                background-color: #E6B042;
                color: #1A1B41;
                border: 1px solid #B88626;
                border-radius: 4px;
                font-weight: bold;
                font-size: 10px;
                padding: 0;
            }
            QPushButton:hover {
                border: 1px solid #FFFFFF;
            }
        )");
    } else if (status == 3) { // VIP da dat
        btn->setStyleSheet(R"(
            QPushButton {
                background-color: #7A1916;
                color: #B37D7C;
                border: 1px solid #52100E;
                border-radius: 4px;
                font-weight: bold;
                font-size: 10px;
                padding: 0;
            }
        )");
    }
}

void SeatSelectionWidget::onSeatClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int r = btn->property("row").toInt();
    int c = btn->property("col").toInt();
    int status = btn->property("status").toInt();

    // Check if already selected
    bool found = false;
    int foundIndex = -1;
    for (int i = 0; i < selectedSeats.size(); ++i) {
        if (selectedSeats[i].row == r && selectedSeats[i].col == c) {
            found = true;
            foundIndex = i;
            break;
        }
    }

    if (found) {
        // Deselect
        selectedSeats.removeAt(foundIndex);
        updateSeatVisuals(btn, r, c, status);
    } else {
        // Select
        SelectedSeat s = {r, c};
        selectedSeats.push_back(s);
        btn->setStyleSheet(R"(
            QPushButton {
                background-color: #2980B9;
                color: #FFFFFF;
                border: 2px solid #00D2D3;
                border-radius: 4px;
                font-weight: bold;
                font-size: 10px;
                padding: 0;
            }
        )");
    }

    updateSelectedInfo();
}

void SeatSelectionWidget::updateSelectedInfo() {
    lblSelectedInfo->setText(QString("Ghế đã chọn: %1 ghế").arg(selectedSeats.size()));
}

void SeatSelectionWidget::onContinue() {
    if (selectedSeats.size() == 0) {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn ít nhất 1 ghế!");
        return;
    }
    
    QStringList seatStrs;
    for (int i = 0; i < selectedSeats.size(); ++i) {
        seatStrs << QString("%1,%2").arg(selectedSeats[i].row).arg(selectedSeats[i].col);
    }
    
    emit seatsSelected(currentMaSuatChieu, seatStrs);
}
