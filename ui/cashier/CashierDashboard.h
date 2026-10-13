#pragma once
#include <QWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <string>

class MovieManager;
class RoomManager;
class ShowtimeManager;
class TicketManager;
class ComboManager;

class CashierDashboard : public QWidget {
    Q_OBJECT
public:
    explicit CashierDashboard(const std::string& maNhanVien, const std::string& tenNhanVien,
                              MovieManager* movieMgr, RoomManager* roomMgr,
                              ShowtimeManager* showtimeMgr, TicketManager* ticketMgr,
                              ComboManager* comboMgr, QWidget* parent = nullptr);

signals:
    void logout();

private:
    std::string maNhanVien;
    std::string tenNhanVien;

    QStackedWidget* stackedWidget;
    MovieManager* movieManager;
    RoomManager* roomManager;
    ShowtimeManager* showtimeManager;
    TicketManager* ticketManager;
    ComboManager* comboManager;

    void setupUI();
    void setupConnections();
};
