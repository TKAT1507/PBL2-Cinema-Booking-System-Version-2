#include "CheckTicketWidget.h"
#include "../common/Theme.h"
#include "managers/TicketManager.h"
#include "managers/ShowtimeManager.h"
#include "managers/MovieManager.h"
#include "models/Ticket.h"
#include "models/Showtime.h"
#include "models/Movie.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QFrame>

CheckTicketWidget::CheckTicketWidget(TicketManager* ticketMgr, ShowtimeManager* showtimeMgr,
                                     MovieManager* movieMgr, QWidget* parent)
    : QWidget(parent), ticketManager(ticketMgr), showtimeManager(showtimeMgr), movieManager(movieMgr)
{
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(26, 22, 26, 20);
    rootLayout->setSpacing(16);

    QVBoxLayout* titleCol = new QVBoxLayout();
    titleCol->setSpacing(2);
    QLabel* lblTitle = new QLabel("Soát vé", this);
    lblTitle->setStyleSheet(Theme::sectionTitleStyle());
    titleCol->addWidget(lblTitle);
    rootLayout->addLayout(titleCol);

    QFrame* card = new QFrame(this);
    card->setObjectName("checkCard");
    card->setFixedWidth(640);
    card->setStyleSheet(Theme::luxuryCardStyle("checkCard"));

    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(30, 28, 30, 28);
    cardLayout->setSpacing(16);

    QLabel* lblInputHint = new QLabel("Mã vé", card);
    lblInputHint->setStyleSheet("color: #D97706; font-size: 12px; font-weight: 600; border: none; background: transparent;");

    QHBoxLayout* searchRow = new QHBoxLayout();
    searchRow->setSpacing(10);

    ticketIdInput = new QLineEdit(card);
    ticketIdInput->setPlaceholderText("Ví dụ: TK001, TK002...");
    ticketIdInput->setFixedHeight(44);
    ticketIdInput->setStyleSheet(R"(
        QLineEdit {
            background-color: #F9FAFB;
            color: #1E293B;
            font-size: 14px;
            font-weight: 600;
            border: 1.5px solid #E5E7EB;
            border-radius: 6px;
            padding: 0 16px;
        }
        QLineEdit:focus {
            border: 1.5px solid #E05D5D;
            background-color: #FFFFFF;
        }
    )");

    btnCheck = new QPushButton("Kiểm Tra Vé", card);
    btnCheck->setFixedHeight(44);
    btnCheck->setCursor(Qt::PointingHandCursor);
    btnCheck->setStyleSheet(Theme::primaryButtonStyle(42, 14));

    searchRow->addWidget(ticketIdInput, 1);
    searchRow->addWidget(btnCheck);

    QFrame* resultBox = new QFrame(card);
    resultBox->setStyleSheet("QFrame { background-color: #F9FAFB; border: 1px solid #E5E7EB; border-radius: 8px; }");
    QVBoxLayout* resLayout = new QVBoxLayout(resultBox);
    resLayout->setContentsMargins(20, 18, 20, 18);

    resultLabel = new QLabel("<span style='color:#94A3B8;'>Nhập mã vé và nhấn <b>Kiểm Tra Vé</b> để xem thông tin chi tiết và trạng thái hiệu lực.</span>", resultBox);
    resultLabel->setWordWrap(true);
    resultLabel->setMinimumHeight(170);
    resultLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    resultLabel->setStyleSheet("color: #1E293B; font-size: 14px; border: none; background: transparent;");
    resLayout->addWidget(resultLabel);

    btnConfirm = new QPushButton("Xác Nhận Sử Dụng Vé (Check-in)", card);
    btnConfirm->setCursor(Qt::PointingHandCursor);
    btnConfirm->setEnabled(false);
    btnConfirm->setStyleSheet(Theme::successButtonStyle(46, 14));

    cardLayout->addWidget(lblInputHint);
    cardLayout->addLayout(searchRow);
    cardLayout->addWidget(resultBox);
    cardLayout->addWidget(btnConfirm);

    rootLayout->addSpacing(10);
    rootLayout->addWidget(card, 0, Qt::AlignHCenter);
    rootLayout->addStretch();

    connect(btnCheck, &QPushButton::clicked, this, &CheckTicketWidget::checkTicket);
    connect(ticketIdInput, &QLineEdit::returnPressed, this, &CheckTicketWidget::checkTicket);
    connect(btnConfirm, &QPushButton::clicked, this, &CheckTicketWidget::confirmTicket);
}

