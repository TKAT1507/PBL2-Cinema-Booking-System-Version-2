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
    mainLayout->setContentsMargins(0, 0, 14, 0);
    mainLayout->setSpacing(14);
    
    // Left Sidebar: Dính sát viền trái, vuông vức (border-radius: 0px), màu vàng mù tạt #EAB02F
    QWidget* sidebar = new QWidget(this);
    sidebar->setFixedWidth(240);
    sidebar->setStyleSheet(R"(
        QWidget {
            background-color: #EAB02F;
            border: none;
            border-radius: 0px;
        }
    )");

    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(12, 16, 12, 16);
    sidebarLayout->setSpacing(10);
    sidebarLayout->setAlignment(Qt::AlignTop);
    
    // Top Greeting Frame: Bo tròn như cũ, màu đỏ gạch #A52C20
    QFrame* headerFrame = new QFrame(sidebar);
    headerFrame->setStyleSheet(R"(
        QFrame {
            background-color: #A52C20;
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
    
    // Navigation Tabs: Bo tròn như cũ, màu kem #F9F1E5
    QString tabBtnStyle = R"(
        QPushButton {
            background-color: #F9F1E5;
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
            background-color: #FFFFFF;
            border-color: #A52C20;
        }
        QPushButton:pressed {
            background-color: #EAB02F;
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
    
    // Bottom Buttons: Bo tròn như cũ
    QPushButton* btnChangePassword = new QPushButton("Đổi Mật Khẩu", sidebar);
    btnChangePassword->setCursor(Qt::PointingHandCursor);
    btnChangePassword->setStyleSheet(R"(
        QPushButton {
            background-color: #F9F1E5;
            color: #A52C20;
            border: 1px solid #A52C20;
            border-radius: 6px;
            font-size: 13px;
            font-weight: bold;
            min-height: 38px;
        }
        QPushButton:hover {
            background-color: #A52C20;
            color: #FFFFFF;
        }
    )");

    QPushButton* btnDangXuat = new QPushButton("Đăng Xuất", sidebar);
    btnDangXuat->setCursor(Qt::PointingHandCursor);
    btnDangXuat->setStyleSheet(R"(
        QPushButton {
            background-color: #A52C20;
            color: #FFFFFF;
            border: 1px solid #822017;
            border-radius: 6px;
            font-size: 14px;
            font-weight: bold;
            min-height: 38px;
        }
        QPushButton:hover {
            background-color: #BD362A;
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
