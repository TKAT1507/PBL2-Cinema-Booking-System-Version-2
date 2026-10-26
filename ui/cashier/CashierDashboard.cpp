#include "CashierDashboard.h"
#include "SaleWidget.h"
#include "CheckTicketWidget.h"
#include "TicketApprovalWidget.h"
#include "../common/ChangePasswordDialog.h"
#include "managers/UserManager.h"
#include <QFrame>

CashierDashboard::CashierDashboard(const std::string& maNhanVien, const std::string& tenNhanVien,
                                   UserManager* userMgr, MovieManager* movieMgr, RoomManager* roomMgr,
                                   ShowtimeManager* showtimeMgr, TicketManager* ticketMgr,
                                   ComboManager* comboMgr, QWidget* parent)
    : QWidget(parent), maNhanVien(maNhanVien), tenNhanVien(tenNhanVien),
      userManager(userMgr), movieManager(movieMgr), roomManager(roomMgr),
      showtimeManager(showtimeMgr), ticketManager(ticketMgr), comboManager(comboMgr) {
    setupUI();
}

void CashierDashboard::setupUI() {
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(14);
    
    // Left Sidebar: Gold #E6B042, rounded
    QWidget* sidebar = new QWidget(this);
    sidebar->setFixedWidth(230);
    sidebar->setStyleSheet(R"(
        QWidget {
            background-color: #E6B042;
            border-radius: 10px;
        }
    )");

    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(12, 16, 12, 16);
    sidebarLayout->setSpacing(10);
    sidebarLayout->setAlignment(Qt::AlignTop);
    
    // Top Greeting Frame
    QFrame* headerFrame = new QFrame(sidebar);
    headerFrame->setStyleSheet(R"(
        QFrame {
            background-color: #9E221E;
            border-radius: 8px;
            padding: 8px;
        }
    )");
    QVBoxLayout* headerLayout = new QVBoxLayout(headerFrame);
    headerLayout->setContentsMargins(5, 8, 5, 8);
    headerLayout->setSpacing(4);

    QLabel* lblGreeting = new QLabel("Xin chào", headerFrame);
    lblGreeting->setAlignment(Qt::AlignCenter);
    lblGreeting->setStyleSheet("color: #FFFFFF; font-size: 13px; background: transparent; border: none;");

    QLabel* lblName = new QLabel("Thu ngân\n" + QString::fromStdString(tenNhanVien), headerFrame);
    lblName->setAlignment(Qt::AlignCenter);
    lblName->setStyleSheet("color: #FFFFFF; font-size: 15px; font-weight: bold; background: transparent; border: none;");

    headerLayout->addWidget(lblGreeting);
    headerLayout->addWidget(lblName);
    sidebarLayout->addWidget(headerFrame);
    sidebarLayout->addSpacing(15);
    
    // Navigation Tabs
    QString tabBtnStyle = R"(
        QPushButton {
            background-color: #F4DFB6;
            color: #1A1A1A;
            border: 1px solid #2B1810;
            border-radius: 6px;
            font-size: 14px;
            font-weight: bold;
            min-height: 40px;
            padding: 6px 12px;
            text-align: center;
        }
        QPushButton:hover {
            background-color: #FFF2D6;
            border-color: #9E221E;
        }
        QPushButton:pressed {
            background-color: #D4A034;
        }
    )";

    QPushButton* btnBanVe = new QPushButton("Bán Vé Tại Rạp", sidebar);
    btnBanVe->setCursor(Qt::PointingHandCursor);
    btnBanVe->setStyleSheet(tabBtnStyle);

    QPushButton* btnDuyetVe = new QPushButton("Xác Nhận Vé ⭐", sidebar);
    btnDuyetVe->setCursor(Qt::PointingHandCursor);
    btnDuyetVe->setStyleSheet(tabBtnStyle);

    QPushButton* btnKiemTraVe = new QPushButton("Kiểm Tra & Soát Vé", sidebar);
    btnKiemTraVe->setCursor(Qt::PointingHandCursor);
    btnKiemTraVe->setStyleSheet(tabBtnStyle);

    sidebarLayout->addWidget(btnBanVe);
    sidebarLayout->addWidget(btnDuyetVe);
    sidebarLayout->addWidget(btnKiemTraVe);
    sidebarLayout->addStretch();
    
    // Bottom Buttons
    QPushButton* btnChangePassword = new QPushButton("Đổi Mật Khẩu", sidebar);
    btnChangePassword->setCursor(Qt::PointingHandCursor);
    btnChangePassword->setStyleSheet(R"(
        QPushButton {
            background-color: #F4DFB6;
            color: #9E221E;
            border: 1px solid #9E221E;
            border-radius: 6px;
            font-size: 13px;
            font-weight: bold;
            min-height: 38px;
        }
        QPushButton:hover {
            background-color: #9E221E;
            color: #FFFFFF;
        }
    )");

    QPushButton* btnDangXuat = new QPushButton("Đăng Xuất", sidebar);
    btnDangXuat->setCursor(Qt::PointingHandCursor);
    btnDangXuat->setStyleSheet(R"(
        QPushButton {
            background-color: #9E221E;
            color: #FFFFFF;
            border: 1px solid #781A17;
            border-radius: 6px;
            font-size: 14px;
            font-weight: bold;
            min-height: 38px;
        }
        QPushButton:hover {
            background-color: #B52A26;
        }
    )");
    
    sidebarLayout->addWidget(btnChangePassword);
    sidebarLayout->addWidget(btnDangXuat);
    
    // Right Content Area
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setStyleSheet("QStackedWidget { background: transparent; }");

    SaleWidget* saleWidget = new SaleWidget(maNhanVien, movieManager, roomManager, showtimeManager, ticketManager, comboManager, this);
    ticketApprovalWidget = new TicketApprovalWidget(ticketManager, showtimeManager, movieManager, this);
    CheckTicketWidget* checkTicketWidget = new CheckTicketWidget(ticketManager, showtimeManager, movieManager, this);
    
    stackedWidget->addWidget(saleWidget);           // 0
    stackedWidget->addWidget(ticketApprovalWidget);  // 1
    stackedWidget->addWidget(checkTicketWidget);     // 2
    
    mainLayout->addWidget(sidebar, 0);
    mainLayout->addWidget(stackedWidget, 1);
    
    // Tab Navigation
    connect(btnBanVe, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(0); });
    connect(btnDuyetVe, &QPushButton::clicked, [this]() { 
        ticketApprovalWidget->loadPendingOrders();
        stackedWidget->setCurrentIndex(1); 
    });
    connect(btnKiemTraVe, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(2); });
    
    connect(btnChangePassword, &QPushButton::clicked, this, [this]() {
        std::string taiKhoan = maNhanVien;
        if (userManager) {
            Cashier* c = userManager->findCashierById(maNhanVien);
            if (c) taiKhoan = c->getTaiKhoan();
        }
        ChangePasswordDialog dlg(userManager, taiKhoan, this);
        dlg.exec();
    });

    connect(btnDangXuat, &QPushButton::clicked, this, &CashierDashboard::logout);
}
