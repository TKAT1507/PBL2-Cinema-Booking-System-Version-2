#pragma once
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>

class TicketManager;
class ShowtimeManager;
class MovieManager;

class StatisticsWidget : public QWidget {
    Q_OBJECT
public:
    explicit StatisticsWidget(TicketManager* ticketMgr, ShowtimeManager* showtimeMgr, MovieManager* movieMgr, QWidget* parent = nullptr);

private:
    TicketManager* ticketManager;
    ShowtimeManager* showtimeManager;
    MovieManager* movieManager;
    
    QLabel* lblTotalRevenue;
    QLabel* lblTotalTickets;
    
    QDateEdit* dateEdit;
    QLabel* lblDateRevenue;
    QPushButton* btnCheckDate;
    
    QComboBox* cbMovies;
    QLabel* lblMovieRevenue;
    QPushButton* btnCheckMovie;
    
    void loadOverview();
    void checkDateRevenue();
    void checkMovieRevenue();
};
