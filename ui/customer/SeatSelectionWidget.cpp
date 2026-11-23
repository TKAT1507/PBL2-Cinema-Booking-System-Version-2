#include "SeatSelectionWidget.h"
#include "../common/Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QScrollArea>
#include <QFrame>
#include "models/SeatMap.h"

SeatSelectionWidget::SeatSelectionWidget(ShowtimeManager* showtimeManager, TicketManager* ticketManager, QWidget *parent)
    : QWidget(parent), showtimeManager(showtimeManager), ticketManager(ticketManager)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(26, 20, 26, 18);
    mainLayout->setSpacing(14);

    QHBoxLayout *headerRow = new QHBoxLayout();
    QVBoxLayout *titleCol = new QVBoxLayout();
    titleCol->setSpacing(2);
    QLabel *lblEyebrow = new QLabel("BƯỚC 2 / 4 • PHÒNG CHIẾU", this);
    lblEyebrow->setStyleSheet(Theme::sectionEyebrowStyle());
    QLabel *lblTitle = new QLabel("CHỌN VỊ TRÍ GHẾ NGỒI", this);
    lblTitle->setStyleSheet(Theme::sectionTitleStyle());
    titleCol->addWidget(lblEyebrow);
    titleCol->addWidget(lblTitle);
    headerRow->addLayout(titleCol);
    headerRow->addStretch();

    QFrame *legendFrame = new QFrame(this);
    legendFrame->setStyleSheet("QFrame { background-color: #22263C; border: 1px solid #323859; border-radius: 14px; }");
    QHBoxLayout *legendLayout = new QHBoxLayout(legendFrame);
    legendLayout->setAlignment(Qt::AlignCenter);
    legendLayout->setContentsMargins(16, 8, 16, 8);
    legendLayout->setSpacing(14);

    auto createLegendItem = [legendFrame](const QString& text, int swatchType) {
        QWidget* item = new QWidget(legendFrame);
        item->setStyleSheet("background: transparent; border: none;");
        QHBoxLayout* l = new QHBoxLayout(item);
        l->setContentsMargins(0, 0, 0, 0);
        l->setSpacing(6);
        QLabel* badge = new QLabel(item);
        badge->setFixedSize(16, 16);
        badge->setStyleSheet(Theme::seatLegendSwatchStyle(swatchType));
        QLabel* lbl = new QLabel(text, item);
        lbl->setStyleSheet("color: #C8CEE8; font-size: 11px; font-weight: 600; background: transparent; border: none;");
        l->addWidget(badge);
        l->addWidget(lbl);
        return item;
    };

    legendLayout->addWidget(createLegendItem("Thường trống", 0));
    legendLayout->addWidget(createLegendItem("Thường đã đặt", 1));
    legendLayout->addWidget(createLegendItem("VIP trống", 2));
    legendLayout->addWidget(createLegendItem("VIP đã đặt", 3));
    legendLayout->addWidget(createLegendItem("Đang chọn", 4));

    headerRow->addWidget(legendFrame, 0, Qt::AlignVCenter);
    mainLayout->addLayout(headerRow);

    QLabel *lblScreen = new QLabel("MÀN HÌNH CHIẾU PHIM", this);
    lblScreen->setAlignment(Qt::AlignCenter);
    lblScreen->setFixedHeight(36);
    lblScreen->setStyleSheet(Theme::screenBannerStyle());
    mainLayout->addWidget(lblScreen);

    QFrame *auditoriumFrame = new QFrame(this);
    auditoriumFrame->setObjectName("auditoriumBox");
    auditoriumFrame->setStyleSheet("QFrame#auditoriumBox { background-color: #1D2034; border: 1px solid #303654; border-radius: 20px; }");
    QVBoxLayout *auditoriumLayout = new QVBoxLayout(auditoriumFrame);
    auditoriumLayout->setContentsMargins(12, 14, 12, 14);

    scrollArea = new QScrollArea(auditoriumFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { background: transparent; border: none; }");

    gridContainer = new QWidget(scrollArea);
    gridContainer->setStyleSheet("background: transparent;");
    seatGrid = new QGridLayout(gridContainer);
    seatGrid->setSpacing(7);
    seatGrid->setAlignment(Qt::AlignCenter);

    scrollArea->setWidget(gridContainer);
    auditoriumLayout->addWidget(scrollArea);
    mainLayout->addWidget(auditoriumFrame, 1);

    QFrame *bottomBar = new QFrame(this);
    bottomBar->setStyleSheet("QFrame { background-color: #22263C; border: 1px solid #323859; border-radius: 16px; }");
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomBar);
    bottomLayout->setContentsMargins(20, 12, 20, 12);
    bottomLayout->setSpacing(12);

    lblSelectedInfo = new QLabel("Ghế đã chọn: Chưa chọn ghế", bottomBar);
    lblSelectedInfo->setStyleSheet("font-size: 14px; font-weight: bold; color: #FFD166; background: transparent; border: none;");
    lblSelectedInfo->setWordWrap(true);

    btnBack = new QPushButton("Quay Lại", bottomBar);
    btnBack->setCursor(Qt::PointingHandCursor);
    btnBack->setStyleSheet(Theme::secondaryButtonStyle(42, 14));

    btnContinue = new QPushButton("Tiếp Tục Chọn Combo", bottomBar);
    btnContinue->setCursor(Qt::PointingHandCursor);
    btnContinue->setStyleSheet(Theme::primaryButtonStyle(42, 14));

    bottomLayout->addWidget(lblSelectedInfo, 1);
    bottomLayout->addWidget(btnBack);
    bottomLayout->addWidget(btnContinue);

    mainLayout->addWidget(bottomBar);

    connect(btnContinue, &QPushButton::clicked, this, &SeatSelectionWidget::onContinue);
    connect(btnBack, &QPushButton::clicked, this, &SeatSelectionWidget::goBack);
}

