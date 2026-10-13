#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "managers/UserManager.h"
#include "managers/MovieManager.h"
#include "managers/RoomManager.h"
#include "managers/ShowtimeManager.h"
#include "managers/TicketManager.h"
#include "managers/ComboManager.h"

class LoginWindow;
class RegisterWindow;
class ChangePasswordWindow;
class CustomerDashboard;
class CashierDashboard;
class AdminDashboard;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initManagers();

public slots:
    void showLogin();
    void showRegister();
    void showChangePassword();
    void showCustomerDashboard(const QString& maKH);
    void showCashierDashboard(const QString& maNV);
    void showAdminDashboard(const QString& maQL);
    void logout();

private:
    QStackedWidget *stackedWidget;
    UserManager *userManager;
    MovieManager *movieManager;
    RoomManager *roomManager;
    ShowtimeManager *showtimeManager;
    TicketManager *ticketManager;
    ComboManager *comboManager;

    LoginWindow *loginWindow;
    RegisterWindow *registerWindow;
    ChangePasswordWindow *changePasswordWindow;
    CustomerDashboard *customerDashboard;
    CashierDashboard *cashierDashboard;
    AdminDashboard *adminDashboard;
};

#endif // MAINWINDOW_H
