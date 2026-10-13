#include "LoginWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFont>

LoginWindow::LoginWindow(UserManager *userManager, QWidget *parent)
    : QWidget(parent), userManager(userManager)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    QLabel *lblTitle = new QLabel("DANG NHAP", this);
    QFont font = lblTitle->font();
    font.setPointSize(24);
    font.setBold(true);
    lblTitle->setFont(font);
    lblTitle->setAlignment(Qt::AlignCenter);

    txtTaiKhoan = new QLineEdit(this);
    txtTaiKhoan->setPlaceholderText("Tai khoan");
    txtMatKhau = new QLineEdit(this);
    txtMatKhau->setPlaceholderText("Mat khau");
    txtMatKhau->setEchoMode(QLineEdit::Password);

    btnLogin = new QPushButton("Dang Nhap", this);
    
    btnRegister = new QPushButton("Dang Ky", this);
    btnRegister->setFlat(true);
    btnRegister->setCursor(Qt::PointingHandCursor);

    btnChangePassword = new QPushButton("Doi Mat Khau", this);
    btnChangePassword->setFlat(true);
    btnChangePassword->setCursor(Qt::PointingHandCursor);

    mainLayout->addWidget(lblTitle);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(txtTaiKhoan);
    mainLayout->addWidget(txtMatKhau);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(btnLogin);

    QHBoxLayout *linkLayout = new QHBoxLayout();
    linkLayout->addWidget(btnRegister);
    linkLayout->addWidget(btnChangePassword);
    mainLayout->addLayout(linkLayout);

    connect(btnLogin, &QPushButton::clicked, this, &LoginWindow::onLogin);
    connect(btnRegister, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
    connect(btnChangePassword, &QPushButton::clicked, this, &LoginWindow::onChangePasswordClicked);
}

void LoginWindow::onLogin() {
    std::string tk = txtTaiKhoan->text().toStdString();
    std::string mk = txtMatKhau->text().toStdString();

    std::string role;
    std::string userId;
    
    int result = userManager->dangNhap(tk, mk, role, userId);
    
    if (result != -1) {
        emit loginSuccess(QString::fromStdString(role), QString::fromStdString(userId));
    } else {
        QMessageBox::critical(this, "Loi", "Tai khoan hoac mat khau khong chinh xac!");
    }
}

void LoginWindow::onRegisterClicked() {
    emit showRegister();
}

void LoginWindow::onChangePasswordClicked() {
    emit showChangePassword();
}
