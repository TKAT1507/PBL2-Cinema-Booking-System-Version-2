#include "RoomManagementWidget.h"
#include "../common/Theme.h"
#include "managers/RoomManager.h"
#include "models/Room.h"
#include "utils/IDGenerator.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>

RoomManagementWidget::RoomManagementWidget(RoomManager* roomMgr, QWidget* parent)
    : QWidget(parent), roomManager(roomMgr)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(26, 22, 26, 20);
    layout->setSpacing(14);

    QHBoxLayout* topRow = new QHBoxLayout();
    QVBoxLayout* titleCol = new QVBoxLayout();
    titleCol->setSpacing(2);
    QLabel* lblTitle = new QLabel("Quản lý phòng chiếu", this);
    lblTitle->setStyleSheet(Theme::sectionTitleStyle());
    titleCol->addWidget(lblTitle);

    topRow->addLayout(titleCol);
    topRow->addStretch();

    btnAdd = new QPushButton("+ Thêm Phòng", this);
    btnAdd->setCursor(Qt::PointingHandCursor);
    btnAdd->setStyleSheet(Theme::primaryButtonStyle(38, 13));

    btnEdit = new QPushButton("Sửa Phòng", this);
    btnEdit->setCursor(Qt::PointingHandCursor);
    btnEdit->setStyleSheet(Theme::goldOutlineButtonStyle(38, 13));

    btnDelete = new QPushButton("Ngừng Hoạt Động", this);
    btnDelete->setCursor(Qt::PointingHandCursor);
    btnDelete->setStyleSheet(Theme::dangerOutlineButtonStyle(38, 13));

    topRow->addWidget(btnAdd, 0, Qt::AlignVCenter);
    topRow->addWidget(btnEdit, 0, Qt::AlignVCenter);
    topRow->addWidget(btnDelete, 0, Qt::AlignVCenter);
    layout->addLayout(topRow);

    table = new QTableWidget(0, 5, this);
    table->setHorizontalHeaderLabels({"Mã Phòng", "Tên Phòng", "Số Hàng", "Số Cột", "Trạng Thái"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setDefaultSectionSize(46);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    layout->addWidget(table, 1);

    connect(btnAdd, &QPushButton::clicked, this, &RoomManagementWidget::addRoom);
    connect(btnEdit, &QPushButton::clicked, this, &RoomManagementWidget::editRoom);
    connect(btnDelete, &QPushButton::clicked, this, &RoomManagementWidget::deleteRoom);

    loadData();
}

void RoomManagementWidget::loadData() {
    table->setRowCount(0);
    auto& rooms = roomManager->getAllRooms();
    QFont boldFont = table->font();
    boldFont.setBold(true);

    for (int i = 0; i < rooms.size(); ++i) {
        table->insertRow(i);

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::fromStdString(rooms[i].getMaPhong()));
        idItem->setTextAlignment(Qt::AlignCenter);
        idItem->setFont(boldFont);
        idItem->setForeground(QBrush(QColor("#64748B")));
        table->setItem(i, 0, idItem);

        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(rooms[i].getTenPhong()));
        nameItem->setFont(boldFont);
        table->setItem(i, 1, nameItem);

        QTableWidgetItem* rowsItem = new QTableWidgetItem(QString::number(rooms[i].getSoHang()));
        rowsItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 2, rowsItem);

        QTableWidgetItem* colsItem = new QTableWidgetItem(QString::number(rooms[i].getSoCot()));
        colsItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 3, colsItem);

        bool active = rooms[i].getTrangThai();
        QTableWidgetItem* statusItem = new QTableWidgetItem(active ? "Đang hoạt động" : "Ngừng hoạt động");
        statusItem->setTextAlignment(Qt::AlignCenter);
        statusItem->setFont(boldFont);
        statusItem->setForeground(QBrush(QColor(active ? "#059669" : "#94A3B8")));
        table->setItem(i, 4, statusItem);
    }
}

