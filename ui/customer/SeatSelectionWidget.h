#ifndef SEATSELECTIONWIDGET_H
#define SEATSELECTIONWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QStringList>
#include "managers/ShowtimeManager.h"
#include "managers/TicketManager.h"
#include "core/DynamicArray.h"

struct SelectedSeat {
    int row;
    int col;
};

class SeatSelectionWidget : public QWidget {
    Q_OBJECT

public:
    explicit SeatSelectionWidget(ShowtimeManager* showtimeManager, TicketManager* ticketManager = nullptr, QWidget *parent = nullptr);

public slots:
    void loadSeatMap(const QString& maSuatChieu);

signals:
    void seatsSelected(const QString& maSuatChieu, const QStringList& selectedSeats);
    void goBack();

private slots:
    void onSeatClicked();
    void onContinue();

private:
    ShowtimeManager* showtimeManager;
    TicketManager* ticketManager;
    QString currentMaSuatChieu;
    
    QScrollArea *scrollArea;
    QWidget *gridContainer;
    QGridLayout *seatGrid;
    QLabel *lblSelectedInfo;
    QPushButton *btnContinue;
    QPushButton *btnBack;

    DynamicArray<SelectedSeat> selectedSeats;

    void updateSeatVisuals(QPushButton* btn, int row, int col, int status);
    void updateSelectedInfo();
};

#endif // SEATSELECTIONWIDGET_H
