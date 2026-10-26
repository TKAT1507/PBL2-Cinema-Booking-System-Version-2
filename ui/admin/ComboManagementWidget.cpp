#include "ComboManagementWidget.h"
#include "managers/ComboManager.h"
#include "models/Combo.h"
#include "utils/IDGenerator.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>

ComboManagementWidget::ComboManagementWidget(ComboManager* comboMgr, QWidget* parent)
    : QWidget(parent), comboManager(comboMgr) {
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(12);

    QLabel *lblTitle = new QLabel("QUẢN LÝ COMBO BẮP NƯỚC", this);
    lblTitle->setStyleSheet("color: #F5C542; font-size: 18px; font-weight: bold;");
    layout->addWidget(lblTitle);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);
    btnAdd = new QPushButton("Thêm Combo", this);
    btnAdd->setCursor(Qt::PointingHandCursor);
    btnAdd->setStyleSheet("background-color: #28A745; color: white; font-weight: bold; min-height: 34px; border-radius: 5px;");

    btnEdit = new QPushButton("Sửa Combo", this);
    btnEdit->setCursor(Qt::PointingHandCursor);
    btnEdit->setStyleSheet("background-color: #E6B042; color: #1A1B41; font-weight: bold; min-height: 34px; border-radius: 5px;");

    btnDelete = new QPushButton("Ngừng Bán", this);
    btnDelete->setCursor(Qt::PointingHandCursor);
    btnDelete->setStyleSheet("background-color: #9E221E; color: white; font-weight: bold; min-height: 34px; border-radius: 5px;");

    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnEdit);
    btnLayout->addWidget(btnDelete);
    btnLayout->addStretch();
    
    table = new QTableWidget(0, 5, this);
    table->setHorizontalHeaderLabels({"Ma Combo", "Ten Combo", "Mo Ta", "Gia", "Trang Thai"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    layout->addLayout(btnLayout);
    layout->addWidget(table);
    
    connect(btnAdd, &QPushButton::clicked, this, &ComboManagementWidget::addCombo);
    connect(btnEdit, &QPushButton::clicked, this, &ComboManagementWidget::editCombo);
    connect(btnDelete, &QPushButton::clicked, this, &ComboManagementWidget::deleteCombo);
    
    loadData();
}

void ComboManagementWidget::loadData() {
    table->setRowCount(0);
    auto combos = comboManager->getAllCombos();
    for(int i = 0; i < combos.size(); ++i) {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(combos[i].getMaCombo())));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(combos[i].getTenCombo())));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(combos[i].getMoTa())));
        
        QTableWidgetItem* priceItem = new QTableWidgetItem(QString::number(combos[i].getGia(), 'f', 0) + " VND");
        priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        table->setItem(i, 3, priceItem);
        
        QString status = combos[i].getTrangThai() ? "Dang Ban" : "Ngung Ban";
        QTableWidgetItem* statusItem = new QTableWidgetItem(status);
        if(!combos[i].getTrangThai()) statusItem->setBackground(Qt::gray);
        table->setItem(i, 4, statusItem);
    }
}

void ComboManagementWidget::addCombo() {
    QDialog dialog(this);
    dialog.setWindowTitle("Them Combo");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QLineEdit *edtTen = new QLineEdit(&dialog); edtTen->setPlaceholderText("Ten combo");
    QLineEdit *edtMoTa = new QLineEdit(&dialog); edtMoTa->setPlaceholderText("Mo ta combo");
    QDoubleSpinBox *spnGia = new QDoubleSpinBox(&dialog); 
    spnGia->setRange(0, 1000000); spnGia->setValue(50000); spnGia->setSuffix(" VND"); spnGia->setSingleStep(5000);
    
    layout->addWidget(new QLabel("Ten combo:", &dialog)); layout->addWidget(edtTen);
    layout->addWidget(new QLabel("Mo ta:", &dialog)); layout->addWidget(edtMoTa);
    layout->addWidget(new QLabel("Gia:", &dialog)); layout->addWidget(spnGia);
    
    QPushButton *btnOk = new QPushButton("Them", &dialog);
    QPushButton *btnCancel = new QPushButton("Huy", &dialog);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnOk); btnLayout->addWidget(btnCancel);
    layout->addLayout(btnLayout);
    connect(btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted && !edtTen->text().isEmpty()) {
        Combo c("", edtTen->text().toStdString(), edtMoTa->text().toStdString(), spnGia->value(), true);
        if(comboManager->addCombo(c)) {
            comboManager->save();
            loadData();
        }
    }
}

void ComboManagementWidget::editCombo() {
    int row = table->currentRow();
    if(row < 0) {
        QMessageBox::warning(this, "Canh bao", "Vui long chon combo can sua!");
        return;
    }
    std::string id = table->item(row, 0)->text().toStdString();
    Combo* c = comboManager->findById(id);
    if(!c) return;
    
    QDialog dialog(this);
    dialog.setWindowTitle("Sua Combo");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QLineEdit *edtTen = new QLineEdit(QString::fromStdString(c->getTenCombo()), &dialog);
    QLineEdit *edtMoTa = new QLineEdit(QString::fromStdString(c->getMoTa()), &dialog);
    QDoubleSpinBox *spnGia = new QDoubleSpinBox(&dialog);
    spnGia->setRange(0, 1000000); spnGia->setValue(c->getGia()); spnGia->setSuffix(" VND"); spnGia->setSingleStep(5000);
    
    layout->addWidget(new QLabel("Ten combo:", &dialog)); layout->addWidget(edtTen);
    layout->addWidget(new QLabel("Mo ta:", &dialog)); layout->addWidget(edtMoTa);
    layout->addWidget(new QLabel("Gia:", &dialog)); layout->addWidget(spnGia);
    
    QPushButton *btnOk = new QPushButton("Luu", &dialog);
    QPushButton *btnCancel = new QPushButton("Huy", &dialog);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnOk); btnLayout->addWidget(btnCancel);
    layout->addLayout(btnLayout);
    connect(btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted && !edtTen->text().isEmpty()) {
        c->setTenCombo(edtTen->text().toStdString());
        c->setMoTa(edtMoTa->text().toStdString());
        c->setGia(spnGia->value());
        comboManager->updateCombo(id, *c);
        comboManager->save();
        loadData();
    }
}

void ComboManagementWidget::deleteCombo() {
    int row = table->currentRow();
    if(row < 0) {
        QMessageBox::warning(this, "Canh bao", "Vui long chon combo!");
        return;
    }
    std::string id = table->item(row, 0)->text().toStdString();
    if(QMessageBox::question(this, "Xac Nhan", "Ban co muon ngung ban combo nay?") == QMessageBox::Yes) {
        if(comboManager->deactivateCombo(id)) {
            comboManager->save();
            loadData();
        }
    }
}
