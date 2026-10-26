#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "managers/UserManager.h"

class ChangePasswordDialog : public QDialog {
    Q_OBJECT

public:
    explicit ChangePasswordDialog(UserManager *userManager, const std::string& taiKhoan, QWidget *parent = nullptr);

private slots:
    void onChange();

private:
    UserManager *userManager;
    std::string taiKhoan;
    QLineEdit *txtTaiKhoan;
    QLineEdit *txtMatKhauCu;
    QLineEdit *txtMatKhauMoi;
    QLineEdit *txtXacNhanMatKhauMoi;
    QPushButton *btnConfirm;
    QPushButton *btnCancel;
};

#endif // CHANGEPASSWORDDIALOG_H
