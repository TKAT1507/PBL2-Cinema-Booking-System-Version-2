#include "RegisterWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QFont>
#include <QFrame>

RegisterWindow::RegisterWindow(UserManager *userManager, QWidget *parent)
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
    QLabel *lblTitle = new QLabel("ĐĂNG KÍ", card);
    lblTitle->setStyleSheet("color: #FFFFFF; font-size: 24px; font-weight: bold; border: none; background: transparent;");
    lblTitle->setAlignment(Qt::AlignCenter);

    QString inputStyle = R"(
        QLineEdit, QSpinBox {
            background-color: #E6B042;
            color: #1A1B41;
            font-size: 14px;
            font-weight: bold;
            border: 1px solid #C49129;
            border-radius: 6px;
            padding: 0 14px;
            min-height: 38px;
        }
        QLineEdit:focus, QSpinBox:focus {
            border: 2px solid #FFFFFF;
        }
    )";

    txtTaiKhoan = new QLineEdit(card);
    txtTaiKhoan->setPlaceholderText("Tài khoản");
    txtTaiKhoan->setStyleSheet(inputStyle);

    txtMatKhau = new QLineEdit(card);
    txtMatKhau->setPlaceholderText("Mật khẩu");
    txtMatKhau->setEchoMode(QLineEdit::Password);
    txtMatKhau->setStyleSheet(inputStyle);

    txtXacNhanMatKhau = new QLineEdit(card);
    txtXacNhanMatKhau->setPlaceholderText("Xác nhận mật khẩu");
    txtXacNhanMatKhau->setEchoMode(QLineEdit::Password);
    txtXacNhanMatKhau->setStyleSheet(inputStyle);

    txtSoDienThoai = new QLineEdit(card);
    txtSoDienThoai->setPlaceholderText("Số điện thoại");
    txtSoDienThoai->setStyleSheet(inputStyle);

    spinTuoi = new QSpinBox(card);
    spinTuoi->setRange(1, 120);
    spinTuoi->setValue(18);
    spinTuoi->setPrefix("Tuổi: ");
    spinTuoi->setStyleSheet(inputStyle);

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

    btnRegister = new QPushButton("Đăng Kí", card);
    btnRegister->setCursor(Qt::PointingHandCursor);
    btnRegister->setStyleSheet(btnStyle);

    btnBack = new QPushButton("Quay Lại", card);
    btnBack->setCursor(Qt::PointingHandCursor);
    btnBack->setStyleSheet(btnStyle);

    btnLayout->addWidget(btnRegister);
    btnLayout->addWidget(btnBack);

    cardLayout->addWidget(lblSystem);
    cardLayout->addWidget(line);
    cardLayout->addSpacing(5);
    cardLayout->addWidget(lblTitle);
    cardLayout->addSpacing(10);
    cardLayout->addWidget(txtTaiKhoan);
    cardLayout->addWidget(txtMatKhau);
    cardLayout->addWidget(txtXacNhanMatKhau);
    cardLayout->addWidget(txtSoDienThoai);
    cardLayout->addWidget(spinTuoi);
    cardLayout->addSpacing(10);
    cardLayout->addLayout(btnLayout);

    rootLayout->addWidget(card);

    connect(btnRegister, &QPushButton::clicked, this, &RegisterWindow::onRegister);
    connect(btnBack, &QPushButton::clicked, this, &RegisterWindow::onBack);
}

void RegisterWindow::onRegister() {
    if (txtMatKhau->text() != txtXacNhanMatKhau->text()) {
        QMessageBox::warning(this, "Loi", "Mat khau xac nhan khong khop!");
        return;
    }

    std::string tk = txtTaiKhoan->text().toStdString();
    std::string mk = txtMatKhau->text().toStdString();
    std::string sdt = txtSoDienThoai->text().toStdString();
    int tuoi = spinTuoi->value();

    if (tk.empty() || mk.empty() || sdt.empty()) {
        QMessageBox::warning(this, "Loi", "Vui long dien day du thong tin!");
        return;
    }

    if (userManager->dangKy(tk, mk, sdt, tuoi)) {
        userManager->saveAll();
        QMessageBox::information(this, "Thanh cong", "Dang ky thanh cong!");
        emit registerSuccess();
        emit goBack();
    } else {
        QMessageBox::critical(this, "Loi", "Tai khoan da ton tai!");
    }
}

void RegisterWindow::onBack() {
    emit goBack();
}
