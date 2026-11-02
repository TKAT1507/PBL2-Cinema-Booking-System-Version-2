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

    // Center container (no card frame, flat on #101224)
    QWidget *container = new QWidget(this);
    container->setStyleSheet("background: transparent; border: none;");

    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setSpacing(14);
    containerLayout->setAlignment(Qt::AlignCenter);

    // Title: ———— Đăng Kí ————
    QHBoxLayout *titleRow = new QHBoxLayout();
    titleRow->setSpacing(14);
    titleRow->setAlignment(Qt::AlignCenter);

    QFrame *lineLeft = new QFrame(container);
    lineLeft->setFixedHeight(5);
    lineLeft->setFixedWidth(110);
    lineLeft->setStyleSheet("background-color: #FFFFFF; border: none;");

    QLabel *lblTitle = new QLabel("Đăng Kí", container);
    lblTitle->setStyleSheet("color: #FFFFFF; font-size: 36px; font-weight: bold; border: none; background: transparent;");
    lblTitle->setAlignment(Qt::AlignCenter);

    QFrame *lineRight = new QFrame(container);
    lineRight->setFixedHeight(5);
    lineRight->setFixedWidth(110);
    lineRight->setStyleSheet("background-color: #FFFFFF; border: none;");

    titleRow->addWidget(lineLeft);
    titleRow->addWidget(lblTitle);
    titleRow->addWidget(lineRight);

    QString inputStyle = R"(
        QLineEdit, QSpinBox {
            background-color: #EAB02F;
            color: #FFFFFF;
            font-size: 14px;
            font-weight: bold;
            border: none;
            border-radius: 0px;
            padding: 0 14px;
            min-height: 38px;
        }
        QLineEdit:focus, QSpinBox:focus {
            border: 2px solid #FFFFFF;
        }
    )";

    txtTaiKhoan = new QLineEdit(container);
    txtTaiKhoan->setPlaceholderText("Tài khoản");
    txtTaiKhoan->setFixedSize(420, 42);
    txtTaiKhoan->setStyleSheet(inputStyle);

    txtMatKhau = new QLineEdit(container);
    txtMatKhau->setPlaceholderText("Mật khẩu");
    txtMatKhau->setEchoMode(QLineEdit::Password);
    txtMatKhau->setFixedSize(420, 42);
    txtMatKhau->setStyleSheet(inputStyle);

    txtXacNhanMatKhau = new QLineEdit(container);
    txtXacNhanMatKhau->setPlaceholderText("Xác nhận mật khẩu");
    txtXacNhanMatKhau->setEchoMode(QLineEdit::Password);
    txtXacNhanMatKhau->setFixedSize(420, 42);
    txtXacNhanMatKhau->setStyleSheet(inputStyle);

    txtSoDienThoai = new QLineEdit(container);
    txtSoDienThoai->setPlaceholderText("Số điện thoại");
    txtSoDienThoai->setFixedSize(420, 42);
    txtSoDienThoai->setStyleSheet(inputStyle);

    spinTuoi = new QSpinBox(container);
    spinTuoi->setRange(1, 120);
    spinTuoi->setValue(18);
    spinTuoi->setPrefix("Tuổi: ");
    spinTuoi->setFixedSize(420, 42);
    spinTuoi->setStyleSheet(inputStyle);

    // Two buttons row
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

    btnRegister = new QPushButton("Đăng Kí", container);
    btnRegister->setFixedSize(200, 44);
    btnRegister->setCursor(Qt::PointingHandCursor);
    btnRegister->setStyleSheet(btnStyle);

    btnBack = new QPushButton("Quay lại", container);
    btnBack->setFixedSize(200, 44);
    btnBack->setCursor(Qt::PointingHandCursor);
    btnBack->setStyleSheet(btnStyle);

    btnLayout->addWidget(btnRegister);
    btnLayout->addWidget(btnBack);

    containerLayout->addLayout(titleRow);
    containerLayout->addSpacing(10);
    containerLayout->addWidget(txtTaiKhoan, 0, Qt::AlignCenter);
    containerLayout->addWidget(txtMatKhau, 0, Qt::AlignCenter);
    containerLayout->addWidget(txtXacNhanMatKhau, 0, Qt::AlignCenter);
    containerLayout->addWidget(txtSoDienThoai, 0, Qt::AlignCenter);
    containerLayout->addWidget(spinTuoi, 0, Qt::AlignCenter);
    containerLayout->addSpacing(10);
    containerLayout->addLayout(btnLayout);

    rootLayout->addWidget(container);

    connect(btnRegister, &QPushButton::clicked, this, &RegisterWindow::onRegister);
    connect(btnBack, &QPushButton::clicked, this, &RegisterWindow::onBack);
}

void RegisterWindow::onRegister() {
    std::string tk = txtTaiKhoan->text().toStdString();
    std::string mk = txtMatKhau->text().toStdString();
    std::string xnmk = txtXacNhanMatKhau->text().toStdString();
    std::string sdt = txtSoDienThoai->text().toStdString();
    int tuoi = spinTuoi->value();

    if (tk.empty() || mk.empty() || sdt.empty()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng nhập đầy đủ thông tin!");
        return;
    }

    if (mk != xnmk) {
        QMessageBox::warning(this, "Thông báo", "Mật khẩu xác nhận không khớp!");
        return;
    }

    if (userManager->dangKy(tk, mk, sdt, tuoi)) {
        QMessageBox::information(this, "Thành công", "Đăng ký tài khoản thành công!");
        emit registerSuccess();
    } else {
        QMessageBox::critical(this, "Lỗi", "Tài khoản đã tồn tại!");
    }
}

void RegisterWindow::onBack() {
    emit goBack();
}
