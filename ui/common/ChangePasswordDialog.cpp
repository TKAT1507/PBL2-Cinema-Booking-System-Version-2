#include "ChangePasswordDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QFrame>

ChangePasswordDialog::ChangePasswordDialog(UserManager *userManager, const std::string& taiKhoan, QWidget *parent)
    : QDialog(parent), userManager(userManager), taiKhoan(taiKhoan)
{
    setWindowTitle("Đổi Mật Khẩu");
    setFixedSize(420, 400);
    setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2C2E6A, stop:1 #1E1F4B);
            border: 2px solid #E6B042;
            border-radius: 10px;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(12);
    layout->setContentsMargins(30, 25, 30, 25);

    QLabel *lblTitle = new QLabel("ĐỔI MẬT KHẨU", this);
    lblTitle->setStyleSheet("color: #F5C542; font-size: 20px; font-weight: bold;");
    lblTitle->setAlignment(Qt::AlignCenter);

    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #FFFFFF; max-height: 1px; border: none;");

    QString inputStyle = R"(
        QLineEdit {
            background-color: #E6B042;
            color: #1A1B41;
            font-size: 13px;
            font-weight: bold;
            border: 1px solid #C49129;
            border-radius: 5px;
            padding: 0 12px;
            min-height: 36px;
        }
        QLineEdit:focus {
            border: 2px solid #FFFFFF;
        }
    )";

    txtTaiKhoan = new QLineEdit(this);
    txtTaiKhoan->setText(QString::fromStdString(taiKhoan));
    txtTaiKhoan->setPlaceholderText("Tài khoản");
    txtTaiKhoan->setStyleSheet(inputStyle);
    if (!taiKhoan.empty()) {
        txtTaiKhoan->setReadOnly(true);
    }

    txtMatKhauCu = new QLineEdit(this);
    txtMatKhauCu->setPlaceholderText("Mật khẩu hiện tại");
    txtMatKhauCu->setEchoMode(QLineEdit::Password);
    txtMatKhauCu->setStyleSheet(inputStyle);

    txtMatKhauMoi = new QLineEdit(this);
    txtMatKhauMoi->setPlaceholderText("Mật khẩu mới");
    txtMatKhauMoi->setEchoMode(QLineEdit::Password);
    txtMatKhauMoi->setStyleSheet(inputStyle);

    txtXacNhanMatKhauMoi = new QLineEdit(this);
    txtXacNhanMatKhauMoi->setPlaceholderText("Xác nhận mật khẩu mới");
    txtXacNhanMatKhauMoi->setEchoMode(QLineEdit::Password);
    txtXacNhanMatKhauMoi->setStyleSheet(inputStyle);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);

    btnConfirm = new QPushButton("Xác Nhận", this);
    btnConfirm->setCursor(Qt::PointingHandCursor);
    btnConfirm->setStyleSheet(R"(
        QPushButton {
            background-color: #28A745;
            color: #FFFFFF;
            font-size: 14px;
            font-weight: bold;
            border-radius: 5px;
            border: none;
            min-height: 38px;
        }
        QPushButton:hover {
            background-color: #34C759;
        }
    )");

    btnCancel = new QPushButton("Hủy", this);
    btnCancel->setCursor(Qt::PointingHandCursor);
    btnCancel->setStyleSheet(R"(
        QPushButton {
            background-color: #9E221E;
            color: #FFFFFF;
            font-size: 14px;
            font-weight: bold;
            border-radius: 5px;
            border: none;
            min-height: 38px;
        }
        QPushButton:hover {
            background-color: #B52A26;
        }
    )");

    btnLayout->addWidget(btnConfirm);
    btnLayout->addWidget(btnCancel);

    layout->addWidget(lblTitle);
    layout->addWidget(line);
    layout->addSpacing(5);
    layout->addWidget(txtTaiKhoan);
    layout->addWidget(txtMatKhauCu);
    layout->addWidget(txtMatKhauMoi);
    layout->addWidget(txtXacNhanMatKhauMoi);
    layout->addSpacing(8);
    layout->addLayout(btnLayout);

    connect(btnConfirm, &QPushButton::clicked, this, &ChangePasswordDialog::onChange);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

void ChangePasswordDialog::onChange() {
    std::string tk = txtTaiKhoan->text().trimmed().toStdString();
    std::string mkCu = txtMatKhauCu->text().toStdString();
    std::string mkMoi = txtMatKhauMoi->text().toStdString();
    std::string xacNhan = txtXacNhanMatKhauMoi->text().toStdString();

    if (tk.empty() || mkCu.empty() || mkMoi.empty()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng điền đầy đủ các thông tin!");
        return;
    }

    if (mkMoi != xacNhan) {
        QMessageBox::warning(this, "Lỗi", "Mật khẩu xác nhận không khớp!");
        return;
    }

    if (userManager->doiMatKhau(tk, mkCu, mkMoi)) {
        userManager->saveAll();
        QMessageBox::information(this, "Thành công", "Đổi mật khẩu thành công!");
        accept();
    } else {
        QMessageBox::critical(this, "Lỗi", "Tài khoản hoặc mật khẩu cũ không chính xác!");
    }
}
