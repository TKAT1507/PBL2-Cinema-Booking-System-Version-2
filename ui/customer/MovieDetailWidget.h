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
    void setCustomerAge(int age);

public slots:
    void showMovie(const QString& maPhim);

signals:
    void bookMovie(const QString& maPhim);
    void goBack();

private:
    MovieManager* movieManager;
    QString currentMaPhim;
    int customerAge;

    QLabel *lblPoster;
    QLabel *lblTenPhim;
    QLabel *lblTheLoai;
    QLabel *lblThoiLuong;
    QLabel *lblDoTuoi;
    QLabel *lblAgeWarning;
    QLabel *lblMoTa;
    QLabel *lblNoiDung;

    QPushButton *btnBook;
    QPushButton *btnBack;
};

#endif // MOVIEDETAILWIDGET_H
