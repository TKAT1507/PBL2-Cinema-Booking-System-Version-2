#include "MainWindow.h"
#include "../login/LoginWindow.h"
#include "../login/RegisterWindow.h"
#include "../login/ChangePasswordWindow.h"
#include "../customer/CustomerDashboard.h"
#include "../cashier/CashierDashboard.h"
#include "../admin/AdminDashboard.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      userManager(nullptr), movieManager(nullptr), roomManager(nullptr),
      showtimeManager(nullptr), ticketManager(nullptr), comboManager(nullptr),
      loginWindow(nullptr), registerWindow(nullptr), changePasswordWindow(nullptr),
      customerDashboard(nullptr), cashierDashboard(nullptr), adminDashboard(nullptr)
{
    setWindowTitle("PBL2 - He Thong Dat Ve Xem Phim");
    setMinimumSize(1200, 800);

    // Global Stylesheet — Cinema theme
    setStyleSheet(R"(
        QMainWindow {
            background-color: #1a1a2e;
        }
        QWidget {
            font-family: "Segoe UI", Arial, sans-serif;
            font-size: 13px;
            color: #e0e0e0;
        }
        QPushButton {
            background-color: #16213e;
            color: #e0e0e0;
            border: 1px solid #0f3460;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: bold;
            min-height: 30px;
        }
        QPushButton:hover {
            background-color: #0f3460;
            border-color: #e94560;
        }
        QPushButton:pressed {
            background-color: #e94560;
            color: white;
        }
        QPushButton:disabled {
            background-color: #333;
            color: #777;
        }
        QTableWidget {
            background-color: #16213e;
            alternate-background-color: #1a1a2e;
            gridline-color: #0f3460;
            border: 1px solid #0f3460;
            border-radius: 4px;
            selection-background-color: #e94560;
            color: #e0e0e0;
        }
        QTableWidget::item {
            padding: 4px 8px;
        }
        QHeaderView::section {
            background-color: #0f3460;
            color: #ffd700;
            padding: 6px;
            border: 1px solid #16213e;
            font-weight: bold;
        }
        QLineEdit, QSpinBox, QDoubleSpinBox, QComboBox, QDateEdit, QTimeEdit {
            background-color: #16213e;
            color: #e0e0e0;
            border: 1px solid #0f3460;
            border-radius: 4px;
            padding: 6px;
            min-height: 24px;
        }
        QLineEdit:focus, QSpinBox:focus, QComboBox:focus {
            border-color: #e94560;
        }
        QLabel {
            color: #e0e0e0;
        }
        QGroupBox {
            border: 1px solid #0f3460;
            border-radius: 6px;
            margin-top: 10px;
            padding-top: 15px;
            color: #ffd700;
            font-weight: bold;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
        }
        QMessageBox {
            background-color: #1a1a2e;
        }
        QDialog {
            background-color: #1a1a2e;
        }
        QScrollBar:vertical {
            background-color: #16213e;
            width: 12px;
        }
        QScrollBar::handle:vertical {
            background-color: #0f3460;
            border-radius: 6px;
        }
    )");

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    initManagers();
    showLogin();
}

MainWindow::~MainWindow() {
    delete userManager;
    delete movieManager;
    delete roomManager;
    delete showtimeManager;
    delete ticketManager;
    delete comboManager;
}

void MainWindow::initManagers() {
    userManager = new UserManager();
    movieManager = new MovieManager();
    roomManager = new RoomManager();
    showtimeManager = new ShowtimeManager();
    ticketManager = new TicketManager();
    comboManager = new ComboManager();

    userManager->loadAll();
    movieManager->load();
    roomManager->load();
    showtimeManager->load();
    showtimeManager->loadSeatMaps();
    ticketManager->load();
    comboManager->load();
}

