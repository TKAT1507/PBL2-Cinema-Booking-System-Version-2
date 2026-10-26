#include "TicketApprovalWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QTextEdit>
#include "models/Showtime.h"
#include "models/Movie.h"
#include "models/SeatMap.h"

TicketApprovalWidget::TicketApprovalWidget(TicketManager* ticketMgr, ShowtimeManager* showtimeMgr, 
                                           MovieManager* movieMgr, QWidget* parent)
    : QWidget(parent), ticketManager(ticketMgr), showtimeManager(showtimeMgr), movieManager(movieMgr)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(12);

    // Top Header & Search / Refresh Bar
    QHBoxLayout *topBar = new QHBoxLayout();
    
    QLabel *lblTitle = new QLabel("XÁC NHẬN & DUYỆT ĐƠN VÉ (PENDING)", this);
    lblTitle->setStyleSheet("color: #F5C542; font-size: 18px; font-weight: bold;");
    topBar->addWidget(lblTitle);
    topBar->addStretch();

    txtSearch = new QLineEdit(this);
    txtSearch->setPlaceholderText("Tìm theo mã đơn / mã KH...");
    txtSearch->setFixedWidth(240);
    txtSearch->setStyleSheet(R"(
        QLineEdit {
            background-color: #E6B042;
            color: #1A1B41;
            font-size: 13px;
            font-weight: bold;
            border-radius: 5px;
            padding: 4px 10px;
            min-height: 28px;
        }
    )");

    btnRefresh = new QPushButton("Làm Mới", this);
    btnRefresh->setCursor(Qt::PointingHandCursor);
    btnRefresh->setStyleSheet(R"(
        QPushButton {
            background-color: #E6B042;
            color: #1A1B41;
            font-size: 13px;
            font-weight: bold;
            border-radius: 5px;
            border: 1px solid #C49129;
            min-height: 30px;
            padding: 4px 14px;
        }
        QPushButton:hover {
            background-color: #EDB952;
        }
    )");

    topBar->addWidget(txtSearch);
    topBar->addWidget(btnRefresh);
    mainLayout->addLayout(topBar);

    // Table of Pending Orders
    tableOrders = new QTableWidget(this);
    tableOrders->setColumnCount(9);
    tableOrders->setHorizontalHeaderLabels(QStringList() << "#" << "Mã Đơn" << "Khách Hàng" << "Tên Phim" 
                                                          << "Suất Chiếu" << "Ghế Đặt" << "Combo" 
                                                          << "Tổng Tiền" << "Trạng Thái");
    tableOrders->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableOrders->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableOrders->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    tableOrders->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    tableOrders->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tableOrders->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tableOrders->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tableOrders->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    tableOrders->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
    tableOrders->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableOrders->setSelectionMode(QAbstractItemView::SingleSelection);
    tableOrders->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableOrders->setAlternatingRowColors(true);
    tableOrders->verticalHeader()->setVisible(false);
    mainLayout->addWidget(tableOrders, 1);

    // Status label
    lblStatusCount = new QLabel("Đang chờ duyệt: 0 đơn", this);
    lblStatusCount->setStyleSheet("color: #F5C542; font-size: 13px; font-weight: bold;");

    // Action Buttons
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(lblStatusCount);
    bottomLayout->addStretch();

    btnApprove = new QPushButton("Duyệt Đơn (Hợp Lệ)", this);
    btnApprove->setCursor(Qt::PointingHandCursor);
    btnApprove->setStyleSheet(R"(
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

    btnCancel = new QPushButton("Hủy Đơn (Nhả Ghế)", this);
    btnCancel->setCursor(Qt::PointingHandCursor);
    btnCancel->setStyleSheet(R"(
        QPushButton {
            background-color: #9E221E;
            color: #FFFFFF;
            font-size: 14px;
            font-weight: bold;
            border-radius: 6px;
            border: none;
            min-height: 38px;
            padding: 6px 18px;
        }
        QPushButton:hover {
            background-color: #B52A26;
        }
    )");

    btnPrint = new QPushButton("In Hóa Đơn / Vé", this);
    btnPrint->setCursor(Qt::PointingHandCursor);
    btnPrint->setStyleSheet(R"(
        QPushButton {
            background-color: #E6B042;
            color: #1A1B41;
            font-size: 14px;
            font-weight: bold;
            border-radius: 6px;
            border: 1px solid #C49129;
            min-height: 38px;
            padding: 6px 18px;
        }
        QPushButton:hover {
            background-color: #EDB952;
            border: 1px solid #FFFFFF;
        }
    )");

    bottomLayout->addWidget(btnPrint);
    bottomLayout->addWidget(btnCancel);
    bottomLayout->addWidget(btnApprove);
    mainLayout->addLayout(bottomLayout);

    connect(btnRefresh, &QPushButton::clicked, this, &TicketApprovalWidget::loadPendingOrders);
    connect(txtSearch, &QLineEdit::textChanged, this, &TicketApprovalWidget::loadPendingOrders);
    connect(btnApprove, &QPushButton::clicked, this, &TicketApprovalWidget::onApproveClicked);
    connect(btnCancel, &QPushButton::clicked, this, &TicketApprovalWidget::onCancelClicked);
    connect(btnPrint, &QPushButton::clicked, this, &TicketApprovalWidget::onPrintClicked);

    loadPendingOrders();
}

