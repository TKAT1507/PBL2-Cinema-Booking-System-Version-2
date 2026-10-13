#ifndef MYTICKETSWIDGET_H
#define MYTICKETSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include "managers/TicketManager.h"
#include "managers/ShowtimeManager.h"
#include "managers/MovieManager.h"

class MyTicketsWidget : public QWidget {
    Q_OBJECT

public:
    explicit MyTicketsWidget(const QString& maKH, TicketManager* ticketManager, 
                             ShowtimeManager* showtimeManager, MovieManager* movieManager, QWidget *parent = nullptr);

public slots:
    void loadTickets();

private:
    QString maKhachHang;
    TicketManager* ticketManager;
    ShowtimeManager* showtimeManager;
    MovieManager* movieManager;
    
    QLineEdit *txtSearch;
    QTableWidget *tableTickets;
};

#endif // MYTICKETSWIDGET_H
