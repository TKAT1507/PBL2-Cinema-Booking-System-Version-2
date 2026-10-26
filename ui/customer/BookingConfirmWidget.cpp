#include "BookingConfirmWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
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
    rootLayout->setContentsMargins(20, 15, 20, 15);
    rootLayout->setSpacing(15);
    rootLayout->setAlignment(Qt::AlignCenter);

    // Main Card Frame
    QFrame *card = new QFrame(this);
    card->setMaximumWidth(700);
    card->setStyleSheet(R"(
        QFrame {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2C2E6A, stop:1 #1E1F4B);
            border: 2px solid #E6B042;
            border-radius: 12px;
            padding: 15px;
        }
    )");

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(14);

    QLabel *lblTitle = new QLabel("XÁC NHẬN THÔNG TIN ĐẶT VÉ", card);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("color: #F5C542; font-size: 20px; font-weight: bold; border: none; background: transparent;");
    cardLayout->addWidget(lblTitle);

    QFrame *line = new QFrame(card);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #E6B042; max-height: 1px; border: none;");
    cardLayout->addWidget(line);

    // Summary Content in a clean scrollable area
    lblSummary = new QLabel(card);
    lblSummary->setWordWrap(true);
    lblSummary->setStyleSheet("color: #FFFFFF; font-size: 13px; line-height: 1.5; border: none; background: transparent;");
    
    QScrollArea *scroll = new QScrollArea(card);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea { background: transparent; border: none; }");
    scroll->setWidget(lblSummary);
    scroll->setMinimumHeight(240);
    cardLayout->addWidget(scroll);

    // Total Amount Box
    lblTotal = new QLabel(card);
    lblTotal->setAlignment(Qt::AlignCenter);
    lblTotal->setStyleSheet(R"(
        QLabel {
            color: #F5C542;
            font-size: 22px;
            font-weight: bold;
            background-color: #121330;
            border: 1px solid #E6B042;
            border-radius: 6px;
            padding: 10px;
        }
    )");
    cardLayout->addWidget(lblTotal);

    // Action Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);

    btnCancel = new QPushButton("Hủy Bỏ", card);
    btnCancel->setCursor(Qt::PointingHandCursor);
    btnCancel->setStyleSheet(R"(
        QPushButton {
            background-color: #9E221E;
            color: #FFFFFF;
            font-size: 15px;
            font-weight: bold;
            border-radius: 6px;
            border: none;
            min-height: 42px;
            padding: 6px 25px;
        }
        QPushButton:hover {
            background-color: #B52A26;
        }
    )");

    btnConfirm = new QPushButton("Xác Nhận Đặt Vé", card);
    btnConfirm->setCursor(Qt::PointingHandCursor);
    btnConfirm->setStyleSheet(R"(
        QPushButton {
            background-color: #28A745;
            color: #FFFFFF;
            font-size: 15px;
            font-weight: bold;
            border-radius: 6px;
            border: none;
            min-height: 42px;
            padding: 6px 25px;
        }
        QPushButton:hover {
            background-color: #34C759;
            border: 1px solid #FFFFFF;
        }
    )");

    btnLayout->addWidget(btnCancel);
    btnLayout->addWidget(btnConfirm);
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
    
    QString summary = "<div style='font-size: 13px;'>";
    summary += "<b style='color: #F5C542; font-size: 15px;'>THÔNG TIN SUẤT CHIẾU</b><br>";
    summary += "• <b>Mã suất chiếu:</b> " + maSuat + "<br>";
    if (showtime) {
        summary += "• <b>Phòng chiếu:</b> " + QString::fromStdString(showtime->getMaPhong()) + "<br>";
        summary += "• <b>Ngày chiếu:</b> " + QString::fromStdString(showtime->getNgayChieu()) + " &nbsp;|&nbsp; <b>Giờ chiếu:</b> " + QString::fromStdString(showtime->getGioChieu()) + "<br>";
    }
    summary += "• <b>Số lượng ghế đặt:</b> " + QString::number(seats.size()) + " ghế<br><br>";
    
    // Calculate seat prices
    double totalSeatPrice = 0;
    summary += "<b style='color: #F5C542; font-size: 15px;'>DANH SÁCH GHẾ ĐÃ CHỌN</b><br>";
    summary += "<table width='100%' cellpadding='4' style='border-collapse: collapse;'>";
    summary += "<tr style='background-color: #121330; color: #F5C542;'><th>Ghế</th><th>Loại Ghế</th><th align='right'>Đơn Giá</th></tr>";

    for(const auto& seatStr : seats) {
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

            summary += QString("<tr><td align='center'><b>%1</b></td><td align='center'>%2</td><td align='right'>%L3 VND</td></tr>")
                       .arg(seatName)
                       .arg(isVip ? "<span style='color: #F5C542;'>VIP</span>" : "Thường")
                       .arg(price, 0, 'f', 0);
        }
    }
    summary += "</table><br>";
    
    // Combo calculation
    summary += "<b style='color: #F5C542; font-size: 15px;'>BẮP NƯỚC / COMBO</b><br>";
    std::string comboStrStd = "";
    bool hasCombo = false;
    for(const auto& comboInfo : combos) {
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
                summary += QString("• %1 x %2 = <b style='color: #F5C542;'>%L3 VND</b><br>")
                           .arg(QString::fromStdString(cb->getTenCombo()))
                           .arg(qty)
                           .arg(comboPrice, 0, 'f', 0);
            }
        }
    }
    if (!hasCombo) {
        summary += "• <i>Không mua combo bắp nước</i><br>";
    }
    currentComboString = QString::fromStdString(comboStrStd);
    summary += "</div>";

    double total = totalSeatPrice + currentComboTotalPrice;
    lblSummary->setText(summary);
    lblTotal->setText(QString("TỔNG CỘNG: %L1 VND").arg(total, 0, 'f', 0));
}

