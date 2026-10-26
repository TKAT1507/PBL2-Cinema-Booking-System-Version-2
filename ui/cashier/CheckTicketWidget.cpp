#include "CheckTicketWidget.h"
#include "managers/TicketManager.h"
#include "managers/ShowtimeManager.h"
#include "managers/MovieManager.h"
#include "models/Ticket.h"
#include "models/Showtime.h"
#include "models/Movie.h"
#include <QMessageBox>

CheckTicketWidget::CheckTicketWidget(TicketManager* ticketMgr, ShowtimeManager* showtimeMgr,
                                     MovieManager* movieMgr, QWidget* parent)
    : QWidget(parent), ticketManager(ticketMgr), showtimeManager(showtimeMgr), movieManager(movieMgr) {
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    layout->addWidget(new QLabel("Nhap Ma Ve:"));
    ticketIdInput = new QLineEdit(this);
    layout->addWidget(ticketIdInput);
    
    btnCheck = new QPushButton("Kiem Tra", this);
    layout->addWidget(btnCheck);
    
    resultLabel = new QLabel(this);
    layout->addWidget(resultLabel);
    
    btnConfirm = new QPushButton("Xac Nhan Su Dung", this);
    btnConfirm->setEnabled(false);
    layout->addWidget(btnConfirm);
    layout->addStretch();
    
    connect(btnCheck, &QPushButton::clicked, this, &CheckTicketWidget::checkTicket);
    connect(btnConfirm, &QPushButton::clicked, this, &CheckTicketWidget::confirmTicket);
}

void CheckTicketWidget::checkTicket() {
    std::string id = ticketIdInput->text().toStdString();
    Ticket* t = ticketManager->findById(id);
    if(!t) {
        resultLabel->setText("<font color='red'>Khong tim thay ve!</font>");
        btnConfirm->setEnabled(false);
        return;
    }
    
    Showtime* st = showtimeManager->findById(t->getMaSuatChieu());
    std::string movieName = "Unknown";
    if(st) {
        Movie* mv = movieManager->findById(st->getMaPhim());
        if(mv) movieName = mv->getTenPhim();
    }
    
    bool isValid = (t->getTrangThai() == "VALID");
    QString status;
    if (t->getTrangThai() == "VALID") {
        status = "<font color='#28A745'><b>HỢP LỆ (ĐÃ DUYỆT / CHƯA SỬ DỤNG)</b></font>";
    } else if (t->getTrangThai() == "PENDING") {
        status = "<font color='#F5C542'><b>ĐANG CHỜ DUYỆT (PENDING)</b></font>";
    } else if (t->getTrangThai() == "USED") {
        status = "<font color='#AAAAAA'><b>ĐÃ SỬ DỤNG</b></font>";
    } else {
        status = "<font color='#FF4D4D'><b>ĐÃ HỦY</b></font>";
    }
    
    QString info = QString("<b>Phim:</b> %1<br>"
                           "<b>Mã Đơn:</b> %2<br>"
                           "<b>Ghế:</b> %3%4<br>"
                           "<b>Combo:</b> %5<br>"
                           "<b>Giá Vé:</b> %L6 VND<br>"
                           "<b>Trạng Thái:</b> %7")
        .arg(QString::fromStdString(movieName))
        .arg(t->getMaCumVe().empty() ? "(Không)" : QString::fromStdString(t->getMaCumVe()))
        .arg((char)('A' + t->getHang())).arg(t->getCot() + 1)
        .arg(t->getMaCombo().empty() ? "(Không)" : QString::fromStdString(t->getMaCombo()))
        .arg(t->getGiaVe(), 0, 'f', 0)
        .arg(status);
        
    resultLabel->setText(info);
    currentTicketId = id;
    btnConfirm->setEnabled(isValid);
}

void CheckTicketWidget::confirmTicket() {
    if(ticketManager->markUsed(currentTicketId)) {
        QMessageBox::information(this, "Thanh Cong", "Da xac nhan su dung ve.");
        ticketManager->save();
        checkTicket(); // Refresh
    } else {
        QMessageBox::warning(this, "Loi", "Khong the xac nhan ve.");
    }
}