void SeatSelectionWidget::loadSeatMap(const QString& maSuatChieu) {
    currentMaSuatChieu = maSuatChieu;

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
        QLabel *rowLblLeft = new QLabel(QString(QChar('A' + r)), gridContainer);
        rowLblLeft->setFixedSize(22, 36);
        rowLblLeft->setAlignment(Qt::AlignCenter);
        rowLblLeft->setStyleSheet("color: #707899; font-size: 11px; font-weight: bold; background: transparent;");
        seatGrid->addWidget(rowLblLeft, r, 0);

        for (int c = 0; c < cols; ++c) {
            QString seatName = QString(QChar('A' + r)) + QString::number(c + 1);
            QPushButton *btn = new QPushButton(seatName, gridContainer);
            btn->setFixedSize(38, 36);

            btn->setProperty("row", r);
            btn->setProperty("col", c);

            int status = seatMap->getSeatStatus(r, c);
            btn->setProperty("status", status);

            updateSeatVisuals(btn, r, c, status);

            if (status == 1 || status == 3) {
                btn->setEnabled(false);
                btn->setCursor(Qt::ForbiddenCursor);
            } else {
                btn->setCursor(Qt::PointingHandCursor);
                connect(btn, &QPushButton::clicked, this, &SeatSelectionWidget::onSeatClicked);
            }

            seatGrid->addWidget(btn, r, c + 1);
        }

        QLabel *rowLblRight = new QLabel(QString(QChar('A' + r)), gridContainer);
        rowLblRight->setFixedSize(22, 36);
        rowLblRight->setAlignment(Qt::AlignCenter);
        rowLblRight->setStyleSheet("color: #707899; font-size: 11px; font-weight: bold; background: transparent;");
        seatGrid->addWidget(rowLblRight, r, cols + 1);
    }
}

void SeatSelectionWidget::updateSeatVisuals(QPushButton* btn, int row, int col, int status) {
    (void)row;
    (void)col;
    btn->setStyleSheet(Theme::seatButtonStyle(status, false));
}

void SeatSelectionWidget::onSeatClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int r = btn->property("row").toInt();
    int c = btn->property("col").toInt();
    int status = btn->property("status").toInt();

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
        selectedSeats.removeAt(foundIndex);
        updateSeatVisuals(btn, r, c, status);
    } else {
        SelectedSeat s = {r, c};
        selectedSeats.push_back(s);
        btn->setStyleSheet(Theme::seatButtonStyle(status, true));
    }

    updateSelectedInfo();
}

void SeatSelectionWidget::updateSelectedInfo() {
    if (selectedSeats.size() == 0) {
        lblSelectedInfo->setText("Ghế đã chọn: Chưa chọn ghế");
        return;
    }
    QStringList names;
    for (int i = 0; i < selectedSeats.size(); ++i) {
        names << QString(QChar('A' + selectedSeats[i].row)) + QString::number(selectedSeats[i].col + 1);
    }
    lblSelectedInfo->setText(QString("Đã chọn %1 ghế:  %2").arg(selectedSeats.size()).arg(names.join(", ")));
}

void SeatSelectionWidget::onContinue() {
    if (selectedSeats.size() == 0) {
        QMessageBox::warning(this, "Thông Báo", "Vui lòng chọn ít nhất 1 ghế!");
        return;
    }

    QStringList seatStrs;
    for (int i = 0; i < selectedSeats.size(); ++i) {
        seatStrs << QString("%1,%2").arg(selectedSeats[i].row).arg(selectedSeats[i].col);
    }

    emit seatsSelected(currentMaSuatChieu, seatStrs);
}
