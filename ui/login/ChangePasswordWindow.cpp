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

    // Center container (no card frame, flat on #101224)
    QWidget *container = new QWidget(this);
    container->setStyleSheet("background: transparent; border: none;");

    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setSpacing(14);
    containerLayout->setAlignment(Qt::AlignCenter);

    // Title: ———— Đổi Mật Khẩu ————
    QHBoxLayout *titleRow = new QHBoxLayout();
    titleRow->setSpacing(14);
    titleRow->setAlignment(Qt::AlignCenter);

    QFrame *lineLeft = new QFrame(container);
    lineLeft->setFixedHeight(5);
    lineLeft->setFixedWidth(100);
    lineLeft->setStyleSheet("background-color: #FFFFFF; border: none;");

    QLabel *lblTitle = new QLabel("ĐỔI MẬT KHẨU", container);
    lblTitle->setStyleSheet("color: #FFFFFF; font-size: 24px; font-weight: bold; border: none; background: transparent;");
    lblTitle->setAlignment(Qt::AlignCenter);

    QFrame *lineRight = new QFrame(container);
    lineRight->setFixedHeight(5);
    lineRight->setFixedWidth(100);
    lineRight->setStyleSheet("background-color: #FFFFFF; border: none;");

    titleRow->addWidget(lineLeft);
    titleRow->addWidget(lblTitle);
    titleRow->addWidget(lineRight);

    QString inputStyle = R"(
        QLineEdit {
            background-color: #EAB02F;
            color: #FFFFFF;
            font-size: 14px;
            font-weight: bold;
            border: none;
            border-radius: 0px;
            padding: 0 14px;
            min-height: 42px;
        }
        QLineEdit:focus {
            border: 2px solid #FFFFFF;
        }
    )";

    txtTaiKhoan = new QLineEdit(container);
    txtTaiKhoan->setPlaceholderText("Tài khoản");
    txtTaiKhoan->setFixedSize(420, 42);
    txtTaiKhoan->setStyleSheet(inputStyle);

    txtMatKhauCu = new QLineEdit(container);
    txtMatKhauCu->setPlaceholderText("Mật khẩu cũ");
    txtMatKhauCu->setEchoMode(QLineEdit::Password);
    txtMatKhauCu->setFixedSize(420, 42);
    txtMatKhauCu->setStyleSheet(inputStyle);

    txtMatKhauMoi = new QLineEdit(container);
    txtMatKhauMoi->setPlaceholderText("Mật khẩu mới");
    txtMatKhauMoi->setEchoMode(QLineEdit::Password);
    txtMatKhauMoi->setFixedSize(420, 42);
    txtMatKhauMoi->setStyleSheet(inputStyle);

    txtXacNhanMatKhauMoi = new QLineEdit(container);
    txtXacNhanMatKhauMoi->setPlaceholderText("Xác nhận mật khẩu mới");
    txtXacNhanMatKhauMoi->setEchoMode(QLineEdit::Password);
    txtXacNhanMatKhauMoi->setFixedSize(420, 42);
    txtXacNhanMatKhauMoi->setStyleSheet(inputStyle);

    // Buttons row
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

    btnChange = new QPushButton("Xác Nhận", container);
    btnChange->setFixedSize(200, 44);
    btnChange->setCursor(Qt::PointingHandCursor);
    btnChange->setStyleSheet(btnStyle);

    btnBack = new QPushButton("Quay Lại", container);
    btnBack->setFixedSize(200, 44);
    btnBack->setCursor(Qt::PointingHandCursor);
    btnBack->setStyleSheet(btnStyle);

    btnLayout->addWidget(btnChange);
    btnLayout->addWidget(btnBack);

    containerLayout->addLayout(titleRow);
    containerLayout->addSpacing(10);
    containerLayout->addWidget(txtTaiKhoan, 0, Qt::AlignCenter);
    containerLayout->addWidget(txtMatKhauCu, 0, Qt::AlignCenter);
    containerLayout->addWidget(txtMatKhauMoi, 0, Qt::AlignCenter);
    containerLayout->addWidget(txtXacNhanMatKhauMoi, 0, Qt::AlignCenter);
    containerLayout->addSpacing(10);
    containerLayout->addLayout(btnLayout);

    rootLayout->addWidget(container);

    connect(btnChange, &QPushButton::clicked, this, &ChangePasswordWindow::onChangePassword);
    connect(btnBack, &QPushButton::clicked, this, &ChangePasswordWindow::onBack);
}

void ChangePasswordWindow::onChangePassword() {
    std::string tk = txtTaiKhoan->text().toStdString();
    std::string mkCu = txtMatKhauCu->text().toStdString();
    std::string mkMoi = txtMatKhauMoi->text().toStdString();
    std::string xnMk = txtXacNhanMatKhauMoi->text().toStdString();

    if (tk.empty() || mkCu.empty() || mkMoi.empty()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng nhập đầy đủ thông tin!");
        return;
    }

    if (mkMoi != xnMk) {
        QMessageBox::warning(this, "Thông báo", "Mật khẩu xác nhận không khớp!");
        return;
    }

    if (userManager->doiMatKhau(tk, mkCu, mkMoi)) {
        QMessageBox::information(this, "Thành công", "Đổi mật khẩu thành công!");
        emit goBack();
    } else {
        QMessageBox::critical(this, "Lỗi", "Tài khoản hoặc mật khẩu cũ không đúng!");
    }
}

void ChangePasswordWindow::onBack() {
    emit goBack();
}
