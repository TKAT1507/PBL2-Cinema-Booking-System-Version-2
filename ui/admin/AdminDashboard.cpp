#include "AdminDashboard.h"
#include "MovieManagementWidget.h"
#include "RoomManagementWidget.h"
#include "ShowtimeManagementWidget.h"
#include "ComboManagementWidget.h"
#include "StatisticsWidget.h"

AdminDashboard::AdminDashboard(const std::string& maQL, const std::string& tenQL,
                               MovieManager* movieMgr, RoomManager* roomMgr,
                               ShowtimeManager* showtimeMgr, TicketManager* ticketMgr,
                               ComboManager* comboMgr, QWidget* parent)
    : QWidget(parent), maQuanLy(maQL), tenQuanLy(tenQL),
      movieManager(movieMgr), roomManager(roomMgr), showtimeManager(showtimeMgr),
      ticketManager(ticketMgr), comboManager(comboMgr) {
    setupUI();
}

void AdminDashboard::setupUI() {
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(12);
    
    // Left Sidebar
    QWidget* sidebar = new QWidget(this);
    sidebar->setStyleSheet("background-color: #121224; border-radius: 10px;");
    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(15, 20, 15, 20);
    sidebarLayout->setSpacing(10);
    
    QLabel* headerLabel = new QLabel(QString("QUAN LY\n%1").arg(QString::fromStdString(tenQuanLy)), this);
    headerLabel->setAlignment(Qt::AlignCenter);
    headerLabel->setStyleSheet("color: #ffd700; font-size: 15px; font-weight: bold; border-bottom: 2px solid #0f3460; padding-bottom: 12px;");
    
    QPushButton* btnMovie = new QPushButton("Quan Ly Phim", this);
    QPushButton* btnRoom = new QPushButton("Quan Ly Phong", this);
    QPushButton* btnShowtime = new QPushButton("Quan Ly Suat Chieu", this);
    QPushButton* btnCombo = new QPushButton("Quan Ly Combo", this);
    QPushButton* btnStat = new QPushButton("Bao Cao Thong Ke", this);
    QPushButton* btnLogout = new QPushButton("Dang Xuat", this);
    btnLogout->setStyleSheet("background-color: #5a0c0e; color: #ffcccc; border: 1px solid #7b1113; font-weight: bold; border-radius: 6px;");
    
    sidebarLayout->addWidget(headerLabel);
    sidebarLayout->addSpacing(10);
    sidebarLayout->addWidget(btnMovie);
    sidebarLayout->addWidget(btnRoom);
    sidebarLayout->addWidget(btnShowtime);
    sidebarLayout->addWidget(btnCombo);
    sidebarLayout->addWidget(btnStat);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(btnLogout);
    
    // Right Content
    stackedWidget = new QStackedWidget(this);
    
    MovieManagementWidget* movieWidget = new MovieManagementWidget(movieManager, this);
    RoomManagementWidget* roomWidget = new RoomManagementWidget(roomManager, this);
    ShowtimeManagementWidget* showtimeWidget = new ShowtimeManagementWidget(showtimeManager, movieManager, roomManager, this);
    ComboManagementWidget* comboWidget = new ComboManagementWidget(comboManager, this);
    StatisticsWidget* statWidget = new StatisticsWidget(ticketManager, showtimeManager, movieManager, this);
    
    stackedWidget->addWidget(movieWidget);
    stackedWidget->addWidget(roomWidget);
    stackedWidget->addWidget(showtimeWidget);
    stackedWidget->addWidget(comboWidget);
    stackedWidget->addWidget(statWidget);
    
    mainLayout->addWidget(sidebar, 1);
    mainLayout->addWidget(stackedWidget, 4);
    
    // Connections
    connect(btnMovie, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(0); });
    connect(btnRoom, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(1); });
    connect(btnShowtime, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(2); });
    connect(btnCombo, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(3); });
    connect(btnStat, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(4); });
    connect(btnLogout, &QPushButton::clicked, this, &AdminDashboard::logout);
}
