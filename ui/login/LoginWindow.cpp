#include "LoginWindow.h"
#include "../common/Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFrame>

LoginWindow::LoginWindow(UserManager *userManager, QWidget *parent)
    : QWidget(parent), userManager(userManager)
{
    setObjectName("loginPage");
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet(R"(
        QWidget#loginPage {
            background: qradialgradient(cx:0.5, cy:0.18, radius:0.9, fx:0.5, fy:0.12,
                stop:0 #2B254D, stop:0.5 #1B1D33, stop:1 #131524);
        }
    )");

    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setAlignment(Qt::AlignCenter);
    rootLayout->setContentsMargins(24, 24, 24, 24);

    // Cinema Brand Header
    QWidget *brandBox = new QWidget(this);
    brandBox->setStyleSheet("background: transparent;");
    QVBoxLayout *brandLayout = new QVBoxLayout(brandBox);
    brandLayout->setSpacing(10);
    brandLayout->setContentsMargins(0, 0, 0, 20);
    brandLayout->setAlignment(Qt::AlignCenter);

    QLabel *lblBrand = new QLabel("PBL2 CINEMA", brandBox);
    lblBrand->setAlignment(Qt::AlignCenter);
    lblBrand->setStyleSheet(R"(
        QLabel {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FF6B6B, stop:1 #FF8E72);
            color: #FFFFFF;
            font-size: 12px;
            font-weight: bold;
            letter-spacing: 4px;
            border: none;
            border-radius: 13px;
            padding: 6px 20px;
        }
    )");

    QLabel *lblSystem = new QLabel("HỆ THỐNG ĐẶT VÉ XEM PHIM", brandBox);
    lblSystem->setAlignment(Qt::AlignCenter);
    lblSystem->setStyleSheet("color: #F3F4FA; font-size: 28px; font-weight: bold; letter-spacing: 1.5px; background: transparent;");

    brandLayout->addWidget(lblBrand, 0, Qt::AlignCenter);
    brandLayout->addWidget(lblSystem, 0, Qt::AlignCenter);

    // Soft Midnight Floating Auth Card
    QFrame *card = new QFrame(this);
    card->setObjectName("authCard");
    card->setFixedWidth(450);
    card->setStyleSheet(Theme::authCardStyle());

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(38, 36, 38, 36);
    cardLayout->setSpacing(10);

    QLabel *lblTitle = new QLabel("Chào mừng trở lại!", card);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("color: #F3F4FA; font-size: 23px; font-weight: bold; background: transparent; border: none;");

    QLabel *lblSub = new QLabel("Đăng nhập để đặt vé nhanh chóng và tận hưởng bộ phim yêu thích", card);
    lblSub->setAlignment(Qt::AlignCenter);
    lblSub->setWordWrap(true);
    lblSub->setStyleSheet("color: #9AA2C2; font-size: 12px; background: transparent; border: none;");

    QFrame *divider = new QFrame(card);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("background-color: #303654; max-height: 1px; border: none;");

    auto makeFieldLabel = [card](const QString& text) {
        QLabel* lbl = new QLabel(text, card);
        lbl->setStyleSheet("color: #B6BEDE; font-size: 12px; font-weight: 600; background: transparent; border: none;");
        return lbl;
    };

    QString inputStyle = R"(
        QLineEdit {
            background-color: #191C2E;
            color: #F3F4FA;
            font-size: 14px;
            border: 1.5px solid #33395A;
            border-radius: 12px;
            padding: 0 16px;
        }
        QLineEdit:hover {
            border-color: #525B8A;
            background-color: #1E2238;
        }
        QLineEdit:focus {
            border: 1.5px solid #FF6B6B;
            background-color: #212640;
        }
    )";

    txtTaiKhoan = new QLineEdit(card);
    txtTaiKhoan->setPlaceholderText("Nhập tên tài khoản...");
    txtTaiKhoan->setFixedHeight(46);
    txtTaiKhoan->setStyleSheet(inputStyle);

    txtMatKhau = new QLineEdit(card);
    txtMatKhau->setPlaceholderText("Nhập mật khẩu...");
    txtMatKhau->setEchoMode(QLineEdit::Password);
    txtMatKhau->setFixedHeight(46);
    txtMatKhau->setStyleSheet(inputStyle);

    btnLogin = new QPushButton("Đăng Nhập", card);
    btnLogin->setFixedHeight(46);
    btnLogin->setCursor(Qt::PointingHandCursor);
    btnLogin->setStyleSheet(Theme::primaryButtonStyle(46, 15));

    QHBoxLayout *regRow = new QHBoxLayout();
    regRow->setSpacing(6);
    regRow->setAlignment(Qt::AlignCenter);

    QLabel *lblNoAcc = new QLabel("Chưa có tài khoản?", card);
    lblNoAcc->setStyleSheet("color: #9AA2C2; font-size: 13px; background: transparent; border: none;");

    btnRegister = new QPushButton("Đăng Ký Ngay", card);
    btnRegister->setCursor(Qt::PointingHandCursor);
    btnRegister->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #FFD166;
            font-size: 13px;
            font-weight: bold;
            border: none;
            padding: 4px 6px;
            min-height: 24px;
        }
        QPushButton:hover {
            color: #FF8E72;
            text-decoration: underline;
        }
    )");

    regRow->addWidget(lblNoAcc);
    regRow->addWidget(btnRegister);

    cardLayout->addWidget(lblTitle);
    cardLayout->addWidget(lblSub);
    cardLayout->addSpacing(6);
    cardLayout->addWidget(divider);
    cardLayout->addSpacing(8);
    cardLayout->addWidget(makeFieldLabel("Tài khoản"));
    cardLayout->addWidget(txtTaiKhoan);
    cardLayout->addSpacing(4);
    cardLayout->addWidget(makeFieldLabel("Mật khẩu"));
    cardLayout->addWidget(txtMatKhau);
    cardLayout->addSpacing(14);
    cardLayout->addWidget(btnLogin);
    cardLayout->addSpacing(6);
    cardLayout->addLayout(regRow);

    rootLayout->addWidget(brandBox, 0, Qt::AlignCenter);
    rootLayout->addWidget(card, 0, Qt::AlignCenter);

    connect(btnLogin, &QPushButton::clicked, this, &LoginWindow::onLogin);
    connect(btnRegister, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
    connect(txtTaiKhoan, &QLineEdit::returnPressed, this, [this]() { txtMatKhau->setFocus(); });
    connect(txtMatKhau, &QLineEdit::returnPressed, this, &LoginWindow::onLogin);
}

void LoginWindow::resetFields() {
    txtTaiKhoan->clear();
    txtMatKhau->clear();
    txtTaiKhoan->setFocus();
}

void LoginWindow::onLogin() {
    std::string tk = txtTaiKhoan->text().trimmed().toStdString();
    std::string mk = txtMatKhau->text().toStdString();

    std::string role;
    std::string userId;

    int result = userManager->dangNhap(tk, mk, role, userId);

    if (result != -1) {
        emit loginSuccess(QString::fromStdString(role), QString::fromStdString(userId));
    } else {
        QMessageBox::critical(this, "Đăng Nhập Thất Bại", "Tài khoản hoặc mật khẩu không chính xác!");
    }
}

void LoginWindow::onRegisterClicked() {
    emit showRegister();
}
