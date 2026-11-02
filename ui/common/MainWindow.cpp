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

    // Global Stylesheet — He mau chuan: #1f224c, #87271f, #EAB02F, #F9F1E5
    setStyleSheet(R"(
        QMainWindow {
            background-color: #111729;
        }
        QWidget {
            font-family: "Segoe UI", Arial, sans-serif;
            font-size: 13px;
            color: #FFFFFF;
        }
        /* Default buttons */
        QPushButton {
            background-color: #a32419;
            color: #FFFFFF;
            border: 1px solid #7A1F16;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: bold;
            min-height: 32px;
        }
        QPushButton:hover {
            background-color: #BD362A;
            border-color: #EAB02F;
        }
        QPushButton:pressed {
            background-color: #822017;
        }
        QPushButton:disabled {
            background-color: #3E4166;
            color: #8E91A8;
            border-color: #313455;
        }
        /* Tables */
        QTableWidget {
            background-color: #1f224c;
            alternate-background-color: #101224;
            gridline-color: #2c2f5e;
            border: 1px solid #434994;
            border-radius: 6px;
            selection-background-color: #87271f;
            selection-color: #FFFFFF;
            color: #FFFFFF;
        }
        QTableWidget::item {
            padding: 6px 10px;
            border-bottom: 1px solid #383D7C;
        }
        QHeaderView::section {
            background-color: #87271f;
            color: #FFFFFF;
            padding: 8px 10px;
            border: none;
            font-weight: bold;
            font-size: 13px;
        }
        /* Input fields */
        QLineEdit, QSpinBox, QDoubleSpinBox, QComboBox, QDateEdit, QTimeEdit {
            background-color: #e7ab2a;
            color: #1A1A1A;
            font-weight: bold;
            border: 1px solid #C89422;
            border-radius: 5px;
            padding: 6px 10px;
            min-height: 26px;
            selection-background-color: #A52C20;
            selection-color: #FFFFFF;
        }
        QLineEdit:focus, QSpinBox:focus, QComboBox:focus {
            border: 2px solid #FFFFFF;
            background-color: #F0BC40;
        }
        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 25px;
            border-left: 1px solid #C89422;
        }
        QComboBox QAbstractItemView {
            background-color: #101224;
            color: #FFFFFF;
            selection-background-color: #A52C20;
            selection-color: #FFFFFF;
            border: 1px solid #434994;
        }
        QLabel {
            color: #FFFFFF;
        }
        QGroupBox {
            background-color: #26295C;
            border: 1px solid #434994;
            border-radius: 8px;
            margin-top: 15px;
            padding-top: 18px;
            color: #EAB02F;
            font-weight: bold;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 14px;
            padding: 0 6px;
            color: #EAB02F;
        }
        QMessageBox, QDialog {
            background-color: #101224;
            color: #FFFFFF;
        }
        QScrollBar:vertical {
            background-color: #101224;
            width: 10px;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical {
            background-color: #2b2f69;
            border-radius: 5px;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #EAB02F;
        }
        QScrollBar:horizontal {
            background-color: #101224;
            height: 10px;
            border-radius: 5px;
        }
        QScrollBar::handle:horizontal {
            background-color: #2b2f69;
            border-radius: 5px;
            min-width: 20px;
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
    
    cashierDashboard = new CashierDashboard(id, tenNV, userManager, movieManager, roomManager, showtimeManager, ticketManager, comboManager);
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
    
    adminDashboard = new AdminDashboard(id, tenQL, userManager, movieManager, roomManager, showtimeManager, ticketManager, comboManager);
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
