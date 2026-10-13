#ifndef CUSTOMERDASHBOARD_H
#define CUSTOMERDASHBOARD_H

#include <QWidget>
#include <QStackedWidget>
#include <QStringList>
#include "managers/UserManager.h"
#include "managers/MovieManager.h"
#include "managers/ShowtimeManager.h"
#include "managers/TicketManager.h"
#include "managers/ComboManager.h"
#include "managers/RoomManager.h"

class MovieListWidget;
class MovieDetailWidget;
class ShowtimeListWidget;
class SeatSelectionWidget;
class ComboSelectionWidget;
class BookingConfirmWidget;
class MyTicketsWidget;

class CustomerDashboard : public QWidget {
    Q_OBJECT

public:
    explicit CustomerDashboard(const QString& maKH, UserManager* userMgr, MovieManager* movieMgr, 
                               ShowtimeManager* showtimeMgr, TicketManager* ticketMgr, 
                               ComboManager* comboMgr, RoomManager* roomMgr, QWidget *parent = nullptr);

signals:
    void logout();

private:
    QString maKhachHang;
    UserManager* userManager;
    MovieManager* movieManager;
    ShowtimeManager* showtimeManager;
    TicketManager* ticketManager;
    ComboManager* comboManager;
    RoomManager* roomManager;

    QStackedWidget* stackedWidget;

    MovieListWidget* movieListWidget;
    MovieDetailWidget* movieDetailWidget;
    ShowtimeListWidget* showtimeListWidget;
    SeatSelectionWidget* seatSelectionWidget;
    ComboSelectionWidget* comboSelectionWidget;
    BookingConfirmWidget* bookingConfirmWidget;
    MyTicketsWidget* myTicketsWidget;

    // Temporary booking state between steps
    QString tempMaSuatChieu;
    QStringList tempSelectedSeats;
};

#endif // CUSTOMERDASHBOARD_H
