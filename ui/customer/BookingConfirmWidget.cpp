#include "BookingConfirmWidget.h"
#include "../common/Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFrame>
#include <QScrollArea>
#include "models/SeatMap.h"
#include "models/Combo.h"
#include "models/Movie.h"
#include "utils/PriceCalculator.h"
#include <ctime>

BookingConfirmWidget::BookingConfirmWidget(const QString& maKH, TicketManager* ticketManager,
                                           ShowtimeManager* showtimeManager, ComboManager* comboManager, QWidget *parent)
    : QWidget(parent), maKhachHang(maKH), ticketManager(ticketManager),
      showtimeManager(showtimeManager), comboManager(comboManager),
      currentComboTotalPrice(0.0)
{
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(26, 20, 26, 20);
    rootLayout->setSpacing(14);
    rootLayout->setAlignment(Qt::AlignCenter);

    QFrame *card = new QFrame(this);
    card->setObjectName("confirmCard");
    card->setMinimumWidth(620);
    card->setMaximumWidth(740);
    card->setStyleSheet(Theme::luxuryCardStyle("confirmCard"));

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(30, 26, 30, 26);
    cardLayout->setSpacing(14);


    QLabel *lblTitle = new QLabel("Xác Nhận Thông Tin Đặt Vé", card);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("color: #1E293B; font-size: 22px; font-weight: 600; border: none; background: transparent;");

    QFrame *line = new QFrame(card);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #E5E7EB; max-height: 1px; border: none;");

    cardLayout->addWidget(lblTitle);
    cardLayout->addWidget(line);

    lblSummary = new QLabel(card);
    lblSummary->setWordWrap(true);
    lblSummary->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    lblSummary->setStyleSheet("color: #1E293B; font-size: 13px; border: none; background: transparent; padding: 10px;");

    QScrollArea *scroll = new QScrollArea(card);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea { background-color: #F9FAFB; border: 1px solid #E5E7EB; border-radius: 8px; }");
    scroll->setWidget(lblSummary);
    scroll->setMinimumHeight(260);
    cardLayout->addWidget(scroll, 1);

    lblTotal = new QLabel(card);
    lblTotal->setAlignment(Qt::AlignCenter);
    lblTotal->setStyleSheet(R"(
        QLabel {
            color: #D97706;
            font-size: 22px;
            font-weight: 600;
            background-color: #F8FAFC;
            border: 1.5px solid #E5E7EB;
            border-radius: 8px;
            padding: 12px;
        }
    )");
    cardLayout->addWidget(lblTotal);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(14);

    btnCancel = new QPushButton("Quay Lại", card);
    btnCancel->setCursor(Qt::PointingHandCursor);
    btnCancel->setStyleSheet(Theme::secondaryButtonStyle(46, 14));

    btnConfirm = new QPushButton("Xác Nhận Đặt Vé", card);
    btnConfirm->setCursor(Qt::PointingHandCursor);
    btnConfirm->setStyleSheet(Theme::primaryButtonStyle(46, 15));

    btnLayout->addWidget(btnCancel, 1);
    btnLayout->addWidget(btnConfirm, 2);
    cardLayout->addLayout(btnLayout);

    rootLayout->addWidget(card);

    connect(btnConfirm, &QPushButton::clicked, this, &BookingConfirmWidget::onConfirm);
    connect(btnCancel, &QPushButton::clicked, this, &BookingConfirmWidget::goBack);
}

void BookingConfirmWidget::setBookingData(const QString& maSuat, const QStringList& seats, const QStringList& combos) {
    currentMaSuat = maSuat;
    currentSeats = seats;
    currentCombos = combos;
    currentComboTotalPrice = 0.0;
    currentComboString = "";

    auto showtime = showtimeManager->findById(maSuat.toStdString());
    auto seatMap = showtimeManager->findSeatMap(maSuat.toStdString());

    QString summary = "<div style='font-size: 13px; line-height: 1.65; color: #1E293B;'>";
    summary += "<div style='color: #64748B; font-size: 12px; font-weight: 600; margin-bottom: 6px;'>THÔNG TIN SUẤT CHIẾU</div>";
    summary += "• <span style='color:#64748B;'>Mã suất chiếu:</span> <b>" + maSuat + "</b><br>";
    if (showtime) {
        summary += "• <span style='color:#64748B;'>Phòng chiếu:</span> <b>" + QString::fromStdString(showtime->getMaPhong()) + "</b><br>";
        summary += "• <span style='color:#64748B;'>Ngày chiếu:</span> <b>" + QString::fromStdString(showtime->getNgayChieu()) + "</b> &nbsp;|&nbsp; <span style='color:#64748B;'>Giờ chiếu:</span> <b style='color:#D97706;'>" + QString::fromStdString(showtime->getGioChieu()) + "</b><br>";
    }
    summary += "• <span style='color:#64748B;'>Số lượng ghế đặt:</span> <b>" + QString::number(seats.size()) + " ghế</b><br><br>";

    double totalSeatPrice = 0;
    summary += "<div style='color: #64748B; font-size: 12px; font-weight: 600; margin-bottom: 6px;'>DANH SÁCH GHẾ ĐÃ CHỌN</div>";
    summary += "<table width='100%' cellpadding='6' cellspacing='0' style='border-collapse: collapse;'>";
    summary += "<tr style='background-color: #F8FAFC; color: #D97706;'>"
               "<th align='center'>Ghế</th><th align='center'>Loại Ghế</th><th align='right'>Đơn Giá</th></tr>";

    for (const auto& seatStr : seats) {
        QStringList parts = seatStr.split(",");
        if (parts.size() == 2) {
            int r = parts[0].toInt();
            int c = parts[1].toInt();
            QString seatName = QString(QChar('A' + r)) + QString::number(c + 1);
            double price = 65000.0;
            if (showtime && seatMap) {
                price = PriceCalculator::calculateTicketPrice(seatMap->getSeatStatus(r, c), showtime->getNgayChieu(), showtime->getGioChieu());
            }
            bool isVip = seatMap && (seatMap->getSeatStatus(r, c) == 2 || seatMap->getSeatStatus(r, c) == 3);
            totalSeatPrice += price;

            summary += QString("<tr style='border-bottom: 1px solid #E5E7EB;'>"
                               "<td align='center'><b>%1</b></td>"
                               "<td align='center'>%2</td>"
                               "<td align='right'><b>%L3 VND</b></td></tr>")
                       .arg(seatName)
                       .arg(isVip ? "<span style='color: #D97706; font-weight: 600;'>VIP</span>" : "<span style='color:#64748B;'>Thường</span>")
                       .arg(price, 0, 'f', 0);
        }
    }
    summary += "</table><br>";

    summary += "<div style='color: #64748B; font-size: 12px; font-weight: 600; margin-bottom: 6px;'>BẮP NƯỚC / COMBO</div>";
    std::string comboStrStd = "";
    bool hasCombo = false;
    for (const auto& comboInfo : combos) {
        QStringList parts = comboInfo.split(",");
        if (parts.size() == 2) {
            std::string maCombo = parts[0].toStdString();
            int qty = parts[1].toInt();
            Combo* cb = comboManager->findById(maCombo);
            if (cb && qty > 0) {
                hasCombo = true;
                double comboPrice = cb->getGia() * qty;
                currentComboTotalPrice += comboPrice;
                if (!comboStrStd.empty()) comboStrStd += "; ";
                comboStrStd += cb->getTenCombo() + " x" + std::to_string(qty);
                summary += QString("• %1 &times; %2 = <b style='color: #D97706;'>%L3 VND</b><br>")
                           .arg(QString::fromStdString(cb->getTenCombo()))
                           .arg(qty)
                           .arg(comboPrice, 0, 'f', 0);
            }
        }
    }
    if (!hasCombo) {
        summary += "• <i style='color:#94A3B8;'>Không mua combo bắp nước</i><br>";
    }
    currentComboString = QString::fromStdString(comboStrStd);
    summary += "</div>";

    double total = totalSeatPrice + currentComboTotalPrice;
    lblSummary->setText(summary);
    lblTotal->setText(QString("TỔNG THANH TOÁN:  %L1 VND").arg(total, 0, 'f', 0));
}

void BookingConfirmWidget::onConfirm() {
    auto seatMap = showtimeManager->findSeatMap(currentMaSuat.toStdString());
    auto showtime = showtimeManager->findById(currentMaSuat.toStdString());
    if (!seatMap || !showtime) {
        QMessageBox::critical(this, "Lỗi", "Không thể truy xuất thông tin suất chiếu hoặc sơ đồ ghế!");
        return;
    }

    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    char dateBuf[16];
    std::strftime(dateBuf, sizeof(dateBuf), "%d/%m/%Y", &tm);
    std::string ngayMua(dateBuf);

    int seatCount = currentSeats.size();
    double comboPricePerTicket = (seatCount > 0) ? (currentComboTotalPrice / seatCount) : 0;

    std::string maCumVe = ticketManager->generateOrderId();

    for (const QString& seatStr : currentSeats) {
        QStringList parts = seatStr.split(",");
        if (parts.size() == 2) {
            int r = parts[0].toInt();
            int c = parts[1].toInt();

            if (seatMap->isSeatAvailable(r, c)) {
                int seatStatus = seatMap->getSeatStatus(r, c);
                seatMap->bookSeat(r, c);

                double seatPrice = PriceCalculator::calculateTicketPrice(seatStatus, showtime->getNgayChieu(), showtime->getGioChieu());
                double finalPrice = seatPrice + comboPricePerTicket;

                ticketManager->createTicket(currentMaSuat.toStdString(), maKhachHang.toStdString(),
                                            "", r, c, finalPrice, currentComboString.toStdString(),
                                            ngayMua, maCumVe, "PENDING");
            }
        }
    }

    showtimeManager->save();
    showtimeManager->saveSeatMaps();
    ticketManager->save();

    QMessageBox::information(this, "Đặt Vé Thành Công",
        QString("Đơn đặt vé đã được tạo thành công!\n\n"
                "• Mã Đơn Đặt Vé: %1\n"
                "• Trạng Thái: Đang chờ duyệt (PENDING)\n\n"
                "Vui lòng chờ thu ngân xác nhận đơn để vé có hiệu lực.")
                .arg(QString::fromStdString(maCumVe)));

    emit bookingComplete();
}
