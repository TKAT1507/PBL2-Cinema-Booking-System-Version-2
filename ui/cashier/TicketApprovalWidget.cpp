#include "TicketApprovalWidget.h"
#include "../common/Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QTextEdit>
#include <QFrame>
#include "models/Showtime.h"
#include "models/Movie.h"
#include "models/SeatMap.h"

TicketApprovalWidget::TicketApprovalWidget(TicketManager* ticketMgr, ShowtimeManager* showtimeMgr,
                                           MovieManager* movieMgr, QWidget* parent)
    : QWidget(parent), ticketManager(ticketMgr), showtimeManager(showtimeMgr), movieManager(movieMgr)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(26, 22, 26, 20);
    mainLayout->setSpacing(14);

    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->setSpacing(10);

    QVBoxLayout *titleCol = new QVBoxLayout();
    titleCol->setSpacing(2);
    QLabel *lblTitle = new QLabel("Duyệt đơn vé", this);
    lblTitle->setStyleSheet(Theme::sectionTitleStyle());
    titleCol->addWidget(lblTitle);

    topBar->addLayout(titleCol);
    topBar->addStretch();

    txtSearch = new QLineEdit(this);
    txtSearch->setPlaceholderText("Tìm theo mã đơn / mã khách hàng...");
    txtSearch->setFixedWidth(260);
    txtSearch->setFixedHeight(40);

    btnRefresh = new QPushButton("Làm Mới", this);
    btnRefresh->setCursor(Qt::PointingHandCursor);
    btnRefresh->setStyleSheet(Theme::secondaryButtonStyle(38, 13));

    topBar->addWidget(txtSearch, 0, Qt::AlignVCenter);
    topBar->addWidget(btnRefresh, 0, Qt::AlignVCenter);
    mainLayout->addLayout(topBar);

    tableOrders = new QTableWidget(this);
    tableOrders->setColumnCount(9);
    tableOrders->setHorizontalHeaderLabels(QStringList() << "STT" << "Mã Đơn" << "Khách Hàng" << "Tên Phim"
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
    tableOrders->verticalHeader()->setDefaultSectionSize(46);
    tableOrders->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableOrders->setSelectionMode(QAbstractItemView::SingleSelection);
    tableOrders->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableOrders->setAlternatingRowColors(true);
    tableOrders->verticalHeader()->setVisible(false);
    mainLayout->addWidget(tableOrders, 1);

    QFrame *bottomBar = new QFrame(this);
    bottomBar->setStyleSheet("QFrame { background-color: #FFFFFF; border: 1px solid #E5E7EB; border-radius: 8px; }");
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomBar);
    bottomLayout->setContentsMargins(18, 10, 18, 10);
    bottomLayout->setSpacing(10);

    lblStatusCount = new QLabel("Đang chờ duyệt: 0 đơn", bottomBar);
    lblStatusCount->setStyleSheet("color: #D97706; font-size: 13px; font-weight: 600; background: transparent; border: none;");

    btnPrint = new QPushButton("In Hóa Đơn / Vé", bottomBar);
    btnPrint->setCursor(Qt::PointingHandCursor);
    btnPrint->setStyleSheet(Theme::goldOutlineButtonStyle(40, 13));

    btnCancel = new QPushButton("Hủy Đơn (Nhả Ghế)", bottomBar);
    btnCancel->setCursor(Qt::PointingHandCursor);
    btnCancel->setStyleSheet(Theme::dangerOutlineButtonStyle(40, 13));

    btnApprove = new QPushButton("Duyệt Đơn (Hợp Lệ)", bottomBar);
    btnApprove->setCursor(Qt::PointingHandCursor);
    btnApprove->setStyleSheet(Theme::successButtonStyle(40, 13));

    bottomLayout->addWidget(lblStatusCount);
    bottomLayout->addStretch();
    bottomLayout->addWidget(btnPrint);
    bottomLayout->addWidget(btnCancel);
    bottomLayout->addWidget(btnApprove);
    mainLayout->addWidget(bottomBar);

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
    QFont boldFont = tableOrders->font();
    boldFont.setBold(true);

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

        auto showtime = showtimeManager->findById(rep.getMaSuatChieu());
        QString tenPhim = "Không xác định";
        QString suatInfo = "Không xác định";
        if (showtime) {
            auto movie = movieManager->findById(showtime->getMaPhim());
            if (movie) tenPhim = QString::fromStdString(movie->getTenPhim());
            suatInfo = QString::fromStdString(showtime->getNgayChieu() + " " + showtime->getGioChieu() + " (" + showtime->getMaPhong() + ")");
        }

        QStringList seatsList;
        double totalGia = 0;
        for (int k = 0; k < ticketsInOrder.size(); ++k) {
            auto t = ticketsInOrder[k];
            seatsList << QString(QChar('A' + t.getHang())) + QString::number(t.getCot() + 1);
            totalGia += t.getGiaVe();
        }

        QTableWidgetItem* itIdx = new QTableWidgetItem(QString::number(rowCount + 1));
        itIdx->setTextAlignment(Qt::AlignCenter);
        tableOrders->setItem(rowCount, 0, itIdx);

        QTableWidgetItem* itDon = new QTableWidgetItem(QString::fromStdString(maCum));
        itDon->setTextAlignment(Qt::AlignCenter);
        itDon->setFont(boldFont);
        itDon->setForeground(QBrush(QColor("#64748B")));
        tableOrders->setItem(rowCount, 1, itDon);

        QTableWidgetItem* itKH = new QTableWidgetItem(QString::fromStdString(maKH));
        itKH->setTextAlignment(Qt::AlignCenter);
        tableOrders->setItem(rowCount, 2, itKH);

        tableOrders->setItem(rowCount, 3, new QTableWidgetItem(tenPhim));

        QTableWidgetItem* itSuat = new QTableWidgetItem(suatInfo);
        itSuat->setTextAlignment(Qt::AlignCenter);
        tableOrders->setItem(rowCount, 4, itSuat);

        QTableWidgetItem* itGhe = new QTableWidgetItem(seatsList.join(", "));
        itGhe->setTextAlignment(Qt::AlignCenter);
        itGhe->setFont(boldFont);
        tableOrders->setItem(rowCount, 5, itGhe);

        QString comboText = rep.getMaCombo().empty() ? "—" : QString::fromStdString(rep.getMaCombo());
        tableOrders->setItem(rowCount, 6, new QTableWidgetItem(comboText));

        QTableWidgetItem* itTien = new QTableWidgetItem(QString("%L1 VND").arg(totalGia, 0, 'f', 0));
        itTien->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        itTien->setFont(boldFont);
        itTien->setForeground(QBrush(QColor("#D97706")));
        tableOrders->setItem(rowCount, 7, itTien);

        QTableWidgetItem* itTT = new QTableWidgetItem("Đang chờ");
        itTT->setTextAlignment(Qt::AlignCenter);
        itTT->setFont(boldFont);
        itTT->setForeground(QBrush(QColor("#D97706")));
        tableOrders->setItem(rowCount, 8, itTT);

        rowCount++;
    }

    lblStatusCount->setText(QString("Đang chờ duyệt: %1 đơn").arg(rowCount));
}

