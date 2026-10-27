#include "ShowtimeListWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include "models/SeatMap.h"
#include "utils/SystemDate.h"

ShowtimeListWidget::ShowtimeListWidget(ShowtimeManager* showtimeManager, MovieManager* movieManager, QWidget *parent)
    : QWidget(parent), showtimeManager(showtimeManager), movieManager(movieManager), currentDayFilter(-1)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(12);

    // Header Title
    lblMovieHeader = new QLabel("LỊCH CHIẾU PHIM", this);
    lblMovieHeader->setAlignment(Qt::AlignCenter);
    lblMovieHeader->setStyleSheet("color: #FFFFFF; font-size: 20px; font-weight: bold;");
    mainLayout->addWidget(lblMovieHeader);

    // Top Filter Bar: Movie Dropdown
    QHBoxLayout *topBar = new QHBoxLayout();
    QLabel *lblFilter = new QLabel("Chọn phim:", this);
    lblFilter->setStyleSheet("color: #F5C542; font-size: 14px; font-weight: bold;");

    cbMovies = new QComboBox(this);
    cbMovies->addItem("Tất cả phim", "");
    cbMovies->setMinimumWidth(250);
    cbMovies->setStyleSheet(R"(
        QComboBox {
            background-color: #E6B042;
            color: #1A1B41;
            font-size: 13px;
            font-weight: bold;
            border-radius: 5px;
            padding: 5px 10px;
        }
    )");

    auto movies = movieManager->getActiveMovies();
    for (int i = 0; i < movies.size(); ++i) {
        cbMovies->addItem(QString::fromStdString(movies[i].getTenPhim()), QString::fromStdString(movies[i].getMaPhim()));
    }

    topBar->addWidget(lblFilter);
    topBar->addWidget(cbMovies);
    topBar->addStretch();
    mainLayout->addLayout(topBar);

    // Day Tabs: Tất Cả + 7 Ngày liên tiếp (bắt đầu từ Hôm nay)
    upcomingDates = SystemDate::getUpcoming7Days();

    QHBoxLayout *dayTabsLayout = new QHBoxLayout();
    dayTabsLayout->setSpacing(6);

    // Tab 0: "Tất Cả"
    QPushButton *btnAll = new QPushButton("Tất Cả\nLịch Chiếu", this);
    btnAll->setCursor(Qt::PointingHandCursor);
    btnAll->setFixedHeight(46);
    dayButtons.append(btnAll);
    dayTabsLayout->addWidget(btnAll);
    connect(btnAll, &QPushButton::clicked, this, [this]() {
        onDayTabClicked(-1);
    });

    // Tab 1..7: 7 ngày tới
    for (int i = 0; i < upcomingDates.size(); ++i) {
        const QDate& d = upcomingDates[i];
        QString dayName = SystemDate::getDayOfWeekVN(d);
        QString labelText;
        if (i == 0) {
            labelText = QString("%1\n(Hôm nay - %2)").arg(dayName, d.toString("dd/MM"));
        } else {
            labelText = QString("%1\n(%2)").arg(dayName, d.toString("dd/MM"));
        }

        QPushButton *btnDay = new QPushButton(labelText, this);
        btnDay->setCursor(Qt::PointingHandCursor);
        btnDay->setFixedHeight(46);
        dayButtons.append(btnDay);
        dayTabsLayout->addWidget(btnDay);

        connect(btnDay, &QPushButton::clicked, this, [this, i]() {
            onDayTabClicked(i);
        });
    }
    mainLayout->addLayout(dayTabsLayout);
    updateDayButtonStyles();

    // Table of Showtimes
    tableShowtimes = new QTableWidget(this);
    tableShowtimes->setColumnCount(5);
    tableShowtimes->setHorizontalHeaderLabels(QStringList() << "Tên Phim" << "Phòng Chiếu" << "Ngày Chiếu" << "Giờ Chiếu" << "Ghế Trống");
    tableShowtimes->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableShowtimes->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableShowtimes->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    tableShowtimes->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tableShowtimes->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tableShowtimes->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableShowtimes->setSelectionMode(QAbstractItemView::SingleSelection);
    tableShowtimes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableShowtimes->setAlternatingRowColors(true);
    tableShowtimes->verticalHeader()->setVisible(false);
    mainLayout->addWidget(tableShowtimes);

    // Bottom Action Buttons
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(15);

    QString btnStyle = R"(
        QPushButton {
            background-color: #9E221E;
            color: #FFFFFF;
            font-size: 14px;
            font-weight: bold;
            border-radius: 6px;
            border: none;
            min-height: 38px;
            padding: 6px 20px;
        }
        QPushButton:hover {
            background-color: #B52A26;
            border: 1px solid #E6B042;
        }
    )";

    btnSelect = new QPushButton("Chọn Suất Này ->", this);
    btnSelect->setCursor(Qt::PointingHandCursor);
    btnSelect->setStyleSheet(btnStyle);

    btnBack = new QPushButton("<- Quay Lại", this);
    btnBack->setCursor(Qt::PointingHandCursor);
    btnBack->setStyleSheet(btnStyle);

    bottomLayout->addWidget(btnBack);
    bottomLayout->addStretch();
    bottomLayout->addWidget(btnSelect);
    mainLayout->addLayout(bottomLayout);

    connect(cbMovies, &QComboBox::currentIndexChanged, this, &ShowtimeListWidget::filterByMovie);
    connect(btnSelect, &QPushButton::clicked, this, &ShowtimeListWidget::onSelectShowtime);
    connect(btnBack, &QPushButton::clicked, this, &ShowtimeListWidget::goBack);
    connect(tableShowtimes, &QTableWidget::doubleClicked, this, &ShowtimeListWidget::onSelectShowtime);

    loadShowtimes();
}

