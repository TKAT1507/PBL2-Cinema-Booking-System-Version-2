#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include "managers/UserManager.h"

class RegisterWindow : public QWidget {
    Q_OBJECT

public:
    explicit RegisterWindow(UserManager *userManager, QWidget *parent = nullptr);

signals:
    void registerSuccess();
    void goBack();

private slots:
    void onRegister();
    void onBack();

private:
    UserManager *userManager;
    QLineEdit *txtTaiKhoan;
    QLineEdit *txtMatKhau;
    QLineEdit *txtXacNhanMatKhau;
    QLineEdit *txtSoDienThoai;
    QSpinBox *spinTuoi;
    QPushButton *btnRegister;
    QPushButton *btnBack;
};

#endif // REGISTERWINDOW_H
