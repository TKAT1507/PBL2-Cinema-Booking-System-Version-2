#ifndef MOVIELISTWIDGET_H
#define MOVIELISTWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QGridLayout>
#include "managers/MovieManager.h"
#include "managers/ShowtimeManager.h"

class MovieListWidget : public QWidget {
    Q_OBJECT

public:
    explicit MovieListWidget(MovieManager* movieManager, ShowtimeManager* showtimeManager = nullptr, QWidget *parent = nullptr);

signals:
    void movieSelected(const QString& maPhim);

public slots:
    void loadMovies();

private slots:
    void onSearch();

private:
    MovieManager* movieManager;
    ShowtimeManager* showtimeManager;
    QLineEdit *txtSearch;
    QPushButton *btnSearch;
    QScrollArea *scrollArea;
    QWidget *cardsContainer;
    QGridLayout *gridLayout;

    void populateGrid(const DynamicArray<Movie>& movies);
    QWidget* createMovieCard(const Movie& movie);
};

#endif // MOVIELISTWIDGET_H
