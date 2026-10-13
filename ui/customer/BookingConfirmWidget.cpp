#include "BookingConfirmWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "models/SeatMap.h"
#include "models/Combo.h"
#include "utils/PriceCalculator.h"
#include <ctime>

BookingConfirmWidget::BookingConfirmWidget(const QString& maKH, TicketManager* ticketManager, 
                                           ShowtimeManager* showtimeManager, ComboManager* comboManager, QWidget *parent)
    : QWidget(parent), maKhachHang(maKH), ticketManager(ticketManager), 
      showtimeManager(showtimeManager), comboManager(comboManager),
      currentComboTotalPrice(0.0)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    lblSummary = new QLabel(this);
    lblSummary->setWordWrap(true);
    QFont f = lblSummary->font();
    f.setPointSize(12);
    lblSummary->setFont(f);

    lblTotal = new QLabel(this);
    QFont f2 = lblTotal->font();
    f2.setPointSize(16);
    f2.setBold(true);
    lblTotal->setFont(f2);
    lblTotal->setAlignment(Qt::AlignCenter);
    lblTotal->setStyleSheet("color: red;");

    btnConfirm = new QPushButton("Xac Nhan Dat Ve", this);
    btnConfirm->setStyleSheet("background-color: green; color: white; padding: 10px; font-weight: bold;");
    
    btnCancel = new QPushButton("Huy", this);
    btnCancel->setStyleSheet("background-color: red; color: white; padding: 10px; font-weight: bold;");

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnCancel);
    btnLayout->addWidget(btnConfirm);

    layout->addWidget(lblSummary);
    layout->addStretch();
    layout->addWidget(lblTotal);
    layout->addLayout(btnLayout);

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
    
    QString summary = "Thong tin dat ve:\n\n";
    summary += "Suat chieu: " + maSuat + "\n";
    if (showtime) {
        summary += "Ngay: " + QString::fromStdString(showtime->getNgayChieu()) + " - " + QString::fromStdString(showtime->getGioChieu()) + "\n";
    }
    summary += "So ghe: " + QString::number(seats.size()) + "\n\n";
    
    // Calculate seat prices
    double totalSeatPrice = 0;
    summary += "Chi tiet ghe:\n";
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
            summary += "  " + seatName + (isVip ? " (VIP)" : "") + ": " + QString::number(price, 'f', 0) + " VND\n";
        }
    }
    
    // Parse and calculate combo prices
    summary += "\nCombo:\n";
    std::string comboStrStd = "";
    for(const auto& comboInfo : combos) {
        // Format from ComboSelectionWidget: "maCombo,quantity"
        QStringList parts = comboInfo.split(",");
        if (parts.size() == 2) {
            std::string maCombo = parts[0].toStdString();
            int qty = parts[1].toInt();
            Combo* cb = comboManager->findById(maCombo);
            if (cb && qty > 0) {
                double comboPrice = cb->getGia() * qty;
                currentComboTotalPrice += comboPrice;
                if (!comboStrStd.empty()) comboStrStd += "; ";
                comboStrStd += cb->getTenCombo() + " x" + std::to_string(qty);
                summary += "  " + QString::fromStdString(cb->getTenCombo()) + " x" + QString::number(qty) 
                         + " = " + QString::number(comboPrice, 'f', 0) + " VND\n";
            }
        }
    }
    if (combos.isEmpty() || currentComboTotalPrice == 0) {
        summary += "  (Khong chon combo)\n";
    }
    currentComboString = QString::fromStdString(comboStrStd);

    double total = totalSeatPrice + currentComboTotalPrice;
    lblSummary->setText(summary);
    lblTotal->setText(QString("TONG CONG: %1 VND").arg(total, 0, 'f', 0));
}

void BookingConfirmWidget::onConfirm() {
    auto seatMap = showtimeManager->findSeatMap(currentMaSuat.toStdString());
    auto showtime = showtimeManager->findById(currentMaSuat.toStdString());
    if (!seatMap || !showtime) {
        QMessageBox::critical(this, "Loi", "Loi he thong!");
        return;
    }

    // Get current date once
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    char dateBuf[11];
    std::strftime(dateBuf, sizeof(dateBuf), "%d/%m/%Y", &tm);
    std::string ngayMua(dateBuf);
    
    // Distribute combo cost evenly across tickets
    int seatCount = currentSeats.size();
    double comboPricePerTicket = (seatCount > 0) ? (currentComboTotalPrice / seatCount) : 0;

    // Book seats & Create tickets
    for(const QString& seatStr : currentSeats) {
        QStringList parts = seatStr.split(",");
        if (parts.size() == 2) {
            int r = parts[0].toInt();
            int c = parts[1].toInt();
            
            if (seatMap->isSeatAvailable(r, c)) {
                // BUG #2 FIX: Get seat status BEFORE booking (status changes after bookSeat)
                int seatStatus = seatMap->getSeatStatus(r, c);
                
                // Now book the seat (changes status: 0->1, 2->3)
                seatMap->bookSeat(r, c);
                
                // Calculate price using ORIGINAL seat status
                double seatPrice = PriceCalculator::calculateTicketPrice(seatStatus, showtime->getNgayChieu(), showtime->getGioChieu());
                double finalPrice = seatPrice + comboPricePerTicket;
                
                // BUG #3 FIX: Pass combo string and include combo price in ticket
                ticketManager->createTicket(currentMaSuat.toStdString(), maKhachHang.toStdString(), 
                                            "", r, c, finalPrice, currentComboString.toStdString(), ngayMua);
            }
        }
    }

    showtimeManager->save();
    showtimeManager->saveSeatMaps();
    ticketManager->save();

    QMessageBox::information(this, "Thanh cong", "Dat ve thanh cong!");
    emit bookingComplete();
}
