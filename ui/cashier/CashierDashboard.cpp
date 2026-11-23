#include "CashierDashboard.h"
#include "SaleWidget.h"
#include "CheckTicketWidget.h"
#include "TicketApprovalWidget.h"
#include "../common/ChangePasswordDialog.h"
#include "../common/Theme.h"
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
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Left Sidebar — Soft Midnight Twilight
    QWidget* sidebar = new QWidget(this);
    sidebar->setObjectName("sidebarContainer");
    sidebar->setAttribute(Qt::WA_StyledBackground, true);
    sidebar->setFixedWidth(252);
    sidebar->setStyleSheet(Theme::sidebarContainerStyle());

    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(18, 24, 18, 20);
    sidebarLayout->setSpacing(10);
    sidebarLayout->setAlignment(Qt::AlignTop);

    QLabel* lblBrandMain = new QLabel("PBL2 CINEMA", sidebar);
    lblBrandMain->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    lblBrandMain->setStyleSheet("color: #1E293B; font-size: 16px; font-weight: 600; background: transparent; padding-left: 6px;");


    QFrame* brandDivider = new QFrame(sidebar);
    brandDivider->setFrameShape(QFrame::HLine);
    brandDivider->setStyleSheet("background-color: #E5E7EB; max-height: 1px; border: none;");

    sidebarLayout->addWidget(lblBrandMain);
    sidebarLayout->addSpacing(6);
    sidebarLayout->addWidget(brandDivider);
    sidebarLayout->addSpacing(6);

    // Profile Card
    QFrame* headerFrame = new QFrame(sidebar);
    headerFrame->setObjectName("profileCard");
    headerFrame->setStyleSheet(Theme::sidebarProfileCardStyle());

    QVBoxLayout* headerLayout = new QVBoxLayout(headerFrame);
    headerLayout->setContentsMargins(14, 12, 14, 12);
    headerLayout->setSpacing(3);

    QLabel* lblRole = new QLabel("Thu ngân", headerFrame);
    lblRole->setStyleSheet("color: #D97706; font-size: 10px; font-weight: 600; background: transparent; border: none;");

    QLabel* lblName = new QLabel(QString::fromStdString(tenNhanVien), headerFrame);
    lblName->setWordWrap(true);
    lblName->setStyleSheet("color: #1E293B; font-size: 14px; font-weight: 600; background: transparent; border: none;");

    headerLayout->addWidget(lblRole);
    headerLayout->addWidget(lblName);
    sidebarLayout->addWidget(headerFrame);
    sidebarLayout->addSpacing(12);


    QPushButton* btnBanVe = new QPushButton("  Bán Vé Tại Rạp", sidebar);
    btnBanVe->setCursor(Qt::PointingHandCursor);

    QPushButton* btnDuyetVe = new QPushButton("  Xác Nhận Vé", sidebar);
    btnDuyetVe->setCursor(Qt::PointingHandCursor);

    QPushButton* btnKiemTraVe = new QPushButton("  Kiểm Tra && Soát Vé", sidebar);
    btnKiemTraVe->setCursor(Qt::PointingHandCursor);

    auto setActiveTab = [btnBanVe, btnDuyetVe, btnKiemTraVe](int index) {
        btnBanVe->setStyleSheet(Theme::sidebarNavButtonStyle(index == 0));
        btnDuyetVe->setStyleSheet(Theme::sidebarNavButtonStyle(index == 1));
        btnKiemTraVe->setStyleSheet(Theme::sidebarNavButtonStyle(index == 2));
    };
    setActiveTab(0);

    sidebarLayout->addWidget(btnBanVe);
    sidebarLayout->addWidget(btnDuyetVe);
    sidebarLayout->addWidget(btnKiemTraVe);
    sidebarLayout->addStretch();

    QFrame* bottomDivider = new QFrame(sidebar);
    bottomDivider->setFrameShape(QFrame::HLine);
    bottomDivider->setStyleSheet("background-color: #E5E7EB; max-height: 1px; border: none;");
    sidebarLayout->addWidget(bottomDivider);
    sidebarLayout->addSpacing(6);

    QPushButton* btnChangePassword = new QPushButton("Đổi Mật Khẩu", sidebar);
    btnChangePassword->setCursor(Qt::PointingHandCursor);
    btnChangePassword->setStyleSheet(Theme::secondaryButtonStyle(40, 13));

    QPushButton* btnDangXuat = new QPushButton("Đăng Xuất", sidebar);
    btnDangXuat->setCursor(Qt::PointingHandCursor);
    btnDangXuat->setStyleSheet(Theme::dangerOutlineButtonStyle(40, 13));

    sidebarLayout->addWidget(btnChangePassword);
    sidebarLayout->addWidget(btnDangXuat);

    // Right Content Area
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setStyleSheet("QStackedWidget { background-color: #F5F6F8; }");

    SaleWidget* saleWidget = new SaleWidget(maNhanVien, movieManager, roomManager, showtimeManager, ticketManager, comboManager, this);
    ticketApprovalWidget = new TicketApprovalWidget(ticketManager, showtimeManager, movieManager, this);
    CheckTicketWidget* checkTicketWidget = new CheckTicketWidget(ticketManager, showtimeManager, movieManager, this);

    stackedWidget->addWidget(saleWidget);            // 0
    stackedWidget->addWidget(ticketApprovalWidget);  // 1
    stackedWidget->addWidget(checkTicketWidget);     // 2

    mainLayout->addWidget(sidebar, 0);
    mainLayout->addWidget(stackedWidget, 1);

    connect(btnBanVe, &QPushButton::clicked, this, [this, setActiveTab]() {
        setActiveTab(0);
        stackedWidget->setCurrentIndex(0);
    });
    connect(btnDuyetVe, &QPushButton::clicked, this, [this, setActiveTab]() {
        setActiveTab(1);
        ticketApprovalWidget->loadPendingOrders();
        stackedWidget->setCurrentIndex(1);
    });
    connect(btnKiemTraVe, &QPushButton::clicked, this, [this, setActiveTab]() {
        setActiveTab(2);
        stackedWidget->setCurrentIndex(2);
    });

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
