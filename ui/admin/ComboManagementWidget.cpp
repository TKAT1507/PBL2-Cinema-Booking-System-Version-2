#include "ComboManagementWidget.h"
#include "../common/Theme.h"
#include "managers/ComboManager.h"
#include "models/Combo.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>

ComboManagementWidget::ComboManagementWidget(ComboManager* comboMgr, QWidget* parent)
    : QWidget(parent), comboManager(comboMgr)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(26, 22, 26, 20);
    layout->setSpacing(14);

    QHBoxLayout* topRow = new QHBoxLayout();
    QVBoxLayout* titleCol = new QVBoxLayout();
    titleCol->setSpacing(2);
    QLabel* lblEyebrow = new QLabel("DANH MỤC DỊCH VỤ QUẦY BẮP NƯỚC", this);
    lblEyebrow->setStyleSheet(Theme::sectionEyebrowStyle());
    QLabel* lblTitle = new QLabel("QUẢN LÝ COMBO BẮP NƯỚC", this);
    lblTitle->setStyleSheet(Theme::sectionTitleStyle());
    titleCol->addWidget(lblEyebrow);
    titleCol->addWidget(lblTitle);

    topRow->addLayout(titleCol);
    topRow->addStretch();

    btnAdd = new QPushButton("+ Thêm Combo", this);
    btnAdd->setCursor(Qt::PointingHandCursor);
    btnAdd->setStyleSheet(Theme::primaryButtonStyle(38, 13));

    btnEdit = new QPushButton("Sửa Combo", this);
    btnEdit->setCursor(Qt::PointingHandCursor);
    btnEdit->setStyleSheet(Theme::goldOutlineButtonStyle(38, 13));

    btnDelete = new QPushButton("Ngừng Bán", this);
    btnDelete->setCursor(Qt::PointingHandCursor);
    btnDelete->setStyleSheet(Theme::dangerOutlineButtonStyle(38, 13));

    topRow->addWidget(btnAdd, 0, Qt::AlignVCenter);
    topRow->addWidget(btnEdit, 0, Qt::AlignVCenter);
    topRow->addWidget(btnDelete, 0, Qt::AlignVCenter);
    layout->addLayout(topRow);

    table = new QTableWidget(0, 5, this);
    table->setHorizontalHeaderLabels({"Mã Combo", "Tên Combo", "Mô Tả", "Đơn Giá", "Trạng Thái"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setDefaultSectionSize(46);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    layout->addWidget(table, 1);

    connect(btnAdd, &QPushButton::clicked, this, &ComboManagementWidget::addCombo);
    connect(btnEdit, &QPushButton::clicked, this, &ComboManagementWidget::editCombo);
    connect(btnDelete, &QPushButton::clicked, this, &ComboManagementWidget::deleteCombo);

    loadData();
}

void ComboManagementWidget::loadData() {
    table->setRowCount(0);
    auto combos = comboManager->getAllCombos();
    QFont boldFont = table->font();
    boldFont.setBold(true);

    for (int i = 0; i < combos.size(); ++i) {
        table->insertRow(i);

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::fromStdString(combos[i].getMaCombo()));
        idItem->setTextAlignment(Qt::AlignCenter);
        idItem->setFont(boldFont);
        idItem->setForeground(QBrush(QColor("#8C92FF")));
        table->setItem(i, 0, idItem);

        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(combos[i].getTenCombo()));
        nameItem->setFont(boldFont);
        table->setItem(i, 1, nameItem);

        QTableWidgetItem* descItem = new QTableWidgetItem(QString::fromStdString(combos[i].getMoTa()));
        descItem->setForeground(QBrush(QColor("#A3ABC9")));
        table->setItem(i, 2, descItem);

        QTableWidgetItem* priceItem = new QTableWidgetItem(QString("%L1 VND").arg(combos[i].getGia(), 0, 'f', 0));
        priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        priceItem->setFont(boldFont);
        priceItem->setForeground(QBrush(QColor("#FFD166")));
        table->setItem(i, 3, priceItem);

        bool active = combos[i].getTrangThai();
        QTableWidgetItem* statusItem = new QTableWidgetItem(active ? "Đang bán" : "Ngừng bán");
        statusItem->setTextAlignment(Qt::AlignCenter);
        statusItem->setFont(boldFont);
        statusItem->setForeground(QBrush(QColor(active ? "#34D399" : "#707899")));
        table->setItem(i, 4, statusItem);
    }
}

