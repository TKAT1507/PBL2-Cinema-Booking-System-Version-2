#ifndef MOVIEDETAILWIDGET_H
#define MOVIEDETAILWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "managers/MovieManager.h"

class MovieDetailWidget : public QWidget {
    Q_OBJECT

public:
    explicit MovieDetailWidget(MovieManager* movieManager, QWidget *parent = nullptr);

public slots:
    void showMovie(const QString& maPhim);

signals:
    void bookMovie(const QString& maPhim);
    void goBack();

private:
    MovieManager* movieManager;
    QString currentMaPhim;

    QLabel *lblPoster;
    QLabel *lblTenPhim;
    QLabel *lblTheLoai;
    QLabel *lblThoiLuong;
    QLabel *lblDoTuoi;
    QLabel *lblMoTa;
    QLabel *lblNoiDung;

    QPushButton *btnBook;
    QPushButton *btnBack;
};

#endif // MOVIEDETAILWIDGET_H
