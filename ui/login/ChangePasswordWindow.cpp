#include "ChangePasswordWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QFont>
#include <QFrame>

ChangePasswordWindow::ChangePasswordWindow(UserManager *userManager, QWidget *parent)
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
    cardLayout->setSpacing(12);
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
    QLabel *lblTitle = new QLabel("ĐỔI MẬT KHẨU", card);
    lblTitle->setStyleSheet("color: #FFFFFF; font-size: 24px; font-weight: bold; border: none; background: transparent;");
    lblTitle->setAlignment(Qt::AlignCenter);

    QString inputStyle = R"(
        QLineEdit {
            background-color: #E6B042;
            color: #1A1B41;
            font-size: 14px;
            font-weight: bold;
            border: 1px solid #C49129;
            border-radius: 6px;
            padding: 0 14px;
            min-height: 38px;
        }
        QLineEdit:focus {
            border: 2px solid #FFFFFF;
        }
    )";

    txtTaiKhoan = new QLineEdit(card);
    txtTaiKhoan->setPlaceholderText("Tài khoản");
    txtTaiKhoan->setStyleSheet(inputStyle);

    txtMatKhauCu = new QLineEdit(card);
    txtMatKhauCu->setPlaceholderText("Mật khẩu cũ");
    txtMatKhauCu->setEchoMode(QLineEdit::Password);
    txtMatKhauCu->setStyleSheet(inputStyle);

    txtMatKhauMoi = new QLineEdit(card);
    txtMatKhauMoi->setPlaceholderText("Mật khẩu mới");
    txtMatKhauMoi->setEchoMode(QLineEdit::Password);
    txtMatKhauMoi->setStyleSheet(inputStyle);

    txtXacNhanMatKhauMoi = new QLineEdit(card);
    txtXacNhanMatKhauMoi->setPlaceholderText("Xác nhận mật khẩu mới");
    txtXacNhanMatKhauMoi->setEchoMode(QLineEdit::Password);
    txtXacNhanMatKhauMoi->setStyleSheet(inputStyle);

    // Two buttons row
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(15);

    QString btnStyle = R"(
        QPushButton {
            background-color: #9E221E;
            color: #FFFFFF;
            font-size: 15px;
            font-weight: bold;
            border-radius: 6px;
            border: none;
            min-height: 40px;
        }
        QPushButton:hover {
            background-color: #B52A26;
        }
        QPushButton:pressed {
            background-color: #7A1916;
        }
    )";

    btnChange = new QPushButton("Đổi Mật Khẩu", card);
    btnChange->setCursor(Qt::PointingHandCursor);
    btnChange->setStyleSheet(btnStyle);

    btnBack = new QPushButton("Quay Lại", card);
    btnBack->setCursor(Qt::PointingHandCursor);
    btnBack->setStyleSheet(btnStyle);

    btnLayout->addWidget(btnChange);
    btnLayout->addWidget(btnBack);

    cardLayout->addWidget(lblSystem);
    cardLayout->addWidget(line);
    cardLayout->addSpacing(5);
    cardLayout->addWidget(lblTitle);
    cardLayout->addSpacing(10);
    cardLayout->addWidget(txtTaiKhoan);
    cardLayout->addWidget(txtMatKhauCu);
    cardLayout->addWidget(txtMatKhauMoi);
    cardLayout->addWidget(txtXacNhanMatKhauMoi);
    cardLayout->addSpacing(10);
    cardLayout->addLayout(btnLayout);

    rootLayout->addWidget(card);

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