QString TicketApprovalWidget::getSelectedOrderId() const {
    int row = tableOrders->currentRow();
    if (row < 0) return "";
    return tableOrders->item(row, 1)->text();
}

void TicketApprovalWidget::loadPendingOrders() {
    auto pendingReps = ticketManager->getPendingOrders();
    QString filter = txtSearch->text().trimmed();

    tableOrders->setRowCount(0);

    int rowCount = 0;
    for (int i = 0; i < pendingReps.size(); ++i) {
        auto rep = pendingReps[i];
        std::string maCum = rep.getMaCumVe();
        std::string maKH = rep.getMaKhachHang();

        if (!filter.isEmpty()) {
            QString qCum = QString::fromStdString(maCum);
            QString qKH = QString::fromStdString(maKH);
            if (!qCum.contains(filter, Qt::CaseInsensitive) && !qKH.contains(filter, Qt::CaseInsensitive)) {
                continue;
            }
        }

        auto ticketsInOrder = ticketManager->getTicketsByOrder(maCum);
        if (ticketsInOrder.isEmpty()) continue;

        tableOrders->insertRow(rowCount);

        // Movie & Showtime info
        auto showtime = showtimeManager->findById(rep.getMaSuatChieu());
        QString tenPhim = "Unknown";
        QString suatInfo = "Unknown";
        if (showtime) {
            auto movie = movieManager->findById(showtime->getMaPhim());
            if (movie) tenPhim = QString::fromStdString(movie->getTenPhim());
            suatInfo = QString::fromStdString(showtime->getNgayChieu() + " " + showtime->getGioChieu() + " (" + showtime->getMaPhong() + ")");
        }

        // Seats and Total Price
        QStringList seatsList;
        double totalGia = 0;
        for (int k = 0; k < ticketsInOrder.size(); ++k) {
            auto t = ticketsInOrder[k];
            seatsList << QString(QChar('A' + t.getHang())) + QString::number(t.getCot() + 1);
            totalGia += t.getGiaVe();
        }

        // #
        QTableWidgetItem* itIdx = new QTableWidgetItem(QString::number(rowCount + 1));
        itIdx->setTextAlignment(Qt::AlignCenter);
        tableOrders->setItem(rowCount, 0, itIdx);

        // Ma Don
        QTableWidgetItem* itDon = new QTableWidgetItem(QString::fromStdString(maCum));
        itDon->setTextAlignment(Qt::AlignCenter);
        itDon->setFont(QFont("Segoe UI", 9, QFont::Bold));
        itDon->setForeground(QBrush(QColor("#F5C542")));
        tableOrders->setItem(rowCount, 1, itDon);

        // Khach Hang
        QTableWidgetItem* itKH = new QTableWidgetItem(QString::fromStdString(maKH));
        itKH->setTextAlignment(Qt::AlignCenter);
        tableOrders->setItem(rowCount, 2, itKH);

        // Ten Phim
        tableOrders->setItem(rowCount, 3, new QTableWidgetItem(tenPhim));

        // Suat Chieu
        QTableWidgetItem* itSuat = new QTableWidgetItem(suatInfo);
        itSuat->setTextAlignment(Qt::AlignCenter);
        tableOrders->setItem(rowCount, 4, itSuat);

        // Danh sach ghe
        QTableWidgetItem* itGhe = new QTableWidgetItem(seatsList.join(", "));
        itGhe->setTextAlignment(Qt::AlignCenter);
        itGhe->setFont(QFont("Segoe UI", 9, QFont::Bold));
        tableOrders->setItem(rowCount, 5, itGhe);

        // Combo
        tableOrders->setItem(rowCount, 6, new QTableWidgetItem(QString::fromStdString(rep.getMaCombo())));

        // Tong Tien
        QTableWidgetItem* itTien = new QTableWidgetItem(QString("%L1 VND").arg(totalGia, 0, 'f', 0));
        itTien->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        itTien->setFont(QFont("Segoe UI", 9, QFont::Bold));
        itTien->setForeground(QBrush(QColor("#F5C542")));
        tableOrders->setItem(rowCount, 7, itTien);

        // Trang Thai
        QTableWidgetItem* itTT = new QTableWidgetItem("Đang chờ");
        itTT->setTextAlignment(Qt::AlignCenter);
        itTT->setFont(QFont("Segoe UI", 9, QFont::Bold));
        itTT->setForeground(QBrush(QColor("#F5C542")));
        tableOrders->setItem(rowCount, 8, itTT);

        rowCount++;
    }

    lblStatusCount->setText(QString("Đang chờ duyệt: %1 đơn").arg(rowCount));
}

