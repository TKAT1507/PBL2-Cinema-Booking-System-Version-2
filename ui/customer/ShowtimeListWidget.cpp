#include "ShowtimeListWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include "models/SeatMap.h"

ShowtimeListWidget::ShowtimeListWidget(ShowtimeManager* showtimeManager, MovieManager* movieManager, QWidget *parent)
    : QWidget(parent), showtimeManager(showtimeManager), movieManager(movieManager)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *topLayout = new QHBoxLayout();
    cbMovies = new QComboBox(this);
    cbMovies->addItem("Tat ca phim", "");
    
    auto movies = movieManager->getActiveMovies();
    for (int i = 0; i < movies.size(); ++i) {
        cbMovies->addItem(QString::fromStdString(movies[i].getTenPhim()), QString::fromStdString(movies[i].getMaPhim()));
    }
    topLayout->addWidget(cbMovies);
    topLayout->addStretch();

    tableShowtimes = new QTableWidget(this);
    tableShowtimes->setColumnCount(5);
    tableShowtimes->setHorizontalHeaderLabels(QStringList() << "Phim" << "Phong" << "Ngay" << "Gio" << "Ghe Trong");
    tableShowtimes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableShowtimes->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableShowtimes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableShowtimes->setAlternatingRowColors(true);

    btnSelect = new QPushButton("Chon Suat Nay", this);
    btnBack = new QPushButton("Quay Lai", this);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(btnSelect);
    bottomLayout->addWidget(btnBack);

    layout->addLayout(topLayout);
    layout->addWidget(tableShowtimes);
    layout->addLayout(bottomLayout);

    connect(cbMovies, &QComboBox::currentIndexChanged, this, &ShowtimeListWidget::filterByMovie);
    connect(btnSelect, &QPushButton::clicked, this, &ShowtimeListWidget::onSelectShowtime);
    connect(btnBack, &QPushButton::clicked, this, &ShowtimeListWidget::goBack);

    loadShowtimes();
}

void ShowtimeListWidget::loadShowtimes() {
    filterByMovie();
}

void ShowtimeListWidget::setMovieFilter(const QString& maPhim) {
    int index = cbMovies->findData(maPhim);
    if (index >= 0) {
        cbMovies->setCurrentIndex(index);
    }
}

void ShowtimeListWidget::filterByMovie() {
    QString maPhim = cbMovies->currentData().toString();
    
    DynamicArray<Showtime> showtimes;
    if (maPhim.isEmpty()) {
        showtimes = showtimeManager->getActiveShowtimes(); // Assuming this exists or just get all valid
    } else {
        showtimes = showtimeManager->getShowtimesByMovie(maPhim.toStdString());
    }

    tableShowtimes->setRowCount(showtimes.size());
    for (int i = 0; i < showtimes.size(); ++i) {
        auto s = showtimes[i];
        auto movie = movieManager->findById(s.getMaPhim());
        QString tenPhim = movie ? QString::fromStdString(movie->getTenPhim()) : "Unknown";

        QTableWidgetItem* itemPhim = new QTableWidgetItem(tenPhim);
        itemPhim->setData(Qt::UserRole, QString::fromStdString(s.getMaSuatChieu()));
        tableShowtimes->setItem(i, 0, itemPhim);
        
        QTableWidgetItem* roomItem = new QTableWidgetItem(QString::fromStdString(s.getMaPhong()));
        roomItem->setTextAlignment(Qt::AlignCenter);
        tableShowtimes->setItem(i, 1, roomItem);
        
        QTableWidgetItem* dateItem = new QTableWidgetItem(QString::fromStdString(s.getNgayChieu()));
        dateItem->setTextAlignment(Qt::AlignCenter);
        tableShowtimes->setItem(i, 2, dateItem);
        
        QTableWidgetItem* timeItem = new QTableWidgetItem(QString::fromStdString(s.getGioChieu()));
        timeItem->setTextAlignment(Qt::AlignCenter);
        tableShowtimes->setItem(i, 3, timeItem);
        
        // Calculate empty seats if seat map exists
        auto seatMap = showtimeManager->findSeatMap(s.getMaSuatChieu());
        int emptyCount = 0;
        if (seatMap) {
            for (int r = 0; r < seatMap->getSoHang(); ++r) {
                for (int c = 0; c < seatMap->getSoCot(); ++c) {
                    if (seatMap->isSeatAvailable(r, c)) emptyCount++;
                }
            }
        }
        QTableWidgetItem* seatsItem = new QTableWidgetItem(QString::number(emptyCount) + " ghe");
        seatsItem->setTextAlignment(Qt::AlignCenter);
        tableShowtimes->setItem(i, 4, seatsItem);
    }
}

void ShowtimeListWidget::onSelectShowtime() {
    int row = tableShowtimes->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Canh bao", "Vui long chon mot suat chieu!");
        return;
    }
    QString maSuat = tableShowtimes->item(row, 0)->data(Qt::UserRole).toString();
    emit showtimeSelected(maSuat);
}
