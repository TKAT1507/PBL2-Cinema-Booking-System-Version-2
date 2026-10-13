#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class TicketManager;
class ShowtimeManager;
class MovieManager;

class CheckTicketWidget : public QWidget {
    Q_OBJECT
public:
    explicit CheckTicketWidget(TicketManager* ticketMgr, ShowtimeManager* showtimeMgr,
                               MovieManager* movieMgr, QWidget* parent = nullptr);

private:
    TicketManager* ticketManager;
    ShowtimeManager* showtimeManager;
    MovieManager* movieManager;

    QLineEdit* ticketIdInput;
    QLabel* resultLabel;
    QPushButton* btnCheck;
    QPushButton* btnConfirm;
    
    std::string currentTicketId;

    void checkTicket();
    void confirmTicket();
};