void TicketApprovalWidget::onApproveClicked() {
    QString orderId = getSelectedOrderId();
    if (orderId.isEmpty()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng chọn một đơn đặt vé cần duyệt!");
        return;
    }

    if (ticketManager->approveOrder(orderId.toStdString())) {
        ticketManager->save();
        QMessageBox::information(this, "Thành công", 
            QString("Đã duyệt đơn %1 thành công!\nToàn bộ vé trong đơn đã chuyển sang trạng thái HỢP LỆ (VALID).").arg(orderId));
        loadPendingOrders();
    } else {
        QMessageBox::critical(this, "Lỗi", "Không thể duyệt đơn vé này!");
    }
}

void TicketApprovalWidget::onCancelClicked() {
    QString orderId = getSelectedOrderId();
    if (orderId.isEmpty()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng chọn một đơn đặt vé cần hủy!");
        return;
    }

    auto reply = QMessageBox::question(this, "Xác nhận hủy đơn", 
        QString("Bạn có chắc chắn muốn hủy đơn %1?\nCác ghế tương ứng trên sơ đồ sẽ được giải phóng về ghế trống.").arg(orderId),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (ticketManager->cancelOrder(orderId.toStdString(), showtimeManager)) {
            ticketManager->save();
            showtimeManager->save();
            showtimeManager->saveSeatMaps();
            QMessageBox::information(this, "Đã Hủy Đơn", 
                QString("Đơn vé %1 đã được hủy và toàn bộ ghế đã được giải phóng.").arg(orderId));
            loadPendingOrders();
        } else {
            QMessageBox::critical(this, "Lỗi", "Không thể hủy đơn vé này!");
        }
    }
}

