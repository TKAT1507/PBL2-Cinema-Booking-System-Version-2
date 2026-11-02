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

    // Center container (no card frame, flat on #212156)
    QWidget *container = new QWidget(this);
    container->setStyleSheet("background: transparent; border: none;");

    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setSpacing(16);
    containerLayout->setAlignment(Qt::AlignCenter);

    // Main system title with 2 thick white lines flanking it
    QHBoxLayout *titleRow = new QHBoxLayout();
    titleRow->setSpacing(14);
    titleRow->setAlignment(Qt::AlignCenter);

    QFrame *lineLeft = new QFrame(container);
    lineLeft->setFixedHeight(5);
    lineLeft->setFixedWidth(110);
    lineLeft->setStyleSheet("background-color: #FFFFFF; border: none;");

    QLabel *lblSystem = new QLabel("HỆ THỐNG ĐẶT VÉ XEM PHIM", container);
    lblSystem->setStyleSheet("color: #FFFFFF; font-size: 24px; font-weight: bold; border: none; background: transparent;");
    lblSystem->setAlignment(Qt::AlignCenter);

    QFrame *lineRight = new QFrame(container);
    lineRight->setFixedHeight(5);
    lineRight->setFixedWidth(110);
    lineRight->setStyleSheet("background-color: #FFFFFF; border: none;");

    titleRow->addWidget(lineLeft);
    titleRow->addWidget(lblSystem);
    titleRow->addWidget(lineRight);

    // Subtitle
    QLabel *lblTitle = new QLabel("Đăng Nhập", container);
    lblTitle->setStyleSheet("color: #FFFFFF; font-size: 16px; font-weight: normal; border: none; background: transparent;");
    lblTitle->setAlignment(Qt::AlignCenter);

    // Inputs: Vuông vức, background #EAB02F
    QString inputStyle = R"(
        QLineEdit {
            background-color: #EAB02F;
            color: #FFFFFF;
            font-size: 14px;
            font-weight: bold;
            border: none;
            border-radius: 0px;
            padding: 0 14px;
        }
        QLineEdit:focus {
            border: 2px solid #FFFFFF;
        }
    )";

    txtTaiKhoan = new QLineEdit(container);
    txtTaiKhoan->setPlaceholderText("Tài khoản");
    txtTaiKhoan->setFixedSize(420, 44);
    txtTaiKhoan->setStyleSheet(inputStyle);

    txtMatKhau = new QLineEdit(container);
    txtMatKhau->setPlaceholderText("Mật khẩu");
    txtMatKhau->setEchoMode(QLineEdit::Password);
    txtMatKhau->setFixedSize(420, 44);
    txtMatKhau->setStyleSheet(inputStyle);

    // Action buttons row: Vuông vức, background #A52C20
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);
    btnLayout->setAlignment(Qt::AlignCenter);

    QString btnStyle = R"(
        QPushButton {
            background-color: #A52C20;
            color: #FFFFFF;
            font-size: 14px;
            font-weight: bold;
            border-radius: 0px;
            border: none;
        }
        QPushButton:hover {
            background-color: #BD362A;
        }
        QPushButton:pressed {
            background-color: #822017;
        }
    )";

    btnLogin = new QPushButton("Đăng nhập", container);
    btnLogin->setFixedSize(200, 44);
    btnLogin->setCursor(Qt::PointingHandCursor);
    btnLogin->setStyleSheet(btnStyle);

    btnRegister = new QPushButton("Đăng kí", container);
    btnRegister->setFixedSize(200, 44);
    btnRegister->setCursor(Qt::PointingHandCursor);
    btnRegister->setStyleSheet(btnStyle);

    btnLayout->addWidget(btnLogin);
    btnLayout->addWidget(btnRegister);

    containerLayout->addLayout(titleRow);
    containerLayout->addWidget(lblTitle);
    containerLayout->addSpacing(10);
    containerLayout->addWidget(txtTaiKhoan, 0, Qt::AlignCenter);
    containerLayout->addWidget(txtMatKhau, 0, Qt::AlignCenter);
    containerLayout->addSpacing(10);
    containerLayout->addLayout(btnLayout);

    rootLayout->addWidget(container);

    connect(btnLogin, &QPushButton::clicked, this, &LoginWindow::onLogin);
    connect(btnRegister, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
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
        QMessageBox::critical(this, "Lỗi", "Tài khoản hoặc mật khẩu không chính xác!");
    }
}

void LoginWindow::onRegisterClicked() {
    emit showRegister();
}
