#include "MyTicketsWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>

MyTicketsWidget::MyTicketsWidget(const QString& maKH, TicketManager* ticketManager, 
                                 ShowtimeManager* showtimeManager, MovieManager* movieManager, QWidget *parent)
    : QWidget(parent), maKhachHang(maKH), ticketManager(ticketManager), showtimeManager(showtimeManager), movieManager(movieManager)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    txtSearch = new QLineEdit(this);
    txtSearch->setPlaceholderText("Tim ve bang ma ve...");
    QPushButton *btnSearch = new QPushButton("Tim", this);
    searchLayout->addWidget(txtSearch);
    searchLayout->addWidget(btnSearch);

    tableTickets = new QTableWidget(this);
    tableTickets->setColumnCount(6);
    tableTickets->setHorizontalHeaderLabels(QStringList() << "Ma Ve" << "Phim" << "Suat" << "Ghe" << "Gia" << "Trang Thai");
    tableTickets->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableTickets->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableTickets->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableTickets->setAlternatingRowColors(true);

    layout->addLayout(searchLayout);
    layout->addWidget(tableTickets);

    connect(btnSearch, &QPushButton::clicked, this, &MyTicketsWidget::loadTickets);

    // Initial load
    loadTickets();
}

void MyTicketsWidget::loadTickets() {
    auto tickets = ticketManager->getTicketsByCustomer(maKhachHang.toStdString());
    
    QString search = txtSearch->text();
    
    tableTickets->setRowCount(0); // clear
    
    int rowCount = 0;
    for (int i = 0; i < tickets.size(); ++i) {
        auto t = tickets[i];
        
        QString maVe = QString::fromStdString(t.getMaVe());
        if (!search.isEmpty() && !maVe.contains(search, Qt::CaseInsensitive)) continue;
        
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
        std::string status = t.getTrangThai();
        if (status == "VALID") trangThaiStr = "Hop le";
        else if (status == "USED") trangThaiStr = "Da su dung";
        else trangThaiStr = "Da huy";

        tableTickets->setItem(rowCount, 0, new QTableWidgetItem(maVe));
        tableTickets->setItem(rowCount, 1, new QTableWidgetItem(tenPhim));
        tableTickets->setItem(rowCount, 2, new QTableWidgetItem(suatInfo));
        
        QTableWidgetItem* seatItem = new QTableWidgetItem(ghe);
        seatItem->setTextAlignment(Qt::AlignCenter);
        tableTickets->setItem(rowCount, 3, seatItem);
        
        QTableWidgetItem* priceItem = new QTableWidgetItem(QString::number(t.getGiaVe(), 'f', 0) + " VND");
        priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        tableTickets->setItem(rowCount, 4, priceItem);
        
        QTableWidgetItem *statusItem = new QTableWidgetItem(trangThaiStr);
        if (status == "VALID") statusItem->setForeground(QBrush(Qt::green));
        else if (status == "USED") statusItem->setForeground(QBrush(Qt::gray));
        else statusItem->setForeground(QBrush(Qt::red));
        tableTickets->setItem(rowCount, 5, statusItem);
        
        rowCount++;
    }
}