void CheckTicketWidget::checkTicket() {
    std::string id = ticketIdInput->text().trimmed().toStdString();
    if (id.empty()) {
        resultLabel->setText("<span style='color:#D97706;'>Vui lòng nhập mã vé cần kiểm tra.</span>");
        btnConfirm->setEnabled(false);
        return;
    }

    Ticket* t = ticketManager->findById(id);
    if (!t) {
        resultLabel->setText("<div style='color:#E05D5D; font-size:15px; font-weight:bold;'>Không tìm thấy vé với mã đã nhập!</div>"
                             "<div style='color:#64748B; margin-top:6px;'>Vui lòng kiểm tra lại mã vé của khách hàng.</div>");
        btnConfirm->setEnabled(false);
        return;
    }

    Showtime* st = showtimeManager->findById(t->getMaSuatChieu());
    std::string movieName = "Không xác định";
    QString showtimeInfo = "Không xác định";
    if (st) {
        Movie* mv = movieManager->findById(st->getMaPhim());
        if (mv) movieName = mv->getTenPhim();
        showtimeInfo = QString::fromStdString(st->getNgayChieu() + "  •  " + st->getGioChieu() + "  (" + st->getMaPhong() + ")");
    }

    bool isValid = (t->getTrangThai() == "VALID");
    QString status;
    if (t->getTrangThai() == "VALID") {
        status = "<span style='color:#059669; font-weight:bold;'>HỢP LỆ (ĐÃ DUYỆT / CHƯA SỬ DỤNG)</span>";
    } else if (t->getTrangThai() == "PENDING") {
        status = "<span style='color:#D97706; font-weight:bold;'>ĐANG CHỜ DUYỆT (PENDING)</span>";
    } else if (t->getTrangThai() == "USED") {
        status = "<span style='color:#64748B; font-weight:bold;'>ĐÃ SỬ DỤNG</span>";
    } else {
        status = "<span style='color:#E05D5D; font-weight:bold;'>ĐÃ HỦY</span>";
    }

    QString info = QString(
        "<div style='line-height: 1.85; color: #1E293B;'>"
        "<div style='color:#D97706; font-size:16px; font-weight:bold; margin-bottom:8px;'>%1</div>"
        "• <span style='color:#64748B;'>Mã vé:</span> <b>%2</b> &nbsp;&nbsp;|&nbsp;&nbsp; <span style='color:#64748B;'>Mã đơn:</span> <b>%3</b><br>"
        "• <span style='color:#64748B;'>Suất chiếu:</span> <b>%4</b><br>"
        "• <span style='color:#64748B;'>Vị trí ghế:</span> <b style='color:#E05D5D;'>%5%6</b><br>"
        "• <span style='color:#64748B;'>Combo:</span> %7<br>"
        "• <span style='color:#64748B;'>Giá vé:</span> <b style='color:#D97706;'>%L8 VND</b><br>"
        "• <span style='color:#64748B;'>Trạng thái:</span> %9"
        "</div>")
        .arg(QString::fromStdString(movieName).toUpper())
        .arg(QString::fromStdString(id))
        .arg(t->getMaCumVe().empty() ? "Tại quầy" : QString::fromStdString(t->getMaCumVe()))
        .arg(showtimeInfo)
        .arg((char)('A' + t->getHang())).arg(t->getCot() + 1)
        .arg(t->getMaCombo().empty() ? "Không" : QString::fromStdString(t->getMaCombo()))
        .arg(t->getGiaVe(), 0, 'f', 0)
        .arg(status);

    resultLabel->setText(info);
    currentTicketId = id;
    btnConfirm->setEnabled(isValid);
}

void CheckTicketWidget::confirmTicket() {
    if (ticketManager->markUsed(currentTicketId)) {
        QMessageBox::information(this, "Thành Công", "Đã xác nhận sử dụng vé thành công!");
        ticketManager->save();
        checkTicket();
    } else {
        QMessageBox::warning(this, "Lỗi", "Không thể xác nhận sử dụng vé này.");
    }
}
