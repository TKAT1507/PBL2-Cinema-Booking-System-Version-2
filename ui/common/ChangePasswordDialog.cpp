#include "ChangePasswordDialog.h"
#include "Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QFrame>

ChangePasswordDialog::ChangePasswordDialog(UserManager *userManager, const std::string& taiKhoan, QWidget *parent)
    : QDialog(parent), userManager(userManager), taiKhoan(taiKhoan)
{
    setWindowTitle("Đổi Mật Khẩu");
    setFixedSize(440, 450);
    setStyleSheet(R"(
        QDialog {
            background-color: #1F2338;
            border: 1px solid #353C61;
            border-radius: 18px;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(32, 28, 32, 28);

    QLabel *lblEyebrow = new QLabel("BẢO MẬT TÀI KHOẢN", this);
    lblEyebrow->setAlignment(Qt::AlignCenter);
    lblEyebrow->setStyleSheet(Theme::sectionEyebrowStyle());

    QLabel *lblTitle = new QLabel("Đổi Mật Khẩu", this);
    lblTitle->setStyleSheet("color: #F3F4FA; font-size: 22px; font-weight: bold;");
    lblTitle->setAlignment(Qt::AlignCenter);

    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #303654; max-height: 1px; border: none;");

    auto makeFieldLabel = [this](const QString& text) {
        QLabel* lbl = new QLabel(text, this);
        lbl->setStyleSheet("color: #A3ABC9; font-size: 12px; font-weight: 600;");
        return lbl;
    };

    txtTaiKhoan = new QLineEdit(this);
    txtTaiKhoan->setText(QString::fromStdString(taiKhoan));
    txtTaiKhoan->setPlaceholderText("Nhập tên tài khoản");
    txtTaiKhoan->setMinimumHeight(40);
    if (!taiKhoan.empty()) {
        txtTaiKhoan->setReadOnly(true);
    }

    txtMatKhauCu = new QLineEdit(this);
    txtMatKhauCu->setPlaceholderText("Nhập mật khẩu hiện tại");
    txtMatKhauCu->setEchoMode(QLineEdit::Password);
    txtMatKhauCu->setMinimumHeight(40);

    txtMatKhauMoi = new QLineEdit(this);
    txtMatKhauMoi->setPlaceholderText("Nhập mật khẩu mới");
    txtMatKhauMoi->setEchoMode(QLineEdit::Password);
    txtMatKhauMoi->setMinimumHeight(40);

    txtXacNhanMatKhauMoi = new QLineEdit(this);
    txtXacNhanMatKhauMoi->setPlaceholderText("Nhập lại mật khẩu mới");
    txtXacNhanMatKhauMoi->setEchoMode(QLineEdit::Password);
    txtXacNhanMatKhauMoi->setMinimumHeight(40);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);

    btnCancel = new QPushButton("Hủy Bỏ", this);
    btnCancel->setCursor(Qt::PointingHandCursor);
    btnCancel->setStyleSheet(Theme::secondaryButtonStyle(42, 13));

    btnConfirm = new QPushButton("Xác Nhận Đổi", this);
    btnConfirm->setCursor(Qt::PointingHandCursor);
    btnConfirm->setStyleSheet(Theme::primaryButtonStyle(42, 13));

    btnLayout->addWidget(btnCancel);
    btnLayout->addWidget(btnConfirm);

    layout->addWidget(lblEyebrow);
    layout->addWidget(lblTitle);
    layout->addSpacing(4);
    layout->addWidget(line);
    layout->addSpacing(4);
    layout->addWidget(makeFieldLabel("Tài khoản"));
    layout->addWidget(txtTaiKhoan);
    layout->addWidget(makeFieldLabel("Mật khẩu hiện tại"));
    layout->addWidget(txtMatKhauCu);
    layout->addWidget(makeFieldLabel("Mật khẩu mới"));
    layout->addWidget(txtMatKhauMoi);
    layout->addWidget(makeFieldLabel("Xác nhận mật khẩu mới"));
    layout->addWidget(txtXacNhanMatKhauMoi);
    layout->addSpacing(10);
    layout->addLayout(btnLayout);

    connect(btnConfirm, &QPushButton::clicked, this, &ChangePasswordDialog::onChange);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(txtXacNhanMatKhauMoi, &QLineEdit::returnPressed, this, &ChangePasswordDialog::onChange);
}

void ChangePasswordDialog::onChange() {
    std::string tk = txtTaiKhoan->text().trimmed().toStdString();
    std::string mkCu = txtMatKhauCu->text().toStdString();
    std::string mkMoi = txtMatKhauMoi->text().toStdString();
    std::string xacNhan = txtXacNhanMatKhauMoi->text().toStdString();

    if (tk.empty() || mkCu.empty() || mkMoi.empty()) {
        QMessageBox::warning(this, "Thông Báo", "Vui lòng điền đầy đủ các thông tin!");
        return;
    }

    if (mkMoi != xacNhan) {
        QMessageBox::warning(this, "Lỗi", "Mật khẩu xác nhận không khớp!");
        return;
    }

    if (userManager->doiMatKhau(tk, mkCu, mkMoi)) {
        userManager->saveAll();
        QMessageBox::information(this, "Thành Công", "Đổi mật khẩu thành công!");
        accept();
    } else {
        QMessageBox::critical(this, "Lỗi", "Tài khoản hoặc mật khẩu hiện tại không chính xác!");
    }
}