void TicketApprovalWidget::onApproveClicked() {
    QString orderId = getSelectedOrderId();
    if (orderId.isEmpty()) {
        QMessageBox::warning(this, "Thông Báo", "Vui lòng chọn một đơn đặt vé cần duyệt!");
        return;
    }

    if (ticketManager->approveOrder(orderId.toStdString())) {
        ticketManager->save();
        QMessageBox::information(this, "Thành Công",
            QString("Đã duyệt đơn %1 thành công!\nToàn bộ vé trong đơn đã chuyển sang trạng thái HỢP LỆ (VALID).").arg(orderId));
        loadPendingOrders();
    } else {
        QMessageBox::critical(this, "Lỗi", "Không thể duyệt đơn vé này!");
    }
}

void TicketApprovalWidget::onCancelClicked() {
    QString orderId = getSelectedOrderId();
    if (orderId.isEmpty()) {
        QMessageBox::warning(this, "Thông Báo", "Vui lòng chọn một đơn đặt vé cần hủy!");
        return;
    }

    auto reply = QMessageBox::question(this, "Xác Nhận Hủy Đơn",
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
        QMessageBox::warning(this, "Thông Báo", "Vui lòng chọn một đơn vé để in hóa đơn!");
        return;
    }

    auto tickets = ticketManager->getTicketsByOrder(orderId.toStdString());
    if (tickets.isEmpty()) {
        QMessageBox::warning(this, "Thông Báo", "Không tìm thấy thông tin vé của đơn này!");
        return;
    }

    auto firstTicket = tickets[0];
    auto showtime = showtimeManager->findById(firstTicket.getMaSuatChieu());
    QString tenPhim = "Không xác định";
    QString roomDate = "Không xác định";
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

    QDialog dlg(this);
    dlg.setWindowTitle("Hóa Đơn & Vé Xem Phim — " + orderId);
    dlg.setFixedSize(500, 520);
    dlg.setStyleSheet(R"(
        QDialog {
            background-color: #FFFFFF;
            border: 1px solid #E5E7EB;
            border-radius: 10px;
        }
    )");

    QVBoxLayout *dLayout = new QVBoxLayout(&dlg);
    dLayout->setContentsMargins(24, 22, 24, 22);
    dLayout->setSpacing(14);

    QLabel *lblHead = new QLabel("HÓA ĐƠN & VÉ XEM PHIM", &dlg);
    lblHead->setAlignment(Qt::AlignCenter);
    lblHead->setStyleSheet("color: #D97706; font-size: 16px; font-weight: 600;");

    QTextEdit *txtReceipt = new QTextEdit(&dlg);
    txtReceipt->setReadOnly(true);
    txtReceipt->setStyleSheet(R"(
        QTextEdit {
            background-color: #F9FAFB;
            color: #1E293B;
            font-family: "Menlo", "Consolas", "Courier New", monospace;
            font-size: 12px;
            border: 1px solid #E5E7EB;
            border-radius: 8px;
            padding: 14px;
        }
    )");

    QString receiptText =
        "================================================\n"
        "                 PBL2 CINEMA                    \n"
        "            HÓA ĐƠN ĐẶT VÉ XEM PHIM             \n"
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
        "      CẢM ƠN QUÝ KHÁCH VÀ HẸN GẶP LẠI!          \n";

    txtReceipt->setText(receiptText);

    QPushButton *btnClose = new QPushButton("Đóng / Hoàn Tất In", &dlg);
    btnClose->setCursor(Qt::PointingHandCursor);
    btnClose->setStyleSheet(Theme::primaryButtonStyle(42, 14));
    connect(btnClose, &QPushButton::clicked, &dlg, &QDialog::accept);

    dLayout->addWidget(lblHead);
    dLayout->addWidget(txtReceipt, 1);
    dLayout->addWidget(btnClose);

    dlg.exec();
}