void ComboManagementWidget::addCombo() {
    QDialog dialog(this);
    dialog.setWindowTitle("Thêm Combo Mới");
    dialog.setFixedWidth(450);
    dialog.setStyleSheet("QDialog { background-color: #1F2338; border: 1px solid #353C61; border-radius: 18px; }");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(26, 24, 26, 24);
    layout->setSpacing(10);

    QLabel *dlgTitle = new QLabel("Thêm Combo Bắp Nước", &dialog);
    dlgTitle->setStyleSheet("color: #FFD166; font-size: 18px; font-weight: bold;");
    layout->addWidget(dlgTitle);

    auto lbl = [&dialog](const QString& t) {
        QLabel* l = new QLabel(t, &dialog);
        l->setStyleSheet("color: #A3ABC9; font-size: 12px; font-weight: 600;");
        return l;
    };

    QLineEdit *edtTen = new QLineEdit(&dialog); edtTen->setPlaceholderText("Ví dụ: Combo Couple Bắp + 2 Nước");
    QLineEdit *edtMoTa = new QLineEdit(&dialog); edtMoTa->setPlaceholderText("Mô tả chi tiết thành phần combo");
    QDoubleSpinBox *spnGia = new QDoubleSpinBox(&dialog);
    spnGia->setRange(0, 1000000);
    spnGia->setValue(50000);
    spnGia->setDecimals(0);
    spnGia->setSuffix(" VND");
    spnGia->setSingleStep(5000);

    layout->addWidget(lbl("Tên combo:")); layout->addWidget(edtTen);
    layout->addWidget(lbl("Mô tả:")); layout->addWidget(edtMoTa);
    layout->addWidget(lbl("Đơn giá:")); layout->addWidget(spnGia);
    layout->addSpacing(8);

    QPushButton *btnCancel = new QPushButton("Hủy Bỏ", &dialog);
    btnCancel->setCursor(Qt::PointingHandCursor);
    btnCancel->setStyleSheet(Theme::secondaryButtonStyle(40, 13));

    QPushButton *btnOk = new QPushButton("Thêm Combo", &dialog);
    btnOk->setCursor(Qt::PointingHandCursor);
    btnOk->setStyleSheet(Theme::primaryButtonStyle(40, 13));

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);
    btnLayout->addWidget(btnCancel);
    btnLayout->addWidget(btnOk);
    layout->addLayout(btnLayout);

    connect(btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted && !edtTen->text().trimmed().isEmpty()) {
        Combo c("", edtTen->text().trimmed().toStdString(), edtMoTa->text().trimmed().toStdString(), spnGia->value(), true);
        if (comboManager->addCombo(c)) {
            comboManager->save();
            loadData();
        }
    }
}

void ComboManagementWidget::editCombo() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Cảnh Báo", "Vui lòng chọn combo cần sửa!");
        return;
    }
    std::string id = table->item(row, 0)->text().toStdString();
    Combo* c = comboManager->findById(id);
    if (!c) return;

    QDialog dialog(this);
    dialog.setWindowTitle("Sửa Combo");
    dialog.setFixedWidth(450);
    dialog.setStyleSheet("QDialog { background-color: #1F2338; border: 1px solid #353C61; border-radius: 18px; }");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(26, 24, 26, 24);
    layout->setSpacing(10);

    QLabel *dlgTitle = new QLabel("Cập Nhật Combo", &dialog);
    dlgTitle->setStyleSheet("color: #FFD166; font-size: 18px; font-weight: bold;");
    layout->addWidget(dlgTitle);

    auto lbl = [&dialog](const QString& t) {
        QLabel* l = new QLabel(t, &dialog);
        l->setStyleSheet("color: #A3ABC9; font-size: 12px; font-weight: 600;");
        return l;
    };

    QLineEdit *edtTen = new QLineEdit(QString::fromStdString(c->getTenCombo()), &dialog);
    QLineEdit *edtMoTa = new QLineEdit(QString::fromStdString(c->getMoTa()), &dialog);
    QDoubleSpinBox *spnGia = new QDoubleSpinBox(&dialog);
    spnGia->setRange(0, 1000000);
    spnGia->setValue(c->getGia());
    spnGia->setDecimals(0);
    spnGia->setSuffix(" VND");
    spnGia->setSingleStep(5000);

    layout->addWidget(lbl("Tên combo:")); layout->addWidget(edtTen);
    layout->addWidget(lbl("Mô tả:")); layout->addWidget(edtMoTa);
    layout->addWidget(lbl("Đơn giá:")); layout->addWidget(spnGia);
    layout->addSpacing(8);

    QPushButton *btnCancel = new QPushButton("Hủy Bỏ", &dialog);
    btnCancel->setCursor(Qt::PointingHandCursor);
    btnCancel->setStyleSheet(Theme::secondaryButtonStyle(40, 13));

    QPushButton *btnOk = new QPushButton("Lưu Thay Đổi", &dialog);
    btnOk->setCursor(Qt::PointingHandCursor);
    btnOk->setStyleSheet(Theme::primaryButtonStyle(40, 13));

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);
    btnLayout->addWidget(btnCancel);
    btnLayout->addWidget(btnOk);
    layout->addLayout(btnLayout);

    connect(btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted && !edtTen->text().trimmed().isEmpty()) {
        c->setTenCombo(edtTen->text().trimmed().toStdString());
        c->setMoTa(edtMoTa->text().trimmed().toStdString());
        c->setGia(spnGia->value());
        comboManager->updateCombo(id, *c);
        comboManager->save();
        loadData();
    }
}

void ComboManagementWidget::deleteCombo() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Cảnh Báo", "Vui lòng chọn combo cần ngừng bán!");
        return;
    }
    std::string id = table->item(row, 0)->text().toStdString();
    if (QMessageBox::question(this, "Xác Nhận", "Bạn có muốn ngừng bán combo này?") == QMessageBox::Yes) {
        if (comboManager->deactivateCombo(id)) {
            comboManager->save();
            loadData();
        }
    }
}
