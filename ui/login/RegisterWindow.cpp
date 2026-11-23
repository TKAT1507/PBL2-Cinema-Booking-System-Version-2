#include "RegisterWindow.h"
#include "../common/Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QFrame>

RegisterWindow::RegisterWindow(UserManager *userManager, QWidget *parent)
    : QWidget(parent), userManager(userManager)
{
    setObjectName("registerPage");
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet(R"(
        QWidget#registerPage {
            background: #F5F6F8;
        }
    )");

    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setAlignment(Qt::AlignCenter);
    rootLayout->setContentsMargins(24, 24, 24, 24);

    QFrame *card = new QFrame(this);
    card->setObjectName("authCard");
    card->setFixedWidth(480);
    card->setStyleSheet(Theme::authCardStyle());

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(38, 32, 38, 32);
    cardLayout->setSpacing(9);


    QLabel *lblTitle = new QLabel("Đăng Ký Tài Khoản", card);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("color: #1E293B; font-size: 24px; font-weight: 600; background: transparent; border: none;");

    QFrame *divider = new QFrame(card);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("background-color: #E5E7EB; max-height: 1px; border: none;");

    auto makeFieldLabel = [card](const QString& text) {
        QLabel* lbl = new QLabel(text, card);
        lbl->setStyleSheet("color: #64748B; font-size: 12px; font-weight: 600; background: transparent; border: none;");
        return lbl;
    };

    QString inputStyle = R"(
        QLineEdit, QSpinBox {
            background-color: #F9FAFB;
            color: #1E293B;
            font-size: 14px;
            border: 1.5px solid #E5E7EB;
            border-radius: 6px;
            padding: 0 14px;
        }
        QLineEdit:hover, QSpinBox:hover {
            border-color: #CBD5E1;
            background-color: #FFFFFF;
        }
        QLineEdit:focus, QSpinBox:focus {
            border: 1.5px solid #E05D5D;
            background-color: #FFFFFF;
        }
    )";

    txtTaiKhoan = new QLineEdit(card);
    txtTaiKhoan->setPlaceholderText("Tên đăng nhập");
    txtTaiKhoan->setFixedHeight(44);
    txtTaiKhoan->setStyleSheet(inputStyle);

    txtMatKhau = new QLineEdit(card);
    txtMatKhau->setPlaceholderText("Mật khẩu");
    txtMatKhau->setEchoMode(QLineEdit::Password);
    txtMatKhau->setFixedHeight(44);
    txtMatKhau->setStyleSheet(inputStyle);

    txtXacNhanMatKhau = new QLineEdit(card);
    txtXacNhanMatKhau->setPlaceholderText("Nhập lại mật khẩu");
    txtXacNhanMatKhau->setEchoMode(QLineEdit::Password);
    txtXacNhanMatKhau->setFixedHeight(44);
    txtXacNhanMatKhau->setStyleSheet(inputStyle);

    QHBoxLayout *phoneAgeRow = new QHBoxLayout();
    phoneAgeRow->setSpacing(12);

    QVBoxLayout *phoneCol = new QVBoxLayout();
    phoneCol->setSpacing(5);
    txtSoDienThoai = new QLineEdit(card);
    txtSoDienThoai->setPlaceholderText("Số điện thoại");
    txtSoDienThoai->setFixedHeight(44);
    txtSoDienThoai->setStyleSheet(inputStyle);
    phoneCol->addWidget(makeFieldLabel("Số điện thoại"));
    phoneCol->addWidget(txtSoDienThoai);

    QVBoxLayout *ageCol = new QVBoxLayout();
    ageCol->setSpacing(5);
    spinTuoi = new QSpinBox(card);
    spinTuoi->setRange(1, 120);
    spinTuoi->setValue(18);
    spinTuoi->setSuffix(" tuổi");
    spinTuoi->setFixedHeight(44);
    spinTuoi->setFixedWidth(124);
    spinTuoi->setStyleSheet(inputStyle);
    ageCol->addWidget(makeFieldLabel("Độ tuổi"));
    ageCol->addWidget(spinTuoi);

    phoneAgeRow->addLayout(phoneCol, 1);
    phoneAgeRow->addLayout(ageCol, 0);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);

    btnBack = new QPushButton("Quay Lại", card);
    btnBack->setFixedHeight(46);
    btnBack->setCursor(Qt::PointingHandCursor);
    btnBack->setStyleSheet(Theme::secondaryButtonStyle(46, 14));

    btnRegister = new QPushButton("Tạo Tài Khoản", card);
    btnRegister->setFixedHeight(46);
    btnRegister->setCursor(Qt::PointingHandCursor);
    btnRegister->setStyleSheet(Theme::primaryButtonStyle(46, 14));

    btnLayout->addWidget(btnBack, 1);
    btnLayout->addWidget(btnRegister, 2);

    cardLayout->addWidget(lblTitle);
    cardLayout->addSpacing(4);
    cardLayout->addWidget(divider);
    cardLayout->addSpacing(6);
    cardLayout->addWidget(makeFieldLabel("Tài khoản"));
    cardLayout->addWidget(txtTaiKhoan);
    cardLayout->addWidget(makeFieldLabel("Mật khẩu"));
    cardLayout->addWidget(txtMatKhau);
    cardLayout->addWidget(makeFieldLabel("Xác nhận mật khẩu"));
    cardLayout->addWidget(txtXacNhanMatKhau);
    cardLayout->addLayout(phoneAgeRow);
    cardLayout->addSpacing(12);
    cardLayout->addLayout(btnLayout);

    rootLayout->addWidget(card, 0, Qt::AlignCenter);

    connect(btnRegister, &QPushButton::clicked, this, &RegisterWindow::onRegister);
    connect(btnBack, &QPushButton::clicked, this, &RegisterWindow::onBack);
    connect(txtSoDienThoai, &QLineEdit::returnPressed, this, &RegisterWindow::onRegister);
}

void RegisterWindow::resetFields() {
    txtTaiKhoan->clear();
    txtMatKhau->clear();
    txtXacNhanMatKhau->clear();
    txtSoDienThoai->clear();
    spinTuoi->setValue(18);
    txtTaiKhoan->setFocus();
}

void RegisterWindow::onRegister() {
    std::string tk = txtTaiKhoan->text().trimmed().toStdString();
    std::string mk = txtMatKhau->text().toStdString();
    std::string xnmk = txtXacNhanMatKhau->text().toStdString();
    std::string sdt = txtSoDienThoai->text().trimmed().toStdString();
    int tuoi = spinTuoi->value();

    if (tk.empty() || mk.empty() || sdt.empty()) {
        QMessageBox::warning(this, "Thông Báo", "Vui lòng nhập đầy đủ thông tin!");
        return;
    }

    if (mk != xnmk) {
        QMessageBox::warning(this, "Thông Báo", "Mật khẩu xác nhận không khớp!");
        return;
    }

    if (userManager->dangKy(tk, mk, sdt, tuoi)) {
        QMessageBox::information(this, "Thành Công", "Đăng ký tài khoản thành công!");
        emit registerSuccess();
    } else {
        QMessageBox::critical(this, "Lỗi", "Tài khoản đã tồn tại!");
    }
}

void RegisterWindow::onBack() {
    emit goBack();
}
