#include "MovieListWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QStringList>
#include <QMessageBox>

MovieListWidget::MovieListWidget(MovieManager* movieManager, QWidget *parent)
    : QWidget(parent), movieManager(movieManager)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    txtSearch = new QLineEdit(this);
    txtSearch->setPlaceholderText("Tim kiem phim...");
    btnSearch = new QPushButton("Tim", this);
    searchLayout->addWidget(txtSearch);
    searchLayout->addWidget(btnSearch);

    tableMovies = new QTableWidget(this);
    tableMovies->setColumnCount(5);
    tableMovies->setHorizontalHeaderLabels(QStringList() << "Ten Phim" << "The Loai" << "Thoi Luong" << "Do Tuoi" << "Trang Thai");
    tableMovies->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableMovies->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableMovies->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableMovies->setAlternatingRowColors(true);

    btnDetail = new QPushButton("Xem Chi Tiet", this);

    layout->addLayout(searchLayout);
    layout->addWidget(tableMovies);
    layout->addWidget(btnDetail);

    connect(btnSearch, &QPushButton::clicked, this, &MovieListWidget::onSearch);
    connect(btnDetail, &QPushButton::clicked, this, &MovieListWidget::onMovieSelected);
    connect(tableMovies, &QTableWidget::doubleClicked, this, &MovieListWidget::onMovieSelected);

    loadMovies();
}

void MovieListWidget::loadMovies() {
    auto movies = movieManager->getActiveMovies();
    tableMovies->setRowCount(movies.size());
    for (int i = 0; i < movies.size(); ++i) {
        auto m = movies[i];
        QTableWidgetItem* itemTen = new QTableWidgetItem(QString::fromStdString(m.getTenPhim()));
        itemTen->setData(Qt::UserRole, QString::fromStdString(m.getMaPhim())); // store ID

        tableMovies->setItem(i, 0, itemTen);
        tableMovies->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(m.getTheLoai())));
        
        QTableWidgetItem* durItem = new QTableWidgetItem(QString::number(m.getThoiLuong()) + " phut");
        durItem->setTextAlignment(Qt::AlignCenter);
        tableMovies->setItem(i, 2, durItem);
        
        QTableWidgetItem* ageItem = new QTableWidgetItem(QString::number(m.getDoTuoi()) + "+");
        ageItem->setTextAlignment(Qt::AlignCenter);
        tableMovies->setItem(i, 3, ageItem);
        
        tableMovies->setItem(i, 4, new QTableWidgetItem(m.getTrangThai() ? "Dang chieu" : "Ngung chieu"));
    }
}

void MovieListWidget::onSearch() {
    std::string keyword = txtSearch->text().toStdString();
    auto movies = movieManager->searchByName(keyword);
    tableMovies->setRowCount(movies.size());
    for (int i = 0; i < movies.size(); ++i) {
        auto m = movies[i];
        QTableWidgetItem* itemTen = new QTableWidgetItem(QString::fromStdString(m.getTenPhim()));
        itemTen->setData(Qt::UserRole, QString::fromStdString(m.getMaPhim()));

        tableMovies->setItem(i, 0, itemTen);
        tableMovies->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(m.getTheLoai())));
        
        QTableWidgetItem* durItem = new QTableWidgetItem(QString::number(m.getThoiLuong()) + " phut");
        durItem->setTextAlignment(Qt::AlignCenter);
        tableMovies->setItem(i, 2, durItem);
        
        QTableWidgetItem* ageItem = new QTableWidgetItem(QString::number(m.getDoTuoi()) + "+");
        ageItem->setTextAlignment(Qt::AlignCenter);
        tableMovies->setItem(i, 3, ageItem);
        
        tableMovies->setItem(i, 4, new QTableWidgetItem(m.getTrangThai() ? "Dang chieu" : "Ngung chieu"));
    }
}

void MovieListWidget::onMovieSelected() {
    int row = tableMovies->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Canh bao", "Vui long chon mot phim!");
        return;
    }
    QString maPhim = tableMovies->item(row, 0)->data(Qt::UserRole).toString();
    emit movieSelected(maPhim);
}
