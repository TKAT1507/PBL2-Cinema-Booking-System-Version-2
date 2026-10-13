#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>

class RoomManager;

class RoomManagementWidget : public QWidget {
    Q_OBJECT
public:
    explicit RoomManagementWidget(RoomManager* roomMgr, QWidget* parent = nullptr);

private:
    RoomManager* roomManager;
    QTableWidget* table;
    QPushButton* btnAdd;
    QPushButton* btnEdit;
    QPushButton* btnDelete;

    void loadData();
    void addRoom();
    void editRoom();
    void deleteRoom();
};
