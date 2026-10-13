#ifndef SHOWTIMELISTWIDGET_H
#define SHOWTIMELISTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
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
    void onSelectShowtime();

private:
    ShowtimeManager* showtimeManager;
    MovieManager* movieManager;
    
    QComboBox *cbMovies;
    QTableWidget *tableShowtimes;
    QPushButton *btnSelect;
    QPushButton *btnBack;
};

#endif // SHOWTIMELISTWIDGET_H
