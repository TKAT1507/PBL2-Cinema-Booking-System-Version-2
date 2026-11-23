#include "ChangePasswordWindow.h"
#include "../common/Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QFrame>

ChangePasswordWindow::ChangePasswordWindow(UserManager *userManager, QWidget *parent)
    : QWidget(parent), userManager(userManager)
{
    setObjectName("changePwdPage");
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet(R"(
        QWidget#changePwdPage {
            background: #F5F6F8;
        }
    )");

    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setAlignment(Qt::AlignCenter);
    rootLayout->setContentsMargins(24, 24, 24, 24);

    QFrame *card = new QFrame(this);
    card->setObjectName("authCard");
    card->setFixedWidth(460);
    card->setStyleSheet(Theme::authCardStyle());

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(38, 32, 38, 32);
    cardLayout->setSpacing(10);


    QLabel *lblTitle = new QLabel("Đổi Mật Khẩu", card);
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
        QLineEdit {
            background-color: #F9FAFB;
            color: #1E293B;
            font-size: 14px;
            border: 1.5px solid #E5E7EB;
            border-radius: 6px;
            padding: 0 14px;
        }
        QLineEdit:hover {
            border-color: #CBD5E1;
            background-color: #FFFFFF;
        }
        QLineEdit:focus {
            border: 1.5px solid #E05D5D;
            background-color: #FFFFFF;
        }
    )";

    txtTaiKhoan = new QLineEdit(card);
    txtTaiKhoan->setPlaceholderText("Tài khoản");
    txtTaiKhoan->setFixedHeight(44);
    txtTaiKhoan->setStyleSheet(inputStyle);

    txtMatKhauCu = new QLineEdit(card);
    txtMatKhauCu->setPlaceholderText("Mật khẩu cũ");
    txtMatKhauCu->setEchoMode(QLineEdit::Password);
    txtMatKhauCu->setFixedHeight(44);
    txtMatKhauCu->setStyleSheet(inputStyle);

    txtMatKhauMoi = new QLineEdit(card);
    txtMatKhauMoi->setPlaceholderText("Mật khẩu mới");
    txtMatKhauMoi->setEchoMode(QLineEdit::Password);
    txtMatKhauMoi->setFixedHeight(44);
    txtMatKhauMoi->setStyleSheet(inputStyle);

    txtXacNhanMatKhauMoi = new QLineEdit(card);
    txtXacNhanMatKhauMoi->setPlaceholderText("Xác nhận mật khẩu mới");
    txtXacNhanMatKhauMoi->setEchoMode(QLineEdit::Password);
    txtXacNhanMatKhauMoi->setFixedHeight(44);
    txtXacNhanMatKhauMoi->setStyleSheet(inputStyle);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);

    btnBack = new QPushButton("Quay Lại", card);
    btnBack->setFixedHeight(46);
    btnBack->setCursor(Qt::PointingHandCursor);
    btnBack->setStyleSheet(Theme::secondaryButtonStyle(46, 14));

    btnChange = new QPushButton("Xác Nhận", card);
    btnChange->setFixedHeight(46);
    btnChange->setCursor(Qt::PointingHandCursor);
    btnChange->setStyleSheet(Theme::primaryButtonStyle(46, 14));

    btnLayout->addWidget(btnBack, 1);
    btnLayout->addWidget(btnChange, 2);

    cardLayout->addWidget(lblTitle);
    cardLayout->addSpacing(4);
    cardLayout->addWidget(divider);
    cardLayout->addSpacing(6);
    cardLayout->addWidget(makeFieldLabel("Tài khoản"));
    cardLayout->addWidget(txtTaiKhoan);
    cardLayout->addWidget(makeFieldLabel("Mật khẩu cũ"));
    cardLayout->addWidget(txtMatKhauCu);
    cardLayout->addWidget(makeFieldLabel("Mật khẩu mới"));
    cardLayout->addWidget(txtMatKhauMoi);
    cardLayout->addWidget(makeFieldLabel("Xác nhận mật khẩu mới"));
    cardLayout->addWidget(txtXacNhanMatKhauMoi);
    cardLayout->addSpacing(12);
    cardLayout->addLayout(btnLayout);

    rootLayout->addWidget(card, 0, Qt::AlignCenter);

    connect(btnChange, &QPushButton::clicked, this, &ChangePasswordWindow::onChangePassword);
    connect(btnBack, &QPushButton::clicked, this, &ChangePasswordWindow::onBack);
    connect(txtXacNhanMatKhauMoi, &QLineEdit::returnPressed, this, &ChangePasswordWindow::onChangePassword);
}

void ChangePasswordWindow::onChangePassword() {
    std::string tk = txtTaiKhoan->text().trimmed().toStdString();
    std::string mkCu = txtMatKhauCu->text().toStdString();
    std::string mkMoi = txtMatKhauMoi->text().toStdString();
    std::string xnMk = txtXacNhanMatKhauMoi->text().toStdString();

    if (tk.empty() || mkCu.empty() || mkMoi.empty()) {
        QMessageBox::warning(this, "Thông Báo", "Vui lòng nhập đầy đủ thông tin!");
        return;
    }

    if (mkMoi != xnMk) {
        QMessageBox::warning(this, "Thông Báo", "Mật khẩu xác nhận không khớp!");
        return;
    }

    if (userManager->doiMatKhau(tk, mkCu, mkMoi)) {
        QMessageBox::information(this, "Thành Công", "Đổi mật khẩu thành công!");
        emit goBack();
    } else {
        QMessageBox::critical(this, "Lỗi", "Tài khoản hoặc mật khẩu cũ không đúng!");
    }
}

void ChangePasswordWindow::onBack() {
    emit goBack();
}
