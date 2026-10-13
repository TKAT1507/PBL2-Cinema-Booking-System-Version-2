#include "CashierDashboard.h"
#include "SaleWidget.h"
#include "CheckTicketWidget.h"

CashierDashboard::CashierDashboard(const std::string& maNhanVien, const std::string& tenNhanVien,
                                   MovieManager* movieMgr, RoomManager* roomMgr,
                                   ShowtimeManager* showtimeMgr, TicketManager* ticketMgr,
                                   ComboManager* comboMgr, QWidget* parent)
    : QWidget(parent), maNhanVien(maNhanVien), tenNhanVien(tenNhanVien),
      movieManager(movieMgr), roomManager(roomMgr), showtimeManager(showtimeMgr),
      ticketManager(ticketMgr), comboManager(comboMgr) {
    setupUI();
}

void CashierDashboard::setupUI() {
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(12);
    
    // Left Sidebar
    QWidget* sidebar = new QWidget(this);
    sidebar->setStyleSheet("background-color: #121224; border-radius: 10px;");
    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(15, 20, 15, 20);
    sidebarLayout->setSpacing(12);
    
    QLabel* headerLabel = new QLabel(QString("THU NGAN\n%1").arg(QString::fromStdString(tenNhanVien)), this);
    headerLabel->setAlignment(Qt::AlignCenter);
    headerLabel->setStyleSheet("color: #ffd700; font-size: 15px; font-weight: bold; border-bottom: 2px solid #0f3460; padding-bottom: 12px;");
    
    QPushButton* btnBanVe = new QPushButton("Ban Ve Tai Rap", this);
    QPushButton* btnKiemTraVe = new QPushButton("Kiem Tra & Soat Ve", this);
    QPushButton* btnDangXuat = new QPushButton("Dang Xuat", this);
    btnDangXuat->setStyleSheet("background-color: #5a0c0e; color: #ffcccc; border: 1px solid #7b1113; font-weight: bold; border-radius: 6px;");
    
    sidebarLayout->addWidget(headerLabel);
    sidebarLayout->addSpacing(10);
    sidebarLayout->addWidget(btnBanVe);
    sidebarLayout->addWidget(btnKiemTraVe);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(btnDangXuat);
    
    // Right Content
    stackedWidget = new QStackedWidget(this);
    SaleWidget* saleWidget = new SaleWidget(maNhanVien, movieManager, roomManager, showtimeManager, ticketManager, comboManager, this);
    CheckTicketWidget* checkTicketWidget = new CheckTicketWidget(ticketManager, showtimeManager, movieManager, this);
    
    stackedWidget->addWidget(saleWidget);
    stackedWidget->addWidget(checkTicketWidget);
    
    mainLayout->addWidget(sidebar, 1);
    mainLayout->addWidget(stackedWidget, 4);
    
    // Connections
    connect(btnBanVe, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(0); });
    connect(btnKiemTraVe, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(1); });
    connect(btnDangXuat, &QPushButton::clicked, this, &CashierDashboard::logout);
}
