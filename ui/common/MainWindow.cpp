#include "MainWindow.h"
#include "Theme.h"
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
    setWindowTitle("PBL2 Cinema — Hệ Thống Đặt Vé Xem Phim");
    setMinimumSize(1200, 800);

    // Apply unified Dynamic & Soft Cinema theme
    setStyleSheet(Theme::globalStylesheet());

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
    loginWindow->resetFields();
    stackedWidget->setCurrentWidget(loginWindow);
}

void MainWindow::showRegister() {
    if (!registerWindow) {
        registerWindow = new RegisterWindow(userManager);
        connect(registerWindow, &RegisterWindow::goBack, this, &MainWindow::showLogin);
        connect(registerWindow, &RegisterWindow::registerSuccess, this, &MainWindow::showLogin);
        stackedWidget->addWidget(registerWindow);
    }
    registerWindow->resetFields();
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
    std::string id = maNV.toStdString();
    std::string tenNV = "Thu Ngân";
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
    std::string id = maQL.toStdString();
    std::string tenQL = "Quản Lý";
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
