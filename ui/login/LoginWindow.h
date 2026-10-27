#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "managers/UserManager.h"

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(UserManager *userManager, QWidget *parent = nullptr);

signals:
    void loginSuccess(const QString& role, const QString& userId);
    void showRegister();

private slots:
    void onLogin();
    void onRegisterClicked();

private:
    UserManager *userManager;
    QLineEdit *txtTaiKhoan;
    QLineEdit *txtMatKhau;
    QPushButton *btnLogin;
    QPushButton *btnRegister;
};

#endif // LOGINWINDOW_H
