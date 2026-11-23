#include "AdminDashboard.h"
#include "MovieManagementWidget.h"
#include "RoomManagementWidget.h"
#include "ShowtimeManagementWidget.h"
#include "ComboManagementWidget.h"
#include "StatisticsWidget.h"
#include "../common/ChangePasswordDialog.h"
#include "../common/Theme.h"
#include "managers/UserManager.h"
#include <QFrame>

AdminDashboard::AdminDashboard(const std::string& maQL, const std::string& tenQL,
                               UserManager* userMgr, MovieManager* movieMgr, RoomManager* roomMgr,
                               ShowtimeManager* showtimeMgr, TicketManager* ticketMgr,
                               ComboManager* comboMgr, QWidget* parent)
    : QWidget(parent), maQuanLy(maQL), tenQuanLy(tenQL),
      userManager(userMgr), movieManager(movieMgr), roomManager(roomMgr),
      showtimeManager(showtimeMgr), ticketManager(ticketMgr), comboManager(comboMgr) {
    setupUI();
}

void AdminDashboard::setupUI() {
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
    sidebarLayout->setSpacing(8);
    sidebarLayout->setAlignment(Qt::AlignTop);

    QLabel* lblBrandMain = new QLabel("PBL2 CINEMA", sidebar);
    lblBrandMain->setAlignment(Qt::AlignCenter);
    lblBrandMain->setStyleSheet("color: #F3F4FA; font-size: 19px; font-weight: bold; letter-spacing: 2px; background: transparent;");

    QLabel* lblBrandSub = new QLabel("STUDIO MANAGEMENT", sidebar);
    lblBrandSub->setAlignment(Qt::AlignCenter);
    lblBrandSub->setStyleSheet("color: #FF7675; font-size: 10px; font-weight: bold; letter-spacing: 2px; background: transparent;");

    QFrame* brandDivider = new QFrame(sidebar);
    brandDivider->setFrameShape(QFrame::HLine);
    brandDivider->setStyleSheet("background-color: #2D3252; max-height: 1px; border: none;");

    sidebarLayout->addWidget(lblBrandMain);
    sidebarLayout->addWidget(lblBrandSub);
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

    QLabel* lblRole = new QLabel("QUẢN LÝ RẠP PHIM", headerFrame);
    lblRole->setStyleSheet("color: #FFD166; font-size: 10px; font-weight: bold; letter-spacing: 1px; background: transparent; border: none;");

    QLabel* lblName = new QLabel(QString::fromStdString(tenQuanLy), headerFrame);
    lblName->setWordWrap(true);
    lblName->setStyleSheet("color: #F3F4FA; font-size: 14px; font-weight: bold; background: transparent; border: none;");

    headerLayout->addWidget(lblRole);
    headerLayout->addWidget(lblName);
    sidebarLayout->addWidget(headerFrame);
    sidebarLayout->addSpacing(12);

    QLabel* lblMenu = new QLabel("QUẢN TRỊ HỆ THỐNG", sidebar);
    lblMenu->setStyleSheet("color: #707899; font-size: 10px; font-weight: bold; letter-spacing: 2px; padding-left: 8px; background: transparent;");
    sidebarLayout->addWidget(lblMenu);

    QPushButton* btnMovie = new QPushButton("  Quản Lý Phim", sidebar);
    btnMovie->setCursor(Qt::PointingHandCursor);

    QPushButton* btnRoom = new QPushButton("  Quản Lý Phòng", sidebar);
    btnRoom->setCursor(Qt::PointingHandCursor);

    QPushButton* btnShowtime = new QPushButton("  Quản Lý Suất Chiếu", sidebar);
    btnShowtime->setCursor(Qt::PointingHandCursor);

    QPushButton* btnCombo = new QPushButton("  Quản Lý Combo", sidebar);
    btnCombo->setCursor(Qt::PointingHandCursor);

    QPushButton* btnStat = new QPushButton("  Báo Cáo Thống Kê", sidebar);
    btnStat->setCursor(Qt::PointingHandCursor);

    auto setActiveTab = [btnMovie, btnRoom, btnShowtime, btnCombo, btnStat](int index) {
        btnMovie->setStyleSheet(Theme::sidebarNavButtonStyle(index == 0));
        btnRoom->setStyleSheet(Theme::sidebarNavButtonStyle(index == 1));
        btnShowtime->setStyleSheet(Theme::sidebarNavButtonStyle(index == 2));
        btnCombo->setStyleSheet(Theme::sidebarNavButtonStyle(index == 3));
        btnStat->setStyleSheet(Theme::sidebarNavButtonStyle(index == 4));
    };
    setActiveTab(0);

    sidebarLayout->addWidget(btnMovie);
    sidebarLayout->addWidget(btnRoom);
    sidebarLayout->addWidget(btnShowtime);
    sidebarLayout->addWidget(btnCombo);
    sidebarLayout->addWidget(btnStat);
    sidebarLayout->addStretch();

    QFrame* bottomDivider = new QFrame(sidebar);
    bottomDivider->setFrameShape(QFrame::HLine);
    bottomDivider->setStyleSheet("background-color: #2D3252; max-height: 1px; border: none;");
    sidebarLayout->addWidget(bottomDivider);
    sidebarLayout->addSpacing(6);

    QPushButton* btnChangePassword = new QPushButton("Đổi Mật Khẩu", sidebar);
    btnChangePassword->setCursor(Qt::PointingHandCursor);
    btnChangePassword->setStyleSheet(Theme::secondaryButtonStyle(40, 13));

    QPushButton* btnLogout = new QPushButton("Đăng Xuất", sidebar);
    btnLogout->setCursor(Qt::PointingHandCursor);
    btnLogout->setStyleSheet(Theme::dangerOutlineButtonStyle(40, 13));

    sidebarLayout->addWidget(btnChangePassword);
    sidebarLayout->addWidget(btnLogout);

    // Right Content
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setStyleSheet("QStackedWidget { background-color: #161827; }");

    MovieManagementWidget* movieWidget = new MovieManagementWidget(movieManager, this);
    RoomManagementWidget* roomWidget = new RoomManagementWidget(roomManager, this);
    ShowtimeManagementWidget* showtimeWidget = new ShowtimeManagementWidget(showtimeManager, movieManager, roomManager, this);
    ComboManagementWidget* comboWidget = new ComboManagementWidget(comboManager, this);
    StatisticsWidget* statWidget = new StatisticsWidget(ticketManager, showtimeManager, movieManager, this);

    stackedWidget->addWidget(movieWidget);    // 0
    stackedWidget->addWidget(roomWidget);     // 1
    stackedWidget->addWidget(showtimeWidget); // 2
    stackedWidget->addWidget(comboWidget);    // 3
    stackedWidget->addWidget(statWidget);     // 4

    mainLayout->addWidget(sidebar, 0);
    mainLayout->addWidget(stackedWidget, 1);

    connect(btnMovie, &QPushButton::clicked, this, [this, setActiveTab]() {
        setActiveTab(0);
        stackedWidget->setCurrentIndex(0);
    });
    connect(btnRoom, &QPushButton::clicked, this, [this, setActiveTab]() {
        setActiveTab(1);
        stackedWidget->setCurrentIndex(1);
    });
    connect(btnShowtime, &QPushButton::clicked, this, [this, setActiveTab]() {
        setActiveTab(2);
        stackedWidget->setCurrentIndex(2);
    });
    connect(btnCombo, &QPushButton::clicked, this, [this, setActiveTab]() {
        setActiveTab(3);
        stackedWidget->setCurrentIndex(3);
    });
    connect(btnStat, &QPushButton::clicked, this, [this, setActiveTab, statWidget]() {
        setActiveTab(4);
        statWidget->refreshData();
        stackedWidget->setCurrentIndex(4);
    });

    connect(btnChangePassword, &QPushButton::clicked, this, [this]() {
        std::string taiKhoan = maQuanLy;
        if (userManager) {
            Admin* a = userManager->findAdminById(maQuanLy);
            if (a) taiKhoan = a->getTaiKhoan();
        }
        ChangePasswordDialog dlg(userManager, taiKhoan, this);
        dlg.exec();
    });

    connect(btnLogout, &QPushButton::clicked, this, &AdminDashboard::logout);
}
