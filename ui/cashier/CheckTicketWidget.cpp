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
    QString status = isValid ? "<font color='green'>HOP LE (CHUA SU DUNG)</font>" : "<font color='red'>DA SU DUNG HOAC HUY</font>";
    
    QString info = QString("Phim: %1\nGhe: %2%3\nGia: %4\nTrang Thai: %5")
        .arg(QString::fromStdString(movieName))
        .arg((char)('A' + t->getHang())).arg(t->getCot() + 1)
        .arg(t->getGiaVe())
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
