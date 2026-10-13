#ifndef SEATSELECTIONWIDGET_H
#define SEATSELECTIONWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include "managers/ShowtimeManager.h"
#include "core/DynamicArray.h"
#include <QStringList>

struct SelectedSeat {
    int row;
    int col;
};

class SeatSelectionWidget : public QWidget {
    Q_OBJECT

public:
    explicit SeatSelectionWidget(ShowtimeManager* showtimeManager, QWidget *parent = nullptr);

public slots:
    void loadSeatMap(const QString& maSuatChieu);

signals:
    // We can use QStringList to pass the selected seats (e.g. "0,1", "0,2")
    void seatsSelected(const QString& maSuatChieu, const QStringList& selectedSeats);
    void goBack();

private slots:
    void onSeatClicked();
    void onContinue();

private:
    ShowtimeManager* showtimeManager;
    QString currentMaSuatChieu;
    
    QGridLayout *seatGrid;
    QLabel *lblSelectedInfo;
    QPushButton *btnContinue;
    QPushButton *btnBack;

    DynamicArray<SelectedSeat> selectedSeats;

    void updateSeatVisuals(QPushButton* btn, int row, int col, int status);
    void updateSelectedInfo();
};

#endif // SEATSELECTIONWIDGET_H
