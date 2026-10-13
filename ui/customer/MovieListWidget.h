#ifndef MOVIELISTWIDGET_H
#define MOVIELISTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include "managers/MovieManager.h"

class MovieListWidget : public QWidget {
    Q_OBJECT

public:
    explicit MovieListWidget(MovieManager* movieManager, QWidget *parent = nullptr);

signals:
    void movieSelected(const QString& maPhim);

public slots:
    void loadMovies();

private slots:
    void onSearch();
    void onMovieSelected();

private:
    MovieManager* movieManager;
    QLineEdit *txtSearch;
    QPushButton *btnSearch;
    QTableWidget *tableMovies;
    QPushButton *btnDetail;
};

#endif // MOVIELISTWIDGET_H
