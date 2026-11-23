#include "StatisticsWidget.h"
#include "../common/Theme.h"
#include "managers/TicketManager.h"
#include "managers/ShowtimeManager.h"
#include "managers/MovieManager.h"
#include "managers/StatisticManager.h"
#include "models/Movie.h"
#include <QFrame>
#include <QHBoxLayout>

StatisticsWidget::StatisticsWidget(TicketManager* ticketMgr, ShowtimeManager* showtimeMgr, MovieManager* movieMgr, QWidget* parent)
    : QWidget(parent), ticketManager(ticketMgr), showtimeManager(showtimeMgr), movieManager(movieMgr)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(26, 22, 26, 20);
    mainLayout->setSpacing(18);

    QVBoxLayout* titleCol = new QVBoxLayout();
    titleCol->setSpacing(2);
    QLabel* lblEyebrow = new QLabel("PHÂN TÍCH KINH DOANH RẠP PHIM", this);
    lblEyebrow->setStyleSheet(Theme::sectionEyebrowStyle());
    QLabel* lblTitle = new QLabel("BÁO CÁO THỐNG KÊ DOANH THU", this);
    lblTitle->setStyleSheet(Theme::sectionTitleStyle());
    titleCol->addWidget(lblEyebrow);
    titleCol->addWidget(lblTitle);
    mainLayout->addLayout(titleCol);

    // Row 1: Soft Midnight Glowing KPI Cards
    QHBoxLayout* kpiRow = new QHBoxLayout();
    kpiRow->setSpacing(18);

    // KPI 1: Total Revenue
    QFrame* revCard = new QFrame(this);
    revCard->setObjectName("kpiRevCard");
    revCard->setStyleSheet(R"(
        QFrame#kpiRevCard {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #36253A, stop:1 #22263C);
            border: 1px solid #6E4A5C;
            border-radius: 20px;
        }
    )");
    QVBoxLayout* revCardLayout = new QVBoxLayout(revCard);
    revCardLayout->setContentsMargins(24, 22, 24, 22);
    revCardLayout->setSpacing(6);

    QLabel* lblRevCap = new QLabel("TỔNG DOANH THU HỆ THỐNG", revCard);
    lblRevCap->setStyleSheet("color: #C9B2C8; font-size: 11px; font-weight: bold; letter-spacing: 1.5px; border: none; background: transparent;");
    lblTotalRevenue = new QLabel("0 VND", revCard);
    lblTotalRevenue->setStyleSheet("color: #FFD166; font-size: 30px; font-weight: bold; border: none; background: transparent;");
    revCardLayout->addWidget(lblRevCap);
    revCardLayout->addWidget(lblTotalRevenue);

    // KPI 2: Total Tickets Sold
    QFrame* tktCard = new QFrame(this);
    tktCard->setObjectName("kpiTktCard");
    tktCard->setStyleSheet(R"(
        QFrame#kpiTktCard {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #282B52, stop:1 #22263C);
            border: 1px solid #4B5294;
            border-radius: 20px;
        }
    )");
    QVBoxLayout* tktCardLayout = new QVBoxLayout(tktCard);
    tktCardLayout->setContentsMargins(24, 22, 24, 22);
    tktCardLayout->setSpacing(6);

    QLabel* lblTktCap = new QLabel("TỔNG SỐ VÉ ĐÃ BÁN", tktCard);
    lblTktCap->setStyleSheet("color: #AEB5E6; font-size: 11px; font-weight: bold; letter-spacing: 1.5px; border: none; background: transparent;");
    lblTotalTickets = new QLabel("0 vé", tktCard);
    lblTotalTickets->setStyleSheet("color: #9CA3FF; font-size: 30px; font-weight: bold; border: none; background: transparent;");
    tktCardLayout->addWidget(lblTktCap);
    tktCardLayout->addWidget(lblTotalTickets);

    kpiRow->addWidget(revCard, 1);
    kpiRow->addWidget(tktCard, 1);
    mainLayout->addLayout(kpiRow);

    // Card: Revenue by Date
    QFrame* dateCard = new QFrame(this);
    dateCard->setObjectName("statDateCard");
    dateCard->setStyleSheet("QFrame#statDateCard { background-color: #22263C; border: 1px solid #323859; border-radius: 18px; }");
    QVBoxLayout* dateCardLayout = new QVBoxLayout(dateCard);
    dateCardLayout->setContentsMargins(24, 20, 24, 20);
    dateCardLayout->setSpacing(12);

    QLabel* lblDateHeader = new QLabel("TRA CỨU DOANH THU THEO NGÀY", dateCard);
    lblDateHeader->setStyleSheet("color: #8C92FF; font-size: 12px; font-weight: bold; letter-spacing: 1.5px; border: none; background: transparent;");

    QHBoxLayout* dateLayout = new QHBoxLayout();
    dateLayout->setSpacing(12);

    dateEdit = new QDateEdit(QDate::currentDate(), dateCard);
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setFixedWidth(185);
    dateEdit->setFixedHeight(40);

    btnCheckDate = new QPushButton("Xem Doanh Thu", dateCard);
    btnCheckDate->setCursor(Qt::PointingHandCursor);
    btnCheckDate->setStyleSheet(Theme::primaryButtonStyle(38, 13));

    lblDateRevenue = new QLabel("Doanh thu: 0 VND", dateCard);
    lblDateRevenue->setStyleSheet("color: #FFD166; font-size: 16px; font-weight: bold; padding-left: 10px; border: none; background: transparent;");

    dateLayout->addWidget(dateEdit);
    dateLayout->addWidget(btnCheckDate);
    dateLayout->addWidget(lblDateRevenue);
    dateLayout->addStretch();

    dateCardLayout->addWidget(lblDateHeader);
    dateCardLayout->addLayout(dateLayout);
    mainLayout->addWidget(dateCard);

    // Card: Revenue by Movie
    QFrame* movieCard = new QFrame(this);
    movieCard->setObjectName("statMovieCard");
    movieCard->setStyleSheet("QFrame#statMovieCard { background-color: #22263C; border: 1px solid #323859; border-radius: 18px; }");
    QVBoxLayout* movieCardLayout = new QVBoxLayout(movieCard);
    movieCardLayout->setContentsMargins(24, 20, 24, 20);
    movieCardLayout->setSpacing(12);

    QLabel* lblMovieHeader = new QLabel("TRA CỨU DOANH THU THEO PHIM", movieCard);
    lblMovieHeader->setStyleSheet("color: #8C92FF; font-size: 12px; font-weight: bold; letter-spacing: 1.5px; border: none; background: transparent;");

    QHBoxLayout* movieLayout = new QHBoxLayout();
    movieLayout->setSpacing(12);

    cbMovies = new QComboBox(movieCard);
    cbMovies->setMinimumWidth(300);
    cbMovies->setFixedHeight(40);

    btnCheckMovie = new QPushButton("Xem Doanh Thu", movieCard);
    btnCheckMovie->setCursor(Qt::PointingHandCursor);
    btnCheckMovie->setStyleSheet(Theme::primaryButtonStyle(38, 13));

    lblMovieRevenue = new QLabel("Doanh thu: 0 VND", movieCard);
    lblMovieRevenue->setStyleSheet("color: #FFD166; font-size: 16px; font-weight: bold; padding-left: 10px; border: none; background: transparent;");

    movieLayout->addWidget(cbMovies);
    movieLayout->addWidget(btnCheckMovie);
    movieLayout->addWidget(lblMovieRevenue);
    movieLayout->addStretch();

    movieCardLayout->addWidget(lblMovieHeader);
    movieCardLayout->addLayout(movieLayout);
    mainLayout->addWidget(movieCard);

    mainLayout->addStretch();

    connect(btnCheckDate, &QPushButton::clicked, this, &StatisticsWidget::checkDateRevenue);
    connect(btnCheckMovie, &QPushButton::clicked, this, &StatisticsWidget::checkMovieRevenue);

    refreshData();
}

