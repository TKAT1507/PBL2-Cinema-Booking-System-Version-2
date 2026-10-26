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
    lblTitle->setStyleSheet("color: #F5C542; font-size: 18px; font-weight: bold;");
    mainLayout->addWidget(lblTitle);
    
    // Overview
    QGroupBox* gbOverview = new QGroupBox("Tổng Quan Doanh Thu & Vé", this);
    QVBoxLayout* overviewLayout = new QVBoxLayout(gbOverview);
    lblTotalRevenue = new QLabel("Tổng doanh thu: 0 VND", this);
    lblTotalTickets = new QLabel("Tổng vé đã bán: 0 vé", this);
    lblTotalRevenue->setStyleSheet("font-size: 20px; font-weight: bold; color: #F5C542;");
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
    btnCheckDate->setStyleSheet("background-color: #E6B042; color: #1A1B41; font-weight: bold; min-height: 30px; border-radius: 4px; padding: 4px 14px;");
    lblDateRevenue = new QLabel("Doanh thu: 0 VND", this);
    lblDateRevenue->setStyleSheet("color: #F5C542; font-size: 14px; font-weight: bold;");
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
    btnCheckMovie->setStyleSheet("background-color: #E6B042; color: #1A1B41; font-weight: bold; min-height: 30px; border-radius: 4px; padding: 4px 14px;");
    lblMovieRevenue = new QLabel("Doanh thu: 0 VND", this);
    lblMovieRevenue->setStyleSheet("color: #F5C542; font-size: 14px; font-weight: bold;");
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
    lblTotalRevenue->setText(QString("Tong doanh thu: %1 VND").arg(total));
    lblTotalTickets->setText(QString("Tong ve da ban: %1").arg(count));
}

void StatisticsWidget::checkDateRevenue() {
    std::string date = dateEdit->date().toString("dd/MM/yyyy").toStdString();
    double rev = StatisticManager::getRevenueByDate(ticketManager->getAllTickets(), date);
    lblDateRevenue->setText(QString("Doanh thu: %1 VND").arg(rev));
}

void StatisticsWidget::checkMovieRevenue() {
    if(cbMovies->currentIndex() >= 0) {
        std::string movieId = cbMovies->currentData().toString().toStdString();
        double rev = StatisticManager::getRevenueByMovie(ticketManager->getAllTickets(), showtimeManager->getAllShowtimes(), movieId);
        lblMovieRevenue->setText(QString("Doanh thu: %1 VND").arg(rev));
    }
}
