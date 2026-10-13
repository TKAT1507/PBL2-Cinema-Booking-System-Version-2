#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>

class ShowtimeManager;
class MovieManager;
class RoomManager;

class ShowtimeManagementWidget : public QWidget {
    Q_OBJECT
public:
    explicit ShowtimeManagementWidget(ShowtimeManager* showtimeMgr, MovieManager* movieMgr, RoomManager* roomMgr, QWidget* parent = nullptr);

private:
    ShowtimeManager* showtimeManager;
    MovieManager* movieManager;
    RoomManager* roomManager;
    QTableWidget* table;
    QPushButton* btnAdd;
    QPushButton* btnDelete;

    void loadData();
    void addShowtime();
    void deleteShowtime();
};