void RoomManagementWidget::addRoom() {
    QDialog dialog(this);
    dialog.setWindowTitle("Thêm Phòng Chiếu");
    dialog.setFixedWidth(430);
    dialog.setStyleSheet("QDialog { background-color: #FFFFFF; border: 1px solid #E5E7EB; border-radius: 10px; }");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(26, 24, 26, 24);
    layout->setSpacing(10);

    QLabel *dlgTitle = new QLabel("Thêm Phòng Chiếu Mới", &dialog);
    dlgTitle->setStyleSheet("color: #D97706; font-size: 18px; font-weight: 600;");
    layout->addWidget(dlgTitle);

    auto lbl = [&dialog](const QString& t) {
        QLabel* l = new QLabel(t, &dialog);
        l->setStyleSheet("color: #64748B; font-size: 12px; font-weight: 600;");
        return l;
    };

    QLineEdit *edtTen = new QLineEdit(&dialog); edtTen->setPlaceholderText("Ví dụ: Phòng IMAX 01");
    QSpinBox *spnHang = new QSpinBox(&dialog); spnHang->setRange(3, 20); spnHang->setValue(8); spnHang->setSuffix(" hàng");
    QSpinBox *spnCot = new QSpinBox(&dialog); spnCot->setRange(3, 20); spnCot->setValue(10); spnCot->setSuffix(" cột");

    layout->addWidget(lbl("Tên phòng chiếu:")); layout->addWidget(edtTen);
    layout->addWidget(lbl("Số hàng ghế:")); layout->addWidget(spnHang);
    layout->addWidget(lbl("Số cột ghế:")); layout->addWidget(spnCot);
    layout->addSpacing(8);

    QPushButton *btnCancel = new QPushButton("Hủy Bỏ", &dialog);
    btnCancel->setCursor(Qt::PointingHandCursor);
    btnCancel->setStyleSheet(Theme::secondaryButtonStyle(40, 13));

    QPushButton *btnOk = new QPushButton("Thêm Phòng", &dialog);
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
        DynamicArray<std::string> existingIDs;
        auto& allRooms = roomManager->getAllRooms();
        for (int i = 0; i < allRooms.size(); i++) existingIDs.push_back(allRooms[i].getMaPhong());
        std::string id = IDGenerator::generateID("RM", existingIDs);

        Room r(id, edtTen->text().trimmed().toStdString(), spnHang->value(), spnCot->value(), true);
        if (roomManager->addRoom(r)) {
            roomManager->save();
            loadData();
        }
    }
}

void RoomManagementWidget::editRoom() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Cảnh Báo", "Vui lòng chọn phòng cần sửa!");
        return;
    }
    std::string id = table->item(row, 0)->text().toStdString();
    Room* r = roomManager->findById(id);
    if (!r) return;

    QDialog dialog(this);
    dialog.setWindowTitle("Sửa Phòng Chiếu");
    dialog.setFixedWidth(430);
    dialog.setStyleSheet("QDialog { background-color: #FFFFFF; border: 1px solid #E5E7EB; border-radius: 10px; }");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(26, 24, 26, 24);
    layout->setSpacing(10);

    QLabel *dlgTitle = new QLabel("Cập Nhật Phòng Chiếu", &dialog);
    dlgTitle->setStyleSheet("color: #D97706; font-size: 18px; font-weight: 600;");
    layout->addWidget(dlgTitle);

    auto lbl = [&dialog](const QString& t) {
        QLabel* l = new QLabel(t, &dialog);
        l->setStyleSheet("color: #64748B; font-size: 12px; font-weight: 600;");
        return l;
    };

    QLineEdit *edtTen = new QLineEdit(QString::fromStdString(r->getTenPhong()), &dialog);
    QSpinBox *spnHang = new QSpinBox(&dialog); spnHang->setRange(3, 20); spnHang->setValue(r->getSoHang()); spnHang->setSuffix(" hàng");
    QSpinBox *spnCot = new QSpinBox(&dialog); spnCot->setRange(3, 20); spnCot->setValue(r->getSoCot()); spnCot->setSuffix(" cột");

    layout->addWidget(lbl("Tên phòng chiếu:")); layout->addWidget(edtTen);
    layout->addWidget(lbl("Số hàng ghế:")); layout->addWidget(spnHang);
    layout->addWidget(lbl("Số cột ghế:")); layout->addWidget(spnCot);
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
        r->setTenPhong(edtTen->text().trimmed().toStdString());
        r->setSoHang(spnHang->value());
        r->setSoCot(spnCot->value());
        roomManager->updateRoom(id, *r);
        roomManager->save();
        loadData();
    }
}

void RoomManagementWidget::deleteRoom() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Cảnh Báo", "Vui lòng chọn phòng cần ngừng hoạt động!");
        return;
    }
    std::string id = table->item(row, 0)->text().toStdString();
    if (QMessageBox::question(this, "Xác Nhận", "Bạn có muốn ngừng hoạt động phòng chiếu này?") == QMessageBox::Yes) {
        if (roomManager->deactivateRoom(id)) {
            roomManager->save();
            loadData();
        }
    }
}
