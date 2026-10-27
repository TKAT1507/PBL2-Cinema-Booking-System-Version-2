#ifndef SHOWTIMELISTWIDGET_H
#define SHOWTIMELISTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QDate>
#include <QList>
#include "managers/ShowtimeManager.h"
#include "managers/MovieManager.h"

class ShowtimeListWidget : public QWidget {
    Q_OBJECT

public:
    explicit ShowtimeListWidget(ShowtimeManager* showtimeManager, MovieManager* movieManager, QWidget *parent = nullptr);

signals:
    void showtimeSelected(const QString& maSuatChieu);
    void goBack();

public slots:
    void loadShowtimes();
    void setMovieFilter(const QString& maPhim);

private slots:
    void filterByMovie();
    void onDayTabClicked(int tabIndex);
    void onSelectShowtime();

private:
    ShowtimeManager* showtimeManager;
    MovieManager* movieManager;
    
    QLabel *lblMovieHeader;
    QComboBox *cbMovies;
    QList<QPushButton*> dayButtons;
    QList<QDate> upcomingDates;
    int currentDayFilter; // -1 for all, 0..6 for upcomingDates[0..6]
    QTableWidget *tableShowtimes;
    QPushButton *btnSelect;
    QPushButton *btnBack;

    void updateDayButtonStyles();
};

#endif // SHOWTIMELISTWIDGET_H