void TicketApprovalWidget::onPrintClicked() {
    QString orderId = getSelectedOrderId();
    if (orderId.isEmpty()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng chọn một đơn vé để in hóa đơn!");
        return;
    }

    auto tickets = ticketManager->getTicketsByOrder(orderId.toStdString());
    if (tickets.isEmpty()) {
        QMessageBox::warning(this, "Thông báo", "Không tìm thấy thông tin vé của đơn này!");
        return;
    }

    auto firstTicket = tickets[0];
    auto showtime = showtimeManager->findById(firstTicket.getMaSuatChieu());
    QString tenPhim = "Unknown";
    QString roomDate = "Unknown";
    if (showtime) {
        auto movie = movieManager->findById(showtime->getMaPhim());
        if (movie) tenPhim = QString::fromStdString(movie->getTenPhim());
        roomDate = QString::fromStdString(showtime->getNgayChieu() + " " + showtime->getGioChieu() + " | Phòng: " + showtime->getMaPhong());
    }

    QStringList seats;
    double total = 0;
    for (int i = 0; i < tickets.size(); ++i) {
        seats << QString(QChar('A' + tickets[i].getHang())) + QString::number(tickets[i].getCot() + 1);
        total += tickets[i].getGiaVe();
    }

    // Receipt Dialog
    QDialog dlg(this);
    dlg.setWindowTitle("HÓA ĐƠN & VÉ XEM PHIM - " + orderId);
    dlg.setFixedSize(480, 500);
    dlg.setStyleSheet(R"(
        QDialog {
            background: #1E1F4B;
            border: 2px solid #E6B042;
            border-radius: 8px;
        }
    )");

    QVBoxLayout *dLayout = new QVBoxLayout(&dlg);
    dLayout->setContentsMargins(20, 20, 20, 20);
    dLayout->setSpacing(12);

    QTextEdit *txtReceipt = new QTextEdit(&dlg);
    txtReceipt->setReadOnly(true);
    txtReceipt->setStyleSheet(R"(
        QTextEdit {
            background-color: #2C2E6A;
            color: #FFFFFF;
            font-family: 'Consolas', monospace;
            font-size: 13px;
            border: 1px solid #4A4D94;
            border-radius: 6px;
            padding: 12px;
        }
    )");

    QString receiptText = 
        "================================================\n"
        "             HỆ THỐNG RẠP PHIM PBL2             \n"
        "              HÓA ĐƠN ĐẶT VÉ XEM PHIM           \n"
        "================================================\n"
        " Mã Đơn     : " + orderId + "\n"
        " Khách Hàng : " + QString::fromStdString(firstTicket.getMaKhachHang()) + "\n"
        " Phim       : " + tenPhim.toUpper() + "\n"
        " Suất Chiếu : " + roomDate + "\n"
        " Ghế        : " + seats.join(", ") + " (" + QString::number(seats.size()) + " vé)\n"
        " Combo      : " + (firstTicket.getMaCombo().empty() ? "(Không)" : QString::fromStdString(firstTicket.getMaCombo())) + "\n"
        " Ngày In    : " + QString::fromStdString(firstTicket.getNgayMua()) + "\n"
        "------------------------------------------------\n"
        " TỔNG TIỀN  : " + QString("%L1 VND").arg(total, 0, 'f', 0) + "\n"
        " TRẠNG THÁI : " + QString::fromStdString(firstTicket.getTrangThai()) + "\n"
        "================================================\n"
        "      CẢM ƠN QUÝ KHÁCH VÀ HẸN GẶP LẠI!        \n";

    txtReceipt->setText(receiptText);

    QPushButton *btnClose = new QPushButton("Đóng / In Vé", &dlg);
    btnClose->setStyleSheet(R"(
        QPushButton {
            background-color: #28A745;
            color: #FFFFFF;
            font-size: 14px;
            font-weight: bold;
            border-radius: 5px;
            min-height: 38px;
        }
        QPushButton:hover {
            background-color: #34C759;
        }
    )");
    connect(btnClose, &QPushButton::clicked, &dlg, &QDialog::accept);

    dLayout->addWidget(txtReceipt);
    dLayout->addWidget(btnClose);

    dlg.exec();
}
