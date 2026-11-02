#include "StatisticsWidget.h"
#include "managers/TicketManager.h"
#include "managers/ShowtimeManager.h"
#include "managers/MovieManager.h"
#include "managers/StatisticManager.h"
#include "models/Movie.h"
#include <QGroupBox>
#include <QHBoxLayout>

StatisticsWidget::StatisticsWidget(TicketManager* ticketMgr, ShowtimeManager* showtimeMgr, MovieManager* movieMgr, QWidget* parent)
    : QWidget(parent), ticketManager(ticketMgr), showtimeManager(showtimeMgr), movieManager(movieMgr) {
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(14);

    QLabel *lblTitle = new QLabel("BÁO CÁO THỐNG KÊ DOANH THU", this);
    lblTitle->setStyleSheet("color: #EAB02F; font-size: 18px; font-weight: bold;");
    mainLayout->addWidget(lblTitle);
    
    // Overview
    QGroupBox* gbOverview = new QGroupBox("Tổng Quan Doanh Thu & Vé", this);
    QVBoxLayout* overviewLayout = new QVBoxLayout(gbOverview);
    lblTotalRevenue = new QLabel("Tổng doanh thu: 0 VND", this);
    lblTotalTickets = new QLabel("Tổng vé đã bán: 0 vé", this);
    lblTotalRevenue->setStyleSheet("font-size: 20px; font-weight: bold; color: #EAB02F;");
    lblTotalTickets->setStyleSheet("font-size: 16px; font-weight: bold; color: #FFFFFF;");
    overviewLayout->addWidget(lblTotalRevenue);
    overviewLayout->addWidget(lblTotalTickets);
    mainLayout->addWidget(gbOverview);
    
    // Revenue by Date
    QGroupBox* gbDate = new QGroupBox("Doanh Thu Theo Ngày", this);
    QHBoxLayout* dateLayout = new QHBoxLayout(gbDate);
    dateEdit = new QDateEdit(QDate::currentDate(), this);
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    btnCheckDate = new QPushButton("Xem Doanh Thu", this);
    btnCheckDate->setCursor(Qt::PointingHandCursor);
    btnCheckDate->setStyleSheet("background-color: #EAB02F; color: #1A1A1A; font-weight: bold; min-height: 30px; border-radius: 4px; padding: 4px 14px;");
    lblDateRevenue = new QLabel("Doanh thu: 0 VND", this);
    lblDateRevenue->setStyleSheet("color: #EAB02F; font-size: 14px; font-weight: bold;");
    dateLayout->addWidget(dateEdit);
    dateLayout->addWidget(btnCheckDate);
    dateLayout->addWidget(lblDateRevenue);
    dateLayout->addStretch();
    mainLayout->addWidget(gbDate);
    
    // Revenue by Movie
    QGroupBox* gbMovie = new QGroupBox("Doanh Thu Theo Phim", this);
    QHBoxLayout* movieLayout = new QHBoxLayout(gbMovie);
    cbMovies = new QComboBox(this);
    
    auto activeMovies = movieManager->getActiveMovies();
    for(int i = 0; i < activeMovies.size(); ++i) {
        cbMovies->addItem(QString::fromStdString(activeMovies[i].getTenPhim()), QString::fromStdString(activeMovies[i].getMaPhim()));
    }
    
    btnCheckMovie = new QPushButton("Xem Doanh Thu", this);
    btnCheckMovie->setCursor(Qt::PointingHandCursor);
    btnCheckMovie->setStyleSheet("background-color: #EAB02F; color: #1A1A1A; font-weight: bold; min-height: 30px; border-radius: 4px; padding: 4px 14px;");
    lblMovieRevenue = new QLabel("Doanh thu: 0 VND", this);
    lblMovieRevenue->setStyleSheet("color: #EAB02F; font-size: 14px; font-weight: bold;");
    movieLayout->addWidget(cbMovies);
    movieLayout->addWidget(btnCheckMovie);
    movieLayout->addWidget(lblMovieRevenue);
    movieLayout->addStretch();
    mainLayout->addWidget(gbMovie);
    
    mainLayout->addStretch();
    
    connect(btnCheckDate, &QPushButton::clicked, this, &StatisticsWidget::checkDateRevenue);
    connect(btnCheckMovie, &QPushButton::clicked, this, &StatisticsWidget::checkMovieRevenue);
    
    loadOverview();
}

void StatisticsWidget::loadOverview() {
    double total = StatisticManager::getTotalRevenue(ticketManager->getAllTickets());
    int count = StatisticManager::countTicketsSold(ticketManager->getAllTickets());
    lblTotalRevenue->setText(QString("Tổng doanh thu: %L1 VND").arg(total, 0, 'f', 0));
    lblTotalTickets->setText(QString("Tổng vé đã bán: %L1 vé").arg(count));
}

void StatisticsWidget::checkDateRevenue() {
    std::string date = dateEdit->date().toString("dd/MM/yyyy").toStdString();
    double rev = StatisticManager::getRevenueByDate(ticketManager->getAllTickets(), date);
    lblDateRevenue->setText(QString("Doanh thu ngày %1: %L2 VND").arg(QString::fromStdString(date)).arg(rev, 0, 'f', 0));
}

void StatisticsWidget::checkMovieRevenue() {
    if(cbMovies->currentIndex() >= 0) {
        std::string movieId = cbMovies->currentData().toString().toStdString();
        double rev = StatisticManager::getRevenueByMovie(ticketManager->getAllTickets(), showtimeManager->getAllShowtimes(), movieId);
        lblMovieRevenue->setText(QString("Doanh thu phim: %L1 VND").arg(rev, 0, 'f', 0));
    }
}