void MainWindow::showLogin() {
    if (!loginWindow) {
        loginWindow = new LoginWindow(userManager);
        connect(loginWindow, &LoginWindow::loginSuccess, this, [this](const QString& role, const QString& userId) {
            if (role == "CUSTOMER") showCustomerDashboard(userId);
            else if (role == "CASHIER") showCashierDashboard(userId);
            else if (role == "ADMIN") showAdminDashboard(userId);
        });
        connect(loginWindow, &LoginWindow::showRegister, this, &MainWindow::showRegister);
        connect(loginWindow, &LoginWindow::showChangePassword, this, &MainWindow::showChangePassword);
        stackedWidget->addWidget(loginWindow);
    }
    stackedWidget->setCurrentWidget(loginWindow);
}

void MainWindow::showRegister() {
    if (!registerWindow) {
        registerWindow = new RegisterWindow(userManager);
        connect(registerWindow, &RegisterWindow::goBack, this, &MainWindow::showLogin);
        connect(registerWindow, &RegisterWindow::registerSuccess, this, &MainWindow::showLogin);
        stackedWidget->addWidget(registerWindow);
    }
    stackedWidget->setCurrentWidget(registerWindow);
}

void MainWindow::showChangePassword() {
    if (!changePasswordWindow) {
        changePasswordWindow = new ChangePasswordWindow(userManager);
        connect(changePasswordWindow, &ChangePasswordWindow::goBack, this, &MainWindow::showLogin);
        stackedWidget->addWidget(changePasswordWindow);
    }
    stackedWidget->setCurrentWidget(changePasswordWindow);
}

void MainWindow::showCustomerDashboard(const QString& maKH) {
    if (customerDashboard) {
        stackedWidget->removeWidget(customerDashboard);
        delete customerDashboard;
        customerDashboard = nullptr;
    }
    customerDashboard = new CustomerDashboard(maKH, userManager, movieManager, showtimeManager, ticketManager, comboManager, roomManager);
    connect(customerDashboard, &CustomerDashboard::logout, this, &MainWindow::logout);
    stackedWidget->addWidget(customerDashboard);
    stackedWidget->setCurrentWidget(customerDashboard);
}

void MainWindow::showCashierDashboard(const QString& maNV) {
    if (cashierDashboard) {
        stackedWidget->removeWidget(cashierDashboard);
        delete cashierDashboard;
        cashierDashboard = nullptr;
    }
    // Lookup cashier name
    std::string id = maNV.toStdString();
    std::string tenNV = "Thu Ngan";
    Cashier* c = userManager->findCashierById(id);
    if (c) tenNV = c->getTenNhanVien();
    
    cashierDashboard = new CashierDashboard(id, tenNV, movieManager, roomManager, showtimeManager, ticketManager, comboManager);
    connect(cashierDashboard, &CashierDashboard::logout, this, &MainWindow::logout);
    stackedWidget->addWidget(cashierDashboard);
    stackedWidget->setCurrentWidget(cashierDashboard);
}

void MainWindow::showAdminDashboard(const QString& maQL) {
    if (adminDashboard) {
        stackedWidget->removeWidget(adminDashboard);
        delete adminDashboard;
        adminDashboard = nullptr;
    }
    // Lookup admin name
    std::string id = maQL.toStdString();
    std::string tenQL = "Quan Ly";
    Admin* a = userManager->findAdminById(id);
    if (a) tenQL = a->getTenQuanLy();
    
    adminDashboard = new AdminDashboard(id, tenQL, movieManager, roomManager, showtimeManager, ticketManager, comboManager);
    connect(adminDashboard, &AdminDashboard::logout, this, &MainWindow::logout);
    stackedWidget->addWidget(adminDashboard);
    stackedWidget->setCurrentWidget(adminDashboard);
}

void MainWindow::logout() {
    if (customerDashboard) {
        stackedWidget->removeWidget(customerDashboard);
        delete customerDashboard;
        customerDashboard = nullptr;
    }
    if (cashierDashboard) {
        stackedWidget->removeWidget(cashierDashboard);
        delete cashierDashboard;
        cashierDashboard = nullptr;
    }
    if (adminDashboard) {
        stackedWidget->removeWidget(adminDashboard);
        delete adminDashboard;
        adminDashboard = nullptr;
    }
    showLogin();
}