void ShowtimeListWidget::updateDayButtonStyles() {
    for (int i = 0; i < dayButtons.size(); ++i) {
        int filterIndex = i - 1; // -1 is All, 0..6 is upcomingDates[0..6]
        if (filterIndex == currentDayFilter) {
            dayButtons[i]->setStyleSheet(R"(
                QPushButton {
                    background-color: #9E221E;
                    color: #FFFFFF;
                    font-size: 11px;
                    font-weight: bold;
                    border: 1px solid #E6B042;
                    border-radius: 6px;
                    padding: 2px 4px;
                }
            )");
        } else {
            dayButtons[i]->setStyleSheet(R"(
                QPushButton {
                    background-color: #F4DFB6;
                    color: #1A1A1A;
                    font-size: 11px;
                    font-weight: bold;
                    border: 1px solid #333333;
                    border-radius: 6px;
                    padding: 2px 4px;
                }
                QPushButton:hover {
                    background-color: #FFF2D6;
                }
            )");
        }
    }
}

void ShowtimeListWidget::onDayTabClicked(int tabIndex) {
    currentDayFilter = tabIndex; // -1 for all, 0..6 for specific day
    updateDayButtonStyles();
    filterByMovie();
}

void ShowtimeListWidget::loadShowtimes() {
    filterByMovie();
}

void ShowtimeListWidget::setMovieFilter(const QString& maPhim) {
    int index = cbMovies->findData(maPhim);
    if (index >= 0) {
        cbMovies->setCurrentIndex(index);
        auto m = movieManager->findById(maPhim.toStdString());
        if (m) {
            lblMovieHeader->setText("LỊCH CHIẾU: " + QString::fromStdString(m->getTenPhim()).toUpper());
        }
    } else {
        lblMovieHeader->setText("LỊCH CHIẾU PHIM");
    }
}

void ShowtimeListWidget::filterByMovie() {
    QString maPhim = cbMovies->currentData().toString();
    
    DynamicArray<Showtime> showtimes;
    if (maPhim.isEmpty()) {
        showtimes = showtimeManager->getActiveShowtimes();
        lblMovieHeader->setText("LỊCH CHIẾU TẤT CẢ CÁC PHIM");
    } else {
        showtimes = showtimeManager->getShowtimesByMovie(maPhim.toStdString());
        auto m = movieManager->findById(maPhim.toStdString());
        if (m) {
            lblMovieHeader->setText("LỊCH CHIẾU: " + QString::fromStdString(m->getTenPhim()).toUpper());
        }
    }

    // Filter by specific date if selected (currentDayFilter: -1=All, 0..6=upcomingDates[0..6])
    DynamicArray<Showtime> filtered;
    for (int i = 0; i < showtimes.size(); ++i) {
        if (currentDayFilter >= 0 && currentDayFilter < upcomingDates.size()) {
            QString targetDateStr = upcomingDates[currentDayFilter].toString("dd/MM/yyyy");
            QDate date = QDate::fromString(QString::fromStdString(showtimes[i].getNgayChieu()), "dd/MM/yyyy");
            if (!date.isValid()) {
                date = QDate::fromString(QString::fromStdString(showtimes[i].getNgayChieu()), "yyyy-MM-dd");
            }
            if (date.isValid()) {
                if (date.toString("dd/MM/yyyy") != targetDateStr) continue;
            } else {
                if (QString::fromStdString(showtimes[i].getNgayChieu()) != targetDateStr) continue;
            }
        }
        filtered.push_back(showtimes[i]);
    }

    tableShowtimes->setRowCount(filtered.size());
    for (int i = 0; i < filtered.size(); ++i) {
        auto s = filtered[i];
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
        int totalSeats = 0;
        if (seatMap) {
            totalSeats = seatMap->getSoHang() * seatMap->getSoCot();
            for (int r = 0; r < seatMap->getSoHang(); ++r) {
                for (int c = 0; c < seatMap->getSoCot(); ++c) {
                    if (seatMap->isSeatAvailable(r, c)) emptyCount++;
                }
            }
        }
        
        QTableWidgetItem* seatItem = new QTableWidgetItem(QString("%1 / %2").arg(emptyCount).arg(totalSeats));
        seatItem->setTextAlignment(Qt::AlignCenter);
        if (emptyCount > 0) {
            seatItem->setForeground(QBrush(QColor("#5CD65C")));
        } else {
            seatItem->setForeground(QBrush(QColor("#FF4D4D")));
        }
        tableShowtimes->setItem(i, 4, seatItem);
    }
}

void ShowtimeListWidget::onSelectShowtime() {
    int row = tableShowtimes->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng chọn một suất chiếu!");
        return;
    }
    QString maSuatChieu = tableShowtimes->item(row, 0)->data(Qt::UserRole).toString();
    emit showtimeSelected(maSuatChieu);
}