void BookingConfirmWidget::onConfirm() {
    auto seatMap = showtimeManager->findSeatMap(currentMaSuat.toStdString());
    auto showtime = showtimeManager->findById(currentMaSuat.toStdString());
    if (!seatMap || !showtime) {
        QMessageBox::critical(this, "Lỗi", "Không thể truy xuất thông tin suất chiếu hoặc sơ đồ ghế!");
        return;
    }

    // Get current date once (dd/MM/yyyy)
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    char dateBuf[16];
    std::strftime(dateBuf, sizeof(dateBuf), "%d/%m/%Y", &tm);
    std::string ngayMua(dateBuf);
    
    // Distribute combo cost evenly across tickets
    int seatCount = currentSeats.size();
    double comboPricePerTicket = (seatCount > 0) ? (currentComboTotalPrice / seatCount) : 0;

    // Generate Order ID (Batch ID) for this whole group of tickets
    std::string maCumVe = ticketManager->generateOrderId();

    // Book seats on SeatMap & Create tickets with status PENDING
    for(const QString& seatStr : currentSeats) {
        QStringList parts = seatStr.split(",");
        if (parts.size() == 2) {
            int r = parts[0].toInt();
            int c = parts[1].toInt();
            
            if (seatMap->isSeatAvailable(r, c)) {
                int seatStatus = seatMap->getSeatStatus(r, c);
                seatMap->bookSeat(r, c);
                
                double seatPrice = PriceCalculator::calculateTicketPrice(seatStatus, showtime->getNgayChieu(), showtime->getGioChieu());
                double finalPrice = seatPrice + comboPricePerTicket;
                
                // Create ticket with PENDING status and Batch ID
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
                "Vui lòng chờ nhân viên / thu ngân duyệt đơn để vé có hiệu lực.")
                .arg(QString::fromStdString(maCumVe)));

    emit bookingComplete();
}
