#include "AdminDashboard.h"
#include "MovieManagementWidget.h"
#include "RoomManagementWidget.h"
#include "ShowtimeManagementWidget.h"
#include "ComboManagementWidget.h"
#include "StatisticsWidget.h"
#include "../common/ChangePasswordDialog.h"
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

    QLabel* lblName = new QLabel("Quản lý\n" + QString::fromStdString(tenQuanLy), headerFrame);
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
            font-size: 13px;
            font-weight: bold;
            min-height: 38px;
            padding: 5px 10px;
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

    QPushButton* btnMovie = new QPushButton("Quản Lý Phim", sidebar);
    btnMovie->setCursor(Qt::PointingHandCursor);
    btnMovie->setStyleSheet(tabBtnStyle);

    QPushButton* btnRoom = new QPushButton("Quản Lý Phòng", sidebar);
    btnRoom->setCursor(Qt::PointingHandCursor);
    btnRoom->setStyleSheet(tabBtnStyle);

    QPushButton* btnShowtime = new QPushButton("Quản Lý Suất Chiếu", sidebar);
    btnShowtime->setCursor(Qt::PointingHandCursor);
    btnShowtime->setStyleSheet(tabBtnStyle);

    QPushButton* btnCombo = new QPushButton("Quản Lý Combo", sidebar);
    btnCombo->setCursor(Qt::PointingHandCursor);
    btnCombo->setStyleSheet(tabBtnStyle);

    QPushButton* btnStat = new QPushButton("Báo Cáo Thống Kê", sidebar);
    btnStat->setCursor(Qt::PointingHandCursor);
    btnStat->setStyleSheet(tabBtnStyle);

    sidebarLayout->addWidget(btnMovie);
    sidebarLayout->addWidget(btnRoom);
    sidebarLayout->addWidget(btnShowtime);
    sidebarLayout->addWidget(btnCombo);
    sidebarLayout->addWidget(btnStat);
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

    QPushButton* btnLogout = new QPushButton("Đăng Xuất", sidebar);
    btnLogout->setCursor(Qt::PointingHandCursor);
    btnLogout->setStyleSheet(R"(
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
    sidebarLayout->addWidget(btnLogout);
    
    // Right Content
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setStyleSheet("QStackedWidget { background: transparent; }");

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
    
    // Connections
    connect(btnMovie, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(0); });
    connect(btnRoom, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(1); });
    connect(btnShowtime, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(2); });
    connect(btnCombo, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(3); });
    connect(btnStat, &QPushButton::clicked, [this]() { stackedWidget->setCurrentIndex(4); });

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
