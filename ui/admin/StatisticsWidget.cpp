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
    
    // Overview
    QGroupBox* gbOverview = new QGroupBox("Tong Quan", this);
    QVBoxLayout* overviewLayout = new QVBoxLayout(gbOverview);
    lblTotalRevenue = new QLabel("Tong doanh thu: 0 VND", this);
    lblTotalTickets = new QLabel("Tong ve da ban: 0", this);
    lblTotalRevenue->setStyleSheet("font-size: 18px; font-weight: bold; color: green;");
    lblTotalTickets->setStyleSheet("font-size: 16px; font-weight: bold;");
    overviewLayout->addWidget(lblTotalRevenue);
    overviewLayout->addWidget(lblTotalTickets);
    mainLayout->addWidget(gbOverview);
    
    // Revenue by Date
    QGroupBox* gbDate = new QGroupBox("Doanh Thu Theo Ngay", this);
    QHBoxLayout* dateLayout = new QHBoxLayout(gbDate);
    dateEdit = new QDateEdit(QDate::currentDate(), this);
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    btnCheckDate = new QPushButton("Xem", this);
    lblDateRevenue = new QLabel("Doanh thu: 0 VND", this);
    dateLayout->addWidget(dateEdit);
    dateLayout->addWidget(btnCheckDate);
    dateLayout->addWidget(lblDateRevenue);
    mainLayout->addWidget(gbDate);
    
    // Revenue by Movie
    QGroupBox* gbMovie = new QGroupBox("Doanh Thu Theo Phim", this);
    QHBoxLayout* movieLayout = new QHBoxLayout(gbMovie);
    cbMovies = new QComboBox(this);
    
    auto activeMovies = movieManager->getActiveMovies();
    for(int i = 0; i < activeMovies.size(); ++i) {
        cbMovies->addItem(QString::fromStdString(activeMovies[i].getTenPhim()), QString::fromStdString(activeMovies[i].getMaPhim()));
    }
    
    btnCheckMovie = new QPushButton("Xem", this);
    lblMovieRevenue = new QLabel("Doanh thu: 0 VND", this);
    movieLayout->addWidget(cbMovies);
    movieLayout->addWidget(btnCheckMovie);
    movieLayout->addWidget(lblMovieRevenue);
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
