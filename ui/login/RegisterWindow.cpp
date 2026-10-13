#include "RegisterWindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QLabel>
#include <QFont>

RegisterWindow::RegisterWindow(UserManager *userManager, QWidget *parent)
    : QWidget(parent), userManager(userManager)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    QLabel *lblTitle = new QLabel("DANG KY", this);
    QFont font = lblTitle->font();
    font.setPointSize(24);
    font.setBold(true);
    lblTitle->setFont(font);
    lblTitle->setAlignment(Qt::AlignCenter);

    QFormLayout *formLayout = new QFormLayout();
    
    txtTaiKhoan = new QLineEdit(this);
    txtMatKhau = new QLineEdit(this);
    txtMatKhau->setEchoMode(QLineEdit::Password);
    txtXacNhanMatKhau = new QLineEdit(this);
    txtXacNhanMatKhau->setEchoMode(QLineEdit::Password);
    txtSoDienThoai = new QLineEdit(this);
    spinTuoi = new QSpinBox(this);
    spinTuoi->setRange(1, 120);
    spinTuoi->setValue(18);

    formLayout->addRow("Tai Khoan:", txtTaiKhoan);
    formLayout->addRow("Mat Khau:", txtMatKhau);
    formLayout->addRow("Xac Nhan MK:", txtXacNhanMatKhau);
    formLayout->addRow("So Dien Thoai:", txtSoDienThoai);
    formLayout->addRow("Tuoi:", spinTuoi);

    btnRegister = new QPushButton("Dang Ky", this);
    btnBack = new QPushButton("Quay Lai", this);

    mainLayout->addWidget(lblTitle);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(btnRegister);
    mainLayout->addWidget(btnBack);

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