void StatisticsWidget::refreshData() {
    loadOverview();

    QString currentSelection = cbMovies->currentData().toString();
    cbMovies->clear();
    auto activeMovies = movieManager->getActiveMovies();
    for (int i = 0; i < activeMovies.size(); ++i) {
        cbMovies->addItem(QString::fromStdString(activeMovies[i].getTenPhim()),
                          QString::fromStdString(activeMovies[i].getMaPhim()));
    }
    int idx = cbMovies->findData(currentSelection);
    if (idx >= 0) cbMovies->setCurrentIndex(idx);
}

void StatisticsWidget::loadOverview() {
    double total = StatisticManager::getTotalRevenue(ticketManager->getAllTickets());
    int count = StatisticManager::countTicketsSold(ticketManager->getAllTickets());
    lblTotalRevenue->setText(QString("%L1 VND").arg(total, 0, 'f', 0));
    lblTotalTickets->setText(QString("%L1 vé").arg(count));
}

void StatisticsWidget::checkDateRevenue() {
    std::string date = dateEdit->date().toString("dd/MM/yyyy").toStdString();
    double rev = StatisticManager::getRevenueByDate(ticketManager->getAllTickets(), date);
    lblDateRevenue->setText(QString("Ngày %1:  %L2 VND").arg(QString::fromStdString(date)).arg(rev, 0, 'f', 0));
}

void StatisticsWidget::checkMovieRevenue() {
    if (cbMovies->currentIndex() >= 0) {
        std::string movieId = cbMovies->currentData().toString().toStdString();
        double rev = StatisticManager::getRevenueByMovie(ticketManager->getAllTickets(), showtimeManager->getAllShowtimes(), movieId);
        lblMovieRevenue->setText(QString("Doanh thu phim:  %L1 VND").arg(rev, 0, 'f', 0));
    }
}
