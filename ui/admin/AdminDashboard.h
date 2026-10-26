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
class UserManager;

class AdminDashboard : public QWidget {
    Q_OBJECT
public:
    explicit AdminDashboard(const std::string& maQuanLy, const std::string& tenQuanLy,
                            UserManager* userMgr, MovieManager* movieMgr, RoomManager* roomMgr,
                            ShowtimeManager* showtimeMgr, TicketManager* ticketMgr,
                            ComboManager* comboMgr, QWidget* parent = nullptr);

signals:
    void logout();

private:
    std::string maQuanLy;
    std::string tenQuanLy;

    QStackedWidget* stackedWidget;
    UserManager* userManager;
    MovieManager* movieManager;
    RoomManager* roomManager;
    ShowtimeManager* showtimeManager;
    TicketManager* ticketManager;
    ComboManager* comboManager;

    void setupUI();
};
