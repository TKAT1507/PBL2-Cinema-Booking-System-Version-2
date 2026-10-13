#include "SeatSelectionWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "models/SeatMap.h"

SeatSelectionWidget::SeatSelectionWidget(ShowtimeManager* showtimeManager, QWidget *parent)
    : QWidget(parent), showtimeManager(showtimeManager)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Legend
    QHBoxLayout *legendLayout = new QHBoxLayout();
    legendLayout->setAlignment(Qt::AlignCenter);

    auto createLegendItem = [this](const QString& text, const QString& style) {
        QWidget* item = new QWidget(this);
        QHBoxLayout* l = new QHBoxLayout(item);
        l->setContentsMargins(5, 2, 5, 2);
        l->setSpacing(6);
        QLabel* badge = new QLabel(item);
        badge->setFixedSize(18, 18);
        badge->setStyleSheet(style);
        QLabel* lbl = new QLabel(text, item);
        lbl->setStyleSheet("color: #e0e0e0; font-size: 12px;");
        l->addWidget(badge);
        l->addWidget(lbl);
        return item;
    };

    legendLayout->addWidget(createLegendItem("Thuong trong", "background-color: #27ae60; border-radius: 4px; border: 1px solid #1e8449;"));
    legendLayout->addWidget(createLegendItem("Thuong da dat", "background-color: #555555; border-radius: 4px; border: 1px solid #444444;"));
    legendLayout->addWidget(createLegendItem("VIP trong", "background-color: #f39c12; border-radius: 4px; border: 2px solid #ffd700;"));
    legendLayout->addWidget(createLegendItem("VIP da dat", "background-color: #7b1113; border-radius: 4px; border: 1px solid #5a0c0e;"));
    legendLayout->addWidget(createLegendItem("Dang chon", "background-color: #2980b9; border-radius: 4px; border: 2px solid #00d2d3;"));
    mainLayout->addLayout(legendLayout);

    // Screen
    QLabel *lblScreen = new QLabel("--- MAN HINH CHIEU PHIM ---", this);
    lblScreen->setAlignment(Qt::AlignCenter);
    lblScreen->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0f3460, stop:1 #16213e); "
                            "color: #ffd700; border: 2px solid #e94560; border-radius: 10px; "
                            "padding: 10px; font-weight: bold; letter-spacing: 3px; font-size: 14px;");
    mainLayout->addWidget(lblScreen);

    // Seat Grid in a scroll or centered area
    seatGrid = new QGridLayout();
    seatGrid->setSpacing(6);
    seatGrid->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(seatGrid);

    // Bottom Info
    lblSelectedInfo = new QLabel("Ghe da chon: 0", this);
    lblSelectedInfo->setStyleSheet("font-size: 15px; font-weight: bold; color: #ffd700;");

    btnContinue = new QPushButton("Tiep Tuc ->", this);
    btnContinue->setStyleSheet("background-color: #27ae60; color: white; padding: 10px 24px; font-weight: bold; border-radius: 6px; font-size: 14px;");
    
    btnBack = new QPushButton("<- Quay Lai", this);
    btnBack->setStyleSheet("background-color: #16213e; color: #e0e0e0; border: 1px solid #0f3460; padding: 10px 20px; font-weight: bold; border-radius: 6px; font-size: 14px;");

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(lblSelectedInfo);
    bottomLayout->addStretch();
    bottomLayout->addWidget(btnBack);
    bottomLayout->addWidget(btnContinue);

    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);

    connect(btnContinue, &QPushButton::clicked, this, &SeatSelectionWidget::onContinue);
    connect(btnBack, &QPushButton::clicked, this, &SeatSelectionWidget::goBack);
}

void SeatSelectionWidget::loadSeatMap(const QString& maSuatChieu) {
    currentMaSuatChieu = maSuatChieu;
    
    // Clear old grid
    QLayoutItem* item;
    while ((item = seatGrid->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    selectedSeats = DynamicArray<SelectedSeat>(); // clear
    updateSelectedInfo();

    auto seatMap = showtimeManager->findSeatMap(maSuatChieu.toStdString());
    if (!seatMap) {
        QMessageBox::critical(this, "Loi", "Khong tim thay so do ghe cho suat nay!");
        return;
    }

    int rows = seatMap->getSoHang();
    int cols = seatMap->getSoCot();

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QString seatName = QString(QChar('A' + r)) + QString::number(c + 1);
            QPushButton *btn = new QPushButton(seatName, this);
            btn->setFixedSize(48, 48);
            
            // store pos in property
            btn->setProperty("row", r);
            btn->setProperty("col", c);

            int status = seatMap->getSeatStatus(r, c);
            btn->setProperty("status", status); // original status

            updateSeatVisuals(btn, r, c, status);
            
            if (status == 1 || status == 3) {
                btn->setEnabled(false); // booked
            } else {
                connect(btn, &QPushButton::clicked, this, &SeatSelectionWidget::onSeatClicked);
            }

            seatGrid->addWidget(btn, r, c);
        }
    }
}

void SeatSelectionWidget::updateSeatVisuals(QPushButton* btn, int row, int col, int status) {
    (void)row;
    (void)col;
    if (status == 0) {
        btn->setStyleSheet("background-color: #27ae60; color: white; border: 1px solid #1e8449; border-radius: 6px; font-weight: bold; font-size: 11px;");
    } else if (status == 1) {
        btn->setStyleSheet("background-color: #444444; color: #777777; border: 1px solid #333333; border-radius: 6px; font-size: 11px;");
    } else if (status == 2) {
        btn->setStyleSheet("background-color: #f39c12; color: #1a1a2e; border: 2px solid #ffd700; border-radius: 6px; font-weight: bold; font-size: 11px;");
    } else if (status == 3) {
        btn->setStyleSheet("background-color: #7b1113; color: #999999; border: 1px solid #5a0c0e; border-radius: 6px; font-size: 11px;");
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
        btn->setStyleSheet("background-color: #2980b9; color: white; border: 2px solid #00d2d3; border-radius: 6px; font-weight: bold; font-size: 11px;");
    }

    updateSelectedInfo();
}

void SeatSelectionWidget::updateSelectedInfo() {
    lblSelectedInfo->setText(QString("Ghe da chon: %1 ghe").arg(selectedSeats.size()));
}

void SeatSelectionWidget::onContinue() {
    if (selectedSeats.size() == 0) {
        QMessageBox::warning(this, "Canh bao", "Vui long chon it nhat 1 ghe!");
        return;
    }
    
    QStringList seatStrs;
    for (int i = 0; i < selectedSeats.size(); ++i) {
        seatStrs << QString("%1,%2").arg(selectedSeats[i].row).arg(selectedSeats[i].col);
    }
    
    emit seatsSelected(currentMaSuatChieu, seatStrs);
}
