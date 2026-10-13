#ifndef CHANGEPASSWORDWINDOW_H
#define CHANGEPASSWORDWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "managers/UserManager.h"

class ChangePasswordWindow : public QWidget {
    Q_OBJECT

public:
    explicit ChangePasswordWindow(UserManager *userManager, QWidget *parent = nullptr);

signals:
    void goBack();

private slots:
    void onChangePassword();
    void onBack();

private:
    UserManager *userManager;
    QLineEdit *txtTaiKhoan;
    QLineEdit *txtMatKhauCu;
    QLineEdit *txtMatKhauMoi;
    QLineEdit *txtXacNhanMatKhauMoi;
    QPushButton *btnChange;
    QPushButton *btnBack;
};

#endif // CHANGEPASSWORDWINDOW_H
