#include "RoomManagementWidget.h"
#include "managers/RoomManager.h"
#include "models/Room.h"
#include "utils/IDGenerator.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QHBoxLayout>
#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>

RoomManagementWidget::RoomManagementWidget(RoomManager* roomMgr, QWidget* parent)
    : QWidget(parent), roomManager(roomMgr) {
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnAdd = new QPushButton("Them Phong", this);
    btnEdit = new QPushButton("Sua Phong", this);
    btnDelete = new QPushButton("Ngung Su Dung", this);
    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnEdit);
    btnLayout->addWidget(btnDelete);
    
    table = new QTableWidget(0, 5, this);
    table->setHorizontalHeaderLabels({"Ma Phong", "Ten Phong", "So Hang", "So Cot", "Trang Thai"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    layout->addLayout(btnLayout);
    layout->addWidget(table);
    
    connect(btnAdd, &QPushButton::clicked, this, &RoomManagementWidget::addRoom);
    connect(btnEdit, &QPushButton::clicked, this, &RoomManagementWidget::editRoom);
    connect(btnDelete, &QPushButton::clicked, this, &RoomManagementWidget::deleteRoom);
    
    loadData();
}

void RoomManagementWidget::loadData() {
    table->setRowCount(0);
    auto& rooms = roomManager->getAllRooms();
    for(int i = 0; i < rooms.size(); ++i) {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(rooms[i].getMaPhong())));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(rooms[i].getTenPhong())));
        
        QTableWidgetItem* rowsItem = new QTableWidgetItem(QString::number(rooms[i].getSoHang()));
        rowsItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 2, rowsItem);
        
        QTableWidgetItem* colsItem = new QTableWidgetItem(QString::number(rooms[i].getSoCot()));
        colsItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 3, colsItem);
        
        QString status = rooms[i].getTrangThai() ? "Hoat Dong" : "Ngung Hoat Dong";
        QTableWidgetItem* statusItem = new QTableWidgetItem(status);
        if(!rooms[i].getTrangThai()) statusItem->setBackground(Qt::gray);
        table->setItem(i, 4, statusItem);
    }
}

void RoomManagementWidget::addRoom() {
    QDialog dialog(this);
    dialog.setWindowTitle("Them Phong");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QLineEdit *edtTen = new QLineEdit(&dialog); edtTen->setPlaceholderText("Ten phong");
    QSpinBox *spnHang = new QSpinBox(&dialog); spnHang->setRange(3, 20); spnHang->setValue(8);
    QSpinBox *spnCot = new QSpinBox(&dialog); spnCot->setRange(3, 20); spnCot->setValue(10);
    
    layout->addWidget(new QLabel("Ten phong:", &dialog)); layout->addWidget(edtTen);
    layout->addWidget(new QLabel("So hang:", &dialog)); layout->addWidget(spnHang);
    layout->addWidget(new QLabel("So cot:", &dialog)); layout->addWidget(spnCot);
    
    QPushButton *btnOk = new QPushButton("Them", &dialog);
    QPushButton *btnCancel = new QPushButton("Huy", &dialog);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnOk); btnLayout->addWidget(btnCancel);
    layout->addLayout(btnLayout);
    connect(btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted && !edtTen->text().isEmpty()) {
        DynamicArray<std::string> existingIDs;
        auto& allRooms = roomManager->getAllRooms();
        for (int i = 0; i < allRooms.size(); i++) existingIDs.push_back(allRooms[i].getMaPhong());
        std::string id = IDGenerator::generateID("RM", existingIDs);
        
        Room r(id, edtTen->text().toStdString(), spnHang->value(), spnCot->value(), true);
        if(roomManager->addRoom(r)) {
            roomManager->save();
            loadData();
        }
    }
}

void RoomManagementWidget::editRoom() {
    int row = table->currentRow();
    if(row < 0) {
        QMessageBox::warning(this, "Canh bao", "Vui long chon phong can sua!");
        return;
    }
    std::string id = table->item(row, 0)->text().toStdString();
    Room* r = roomManager->findById(id);
    if(!r) return;
    
    QDialog dialog(this);
    dialog.setWindowTitle("Sua Phong");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QLineEdit *edtTen = new QLineEdit(QString::fromStdString(r->getTenPhong()), &dialog);
    QSpinBox *spnHang = new QSpinBox(&dialog); spnHang->setRange(3, 20); spnHang->setValue(r->getSoHang());
    QSpinBox *spnCot = new QSpinBox(&dialog); spnCot->setRange(3, 20); spnCot->setValue(r->getSoCot());
    
    layout->addWidget(new QLabel("Ten phong:", &dialog)); layout->addWidget(edtTen);
    layout->addWidget(new QLabel("So hang:", &dialog)); layout->addWidget(spnHang);
    layout->addWidget(new QLabel("So cot:", &dialog)); layout->addWidget(spnCot);
    
    QPushButton *btnOk = new QPushButton("Luu", &dialog);
    QPushButton *btnCancel = new QPushButton("Huy", &dialog);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnOk); btnLayout->addWidget(btnCancel);
    layout->addLayout(btnLayout);
    connect(btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted && !edtTen->text().isEmpty()) {
        r->setTenPhong(edtTen->text().toStdString());
        r->setSoHang(spnHang->value());
        r->setSoCot(spnCot->value());
        roomManager->updateRoom(id, *r);
        roomManager->save();
        loadData();
    }
}

void RoomManagementWidget::deleteRoom() {
    int row = table->currentRow();
    if(row >= 0) {
        std::string id = table->item(row, 0)->text().toStdString();
        if(QMessageBox::question(this, "Xac Nhan", "Ban co muon ngung hoat dong phong nay?") == QMessageBox::Yes) {
            if(roomManager->deactivateRoom(id)) {
                roomManager->save();
                loadData();
            }
        }
    }
}
