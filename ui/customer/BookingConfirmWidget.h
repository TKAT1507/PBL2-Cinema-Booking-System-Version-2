#ifndef BOOKINGCONFIRMWIDGET_H
#define BOOKINGCONFIRMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include "managers/TicketManager.h"
#include "managers/ShowtimeManager.h"
#include "managers/ComboManager.h"

class BookingConfirmWidget : public QWidget {
    Q_OBJECT

public:
    explicit BookingConfirmWidget(const QString& maKH, TicketManager* ticketManager, 
                                  ShowtimeManager* showtimeManager, ComboManager* comboManager, QWidget *parent = nullptr);

    void setBookingData(const QString& maSuat, const QStringList& seats, const QStringList& combos);

signals:
    void bookingComplete();
    void goBack();

private slots:
    void onConfirm();

private:
    QString maKhachHang;
    TicketManager* ticketManager;
    ShowtimeManager* showtimeManager;
    ComboManager* comboManager;

    QString currentMaSuat;
    QStringList currentSeats;
    QStringList currentCombos;
    QString currentComboString;  // formatted combo string for ticket
    double currentComboTotalPrice; // total combo price

    QLabel *lblSummary;
    QLabel *lblTotal;
    QPushButton *btnConfirm;
    QPushButton *btnCancel;
};

#endif // BOOKINGCONFIRMWIDGET_H
