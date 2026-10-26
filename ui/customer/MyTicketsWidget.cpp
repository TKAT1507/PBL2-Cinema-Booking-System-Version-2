#include "MyTicketsWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>

MyTicketsWidget::MyTicketsWidget(const QString& maKH, TicketManager* ticketManager, 
                                 ShowtimeManager* showtimeManager, MovieManager* movieManager, QWidget *parent)
    : QWidget(parent), maKhachHang(maKH), ticketManager(ticketManager), showtimeManager(showtimeManager), movieManager(movieManager)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(14);

    // Title & Search Bar
    QHBoxLayout *topLayout = new QHBoxLayout();
    
    QLabel *lblTitle = new QLabel("VÉ CỦA TÔI", this);
    lblTitle->setStyleSheet("color: #F5C542; font-size: 20px; font-weight: bold;");
    topLayout->addWidget(lblTitle);
    topLayout->addStretch();

    txtSearch = new QLineEdit(this);
    txtSearch->setPlaceholderText("Tìm vé bằng mã vé / mã đơn...");
    txtSearch->setFixedWidth(280);
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

    QPushButton *btnSearch = new QPushButton("Tìm", this);
    btnSearch->setCursor(Qt::PointingHandCursor);
    btnSearch->setStyleSheet(R"(
        QPushButton {
            background-color: #9E221E;
            color: #FFFFFF;
            font-size: 13px;
            font-weight: bold;
            border-radius: 5px;
            border: none;
            min-height: 30px;
            padding: 4px 16px;
        }
        QPushButton:hover {
            background-color: #B52A26;
        }
    )");

    topLayout->addWidget(txtSearch);
    topLayout->addWidget(btnSearch);
    layout->addLayout(topLayout);

    // Tickets Table
    tableTickets = new QTableWidget(this);
    tableTickets->setColumnCount(8);
    tableTickets->setHorizontalHeaderLabels(QStringList() << "#" << "Mã Vé" << "Mã Đơn" << "Tên Phim" << "Suất Chiếu" << "Ghế" << "Giá Tiền" << "Trạng Thái");
    tableTickets->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableTickets->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableTickets->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    tableTickets->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    tableTickets->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tableTickets->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tableTickets->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tableTickets->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    tableTickets->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableTickets->setSelectionMode(QAbstractItemView::SingleSelection);
    tableTickets->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableTickets->setAlternatingRowColors(true);
    tableTickets->verticalHeader()->setVisible(false);

    layout->addWidget(tableTickets);

    connect(btnSearch, &QPushButton::clicked, this, &MyTicketsWidget::loadTickets);
    connect(txtSearch, &QLineEdit::returnPressed, this, &MyTicketsWidget::loadTickets);

    loadTickets();
}

void MyTicketsWidget::loadTickets() {
    auto tickets = ticketManager->getTicketsByCustomer(maKhachHang.toStdString());
    
    QString search = txtSearch->text().trimmed();
    
    tableTickets->setRowCount(0);
    
    int rowCount = 0;
    for (int i = 0; i < tickets.size(); ++i) {
        auto t = tickets[i];
        
        QString maVe = QString::fromStdString(t.getMaVe());
        QString maCumVe = QString::fromStdString(t.getMaCumVe());

        if (!search.isEmpty()) {
            if (!maVe.contains(search, Qt::CaseInsensitive) && !maCumVe.contains(search, Qt::CaseInsensitive)) {
                continue;
            }
        }
        
        tableTickets->insertRow(rowCount);
        
        auto showtime = showtimeManager->findById(t.getMaSuatChieu());
        QString tenPhim = "Unknown";
        QString suatInfo = "Unknown";
        if (showtime) {
            auto movie = movieManager->findById(showtime->getMaPhim());
            if (movie) tenPhim = QString::fromStdString(movie->getTenPhim());
            suatInfo = QString::fromStdString(showtime->getNgayChieu() + " " + showtime->getGioChieu());
        }

        QString ghe = QString(QChar('A' + t.getHang())) + QString::number(t.getCot() + 1);
        
        QString trangThaiStr;
        QColor statusColor;
        std::string status = t.getTrangThai();
        if (status == "PENDING") {
            trangThaiStr = "Đang chờ duyệt";
            statusColor = QColor("#F5C542"); // Yellow-gold
        } else if (status == "VALID") {
            trangThaiStr = "Hợp lệ";
            statusColor = QColor("#28A745"); // Green
        } else if (status == "USED") {
            trangThaiStr = "Đã sử dụng";
            statusColor = QColor("#AAAAAA"); // Gray
        } else {
            trangThaiStr = "Đã hủy";
            statusColor = QColor("#FF4D4D"); // Red
        }

        // # column
        QTableWidgetItem* itemIdx = new QTableWidgetItem(QString::number(rowCount + 1));
        itemIdx->setTextAlignment(Qt::AlignCenter);
        tableTickets->setItem(rowCount, 0, itemIdx);

        // Ma Ve
        QTableWidgetItem* itemMaVe = new QTableWidgetItem(maVe);
        itemMaVe->setTextAlignment(Qt::AlignCenter);
        itemMaVe->setFont(QFont("Segoe UI", 9, QFont::Bold));
        tableTickets->setItem(rowCount, 1, itemMaVe);

        // Ma Cum Ve / Don
        QTableWidgetItem* itemMaDon = new QTableWidgetItem(maCumVe.isEmpty() ? "-" : maCumVe);
        itemMaDon->setTextAlignment(Qt::AlignCenter);
        itemMaDon->setForeground(QBrush(QColor("#F5C542")));
        tableTickets->setItem(rowCount, 2, itemMaDon);

        // Ten Phim
        tableTickets->setItem(rowCount, 3, new QTableWidgetItem(tenPhim));

        // Suat
        QTableWidgetItem* itemSuat = new QTableWidgetItem(suatInfo);
        itemSuat->setTextAlignment(Qt::AlignCenter);
        tableTickets->setItem(rowCount, 4, itemSuat);
        
        // Ghe
        QTableWidgetItem* seatItem = new QTableWidgetItem(ghe);
        seatItem->setTextAlignment(Qt::AlignCenter);
        seatItem->setFont(QFont("Segoe UI", 9, QFont::Bold));
        tableTickets->setItem(rowCount, 5, seatItem);
        
        // Gia
        QTableWidgetItem* priceItem = new QTableWidgetItem(QString("%L1 VND").arg(t.getGiaVe(), 0, 'f', 0));
        priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        priceItem->setForeground(QBrush(QColor("#F5C542")));
        tableTickets->setItem(rowCount, 6, priceItem);
        
        // Trang Thai
        QTableWidgetItem *statusItem = new QTableWidgetItem(trangThaiStr);
        statusItem->setTextAlignment(Qt::AlignCenter);
        statusItem->setFont(QFont("Segoe UI", 9, QFont::Bold));
        statusItem->setForeground(QBrush(statusColor));
        tableTickets->setItem(rowCount, 7, statusItem);
        
        rowCount++;
    }
}
