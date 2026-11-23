#include "MyTicketsWidget.h"
#include "../common/Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>

MyTicketsWidget::MyTicketsWidget(const QString& maKH, TicketManager* ticketManager,
                                 ShowtimeManager* showtimeManager, MovieManager* movieManager, QWidget *parent)
    : QWidget(parent), maKhachHang(maKH), ticketManager(ticketManager), showtimeManager(showtimeManager), movieManager(movieManager)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(26, 22, 26, 20);
    layout->setSpacing(14);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setSpacing(12);

    QVBoxLayout *titleCol = new QVBoxLayout();
    titleCol->setSpacing(2);
    QLabel *lblEyebrow = new QLabel("LỊCH SỬ ĐẶT VÉ CỦA BẠN", this);
    lblEyebrow->setStyleSheet(Theme::sectionEyebrowStyle());
    QLabel *lblTitle = new QLabel("VÉ CỦA TÔI", this);
    lblTitle->setStyleSheet(Theme::sectionTitleStyle());
    titleCol->addWidget(lblEyebrow);
    titleCol->addWidget(lblTitle);

    topLayout->addLayout(titleCol);
    topLayout->addStretch();

    txtSearch = new QLineEdit(this);
    txtSearch->setPlaceholderText("Tìm theo mã vé hoặc mã đơn...");
    txtSearch->setFixedWidth(280);
    txtSearch->setFixedHeight(40);

    QPushButton *btnSearch = new QPushButton("Tìm Kiếm", this);
    btnSearch->setCursor(Qt::PointingHandCursor);
    btnSearch->setStyleSheet(Theme::primaryButtonStyle(38, 13));

    topLayout->addWidget(txtSearch, 0, Qt::AlignVCenter);
    topLayout->addWidget(btnSearch, 0, Qt::AlignVCenter);
    layout->addLayout(topLayout);

    tableTickets = new QTableWidget(this);
    tableTickets->setColumnCount(8);
    tableTickets->setHorizontalHeaderLabels(QStringList() << "STT" << "Mã Vé" << "Mã Đơn" << "Tên Phim" << "Suất Chiếu" << "Ghế" << "Giá Tiền" << "Trạng Thái");
    tableTickets->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableTickets->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableTickets->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    tableTickets->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    tableTickets->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tableTickets->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tableTickets->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tableTickets->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    tableTickets->verticalHeader()->setDefaultSectionSize(46);
    tableTickets->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableTickets->setSelectionMode(QAbstractItemView::SingleSelection);
    tableTickets->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableTickets->setAlternatingRowColors(true);
    tableTickets->verticalHeader()->setVisible(false);

    layout->addWidget(tableTickets, 1);

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
        QString tenPhim = "Không xác định";
        QString suatInfo = "Không xác định";
        if (showtime) {
            auto movie = movieManager->findById(showtime->getMaPhim());
            if (movie) tenPhim = QString::fromStdString(movie->getTenPhim());
            suatInfo = QString::fromStdString(showtime->getNgayChieu() + "  " + showtime->getGioChieu());
        }

        QString ghe = QString(QChar('A' + t.getHang())) + QString::number(t.getCot() + 1);

        QString trangThaiStr;
        QColor statusColor;
        std::string status = t.getTrangThai();
        if (status == "PENDING") {
            trangThaiStr = "Đang chờ duyệt";
            statusColor = QColor("#FFD166");
        } else if (status == "VALID") {
            trangThaiStr = "Hợp lệ";
            statusColor = QColor("#34D399");
        } else if (status == "USED") {
            trangThaiStr = "Đã sử dụng";
            statusColor = QColor("#9AA2C2");
        } else {
            trangThaiStr = "Đã hủy";
            statusColor = QColor("#FF858D");
        }

        QFont boldFont = tableTickets->font();
        boldFont.setBold(true);

        QTableWidgetItem* itemIdx = new QTableWidgetItem(QString::number(rowCount + 1));
        itemIdx->setTextAlignment(Qt::AlignCenter);
        tableTickets->setItem(rowCount, 0, itemIdx);

        QTableWidgetItem* itemMaVe = new QTableWidgetItem(maVe);
        itemMaVe->setTextAlignment(Qt::AlignCenter);
        itemMaVe->setFont(boldFont);
        tableTickets->setItem(rowCount, 1, itemMaVe);

        QTableWidgetItem* itemMaDon = new QTableWidgetItem(maCumVe.isEmpty() ? "—" : maCumVe);
        itemMaDon->setTextAlignment(Qt::AlignCenter);
        itemMaDon->setFont(boldFont);
        itemMaDon->setForeground(QBrush(QColor("#8C92FF")));
        tableTickets->setItem(rowCount, 2, itemMaDon);

        tableTickets->setItem(rowCount, 3, new QTableWidgetItem(tenPhim));

        QTableWidgetItem* itemSuat = new QTableWidgetItem(suatInfo);
        itemSuat->setTextAlignment(Qt::AlignCenter);
        tableTickets->setItem(rowCount, 4, itemSuat);

        QTableWidgetItem* seatItem = new QTableWidgetItem(ghe);
        seatItem->setTextAlignment(Qt::AlignCenter);
        seatItem->setFont(boldFont);
        tableTickets->setItem(rowCount, 5, seatItem);

        QTableWidgetItem* priceItem = new QTableWidgetItem(QString("%L1 VND").arg(t.getGiaVe(), 0, 'f', 0));
        priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        priceItem->setFont(boldFont);
        priceItem->setForeground(QBrush(QColor("#FFD166")));
        tableTickets->setItem(rowCount, 6, priceItem);

        QTableWidgetItem *statusItem = new QTableWidgetItem(trangThaiStr);
        statusItem->setTextAlignment(Qt::AlignCenter);
        statusItem->setFont(boldFont);
        statusItem->setForeground(QBrush(statusColor));
        tableTickets->setItem(rowCount, 7, statusItem);

        rowCount++;
    }
}
