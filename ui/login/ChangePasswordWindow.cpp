#include "ChangePasswordWindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QLabel>
#include <QFont>

ChangePasswordWindow::ChangePasswordWindow(UserManager *userManager, QWidget *parent)
    : QWidget(parent), userManager(userManager)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    QLabel *lblTitle = new QLabel("DOI MAT KHAU", this);
    QFont font = lblTitle->font();
    font.setPointSize(24);
    font.setBold(true);
    lblTitle->setFont(font);
    lblTitle->setAlignment(Qt::AlignCenter);

    QFormLayout *formLayout = new QFormLayout();
    
    txtTaiKhoan = new QLineEdit(this);
    txtMatKhauCu = new QLineEdit(this);
    txtMatKhauCu->setEchoMode(QLineEdit::Password);
    txtMatKhauMoi = new QLineEdit(this);
    txtMatKhauMoi->setEchoMode(QLineEdit::Password);
    txtXacNhanMatKhauMoi = new QLineEdit(this);
    txtXacNhanMatKhauMoi->setEchoMode(QLineEdit::Password);

    formLayout->addRow("Tai Khoan:", txtTaiKhoan);
    formLayout->addRow("Mat Khau Cu:", txtMatKhauCu);
    formLayout->addRow("Mat Khau Moi:", txtMatKhauMoi);
    formLayout->addRow("Xac Nhan MK Moi:", txtXacNhanMatKhauMoi);

    btnChange = new QPushButton("Doi Mat Khau", this);
    btnBack = new QPushButton("Quay Lai", this);

    mainLayout->addWidget(lblTitle);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(btnChange);
    mainLayout->addWidget(btnBack);

    connect(btnChange, &QPushButton::clicked, this, &ChangePasswordWindow::onChangePassword);
    connect(btnBack, &QPushButton::clicked, this, &ChangePasswordWindow::onBack);
}

void ChangePasswordWindow::onChangePassword() {
    if (txtMatKhauMoi->text() != txtXacNhanMatKhauMoi->text()) {
        QMessageBox::warning(this, "Loi", "Mat khau moi xac nhan khong khop!");
        return;
    }

    std::string tk = txtTaiKhoan->text().toStdString();
    std::string mkCu = txtMatKhauCu->text().toStdString();
    std::string mkMoi = txtMatKhauMoi->text().toStdString();

    if (tk.empty() || mkCu.empty() || mkMoi.empty()) {
        QMessageBox::warning(this, "Loi", "Vui long dien day du thong tin!");
        return;
    }

    if (userManager->doiMatKhau(tk, mkCu, mkMoi)) {
        userManager->saveAll();
        QMessageBox::information(this, "Thanh cong", "Doi mat khau thanh cong!");
        emit goBack();
    } else {
        QMessageBox::critical(this, "Loi", "Tai khoan hoac mat khau cu khong dung!");
    }
}

void ChangePasswordWindow::onBack() {
    emit goBack();
}
