#include "LoginWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFont>

#include "LoginWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFont>
#include <QFrame>

LoginWindow::LoginWindow(UserManager *userManager, QWidget *parent)
    : QWidget(parent), userManager(userManager)
{
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setAlignment(Qt::AlignCenter);

    // Center container card
    QFrame *card = new QFrame(this);
    card->setFixedWidth(460);
    card->setStyleSheet(R"(
        QFrame {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2C2E6A, stop:1 #1E1F4B);
            border: 2px solid #E6B042;
            border-radius: 12px;
            padding: 30px;
        }
    )");

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(15);
    cardLayout->setAlignment(Qt::AlignCenter);

    // Main system title
    QLabel *lblSystem = new QLabel("HỆ THỐNG ĐẶT VÉ XEM PHIM", card);
    lblSystem->setStyleSheet("color: #F5C542; font-size: 20px; font-weight: bold; border: none; background: transparent;");
    lblSystem->setAlignment(Qt::AlignCenter);

    // Divider line
    QFrame *line = new QFrame(card);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #FFFFFF; background-color: #FFFFFF; max-height: 1px; border: none;");

    // Subtitle
    QLabel *lblTitle = new QLabel("ĐĂNG NHẬP", card);
    lblTitle->setStyleSheet("color: #FFFFFF; font-size: 24px; font-weight: bold; border: none; background: transparent;");
    lblTitle->setAlignment(Qt::AlignCenter);

    // Inputs
    txtTaiKhoan = new QLineEdit(card);
    txtTaiKhoan->setPlaceholderText("Tài khoản");
    txtTaiKhoan->setFixedHeight(42);
    txtTaiKhoan->setStyleSheet(R"(
        QLineEdit {
            background-color: #E6B042;
            color: #1A1B41;
            font-size: 14px;
            font-weight: bold;
            border: 1px solid #C49129;
            border-radius: 6px;
            padding: 0 14px;
        }
        QLineEdit:focus {
            border: 2px solid #FFFFFF;
        }
    )");

    txtMatKhau = new QLineEdit(card);
    txtMatKhau->setPlaceholderText("Mật khẩu");
    txtMatKhau->setEchoMode(QLineEdit::Password);
    txtMatKhau->setFixedHeight(42);
    txtMatKhau->setStyleSheet(R"(
        QLineEdit {
            background-color: #E6B042;
            color: #1A1B41;
            font-size: 14px;
            font-weight: bold;
            border: 1px solid #C49129;
            border-radius: 6px;
            padding: 0 14px;
        }
        QLineEdit:focus {
            border: 2px solid #FFFFFF;
        }
    )");

    // Action buttons row
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(15);

    btnLogin = new QPushButton("Đăng Nhập", card);
    btnLogin->setFixedHeight(42);
    btnLogin->setCursor(Qt::PointingHandCursor);
    btnLogin->setStyleSheet(R"(
        QPushButton {
            background-color: #9E221E;
            color: #FFFFFF;
            font-size: 15px;
            font-weight: bold;
            border-radius: 6px;
            border: none;
        }
        QPushButton:hover {
            background-color: #B52A26;
        }
        QPushButton:pressed {
            background-color: #7A1916;
        }
    )");

    btnRegister = new QPushButton("Đăng Kí", card);
    btnRegister->setFixedHeight(42);
    btnRegister->setCursor(Qt::PointingHandCursor);
    btnRegister->setStyleSheet(R"(
        QPushButton {
            background-color: #9E221E;
            color: #FFFFFF;
            font-size: 15px;
            font-weight: bold;
            border-radius: 6px;
            border: none;
        }
        QPushButton:hover {
            background-color: #B52A26;
        }
        QPushButton:pressed {
            background-color: #7A1916;
        }
    )");

    btnLayout->addWidget(btnLogin);
    btnLayout->addWidget(btnRegister);

    // Link: Doi mat khau
    btnChangePassword = new QPushButton("Quên mật khẩu? Đổi Mật Khẩu", card);
    btnChangePassword->setFlat(true);
    btnChangePassword->setCursor(Qt::PointingHandCursor);
    btnChangePassword->setStyleSheet("QPushButton { color: #F5C542; font-size: 12px; border: none; background: transparent; text-decoration: underline; } QPushButton:hover { color: #FFFFFF; }");

    cardLayout->addWidget(lblSystem);
    cardLayout->addWidget(line);
    cardLayout->addSpacing(5);
    cardLayout->addWidget(lblTitle);
    cardLayout->addSpacing(10);
    cardLayout->addWidget(txtTaiKhoan);
    cardLayout->addWidget(txtMatKhau);
    cardLayout->addSpacing(10);
    cardLayout->addLayout(btnLayout);
    cardLayout->addWidget(btnChangePassword, 0, Qt::AlignCenter);

    rootLayout->addWidget(card);

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
