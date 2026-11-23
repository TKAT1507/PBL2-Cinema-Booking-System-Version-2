#include "ShowtimeListWidget.h"
#include "../common/Theme.h"
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
    mainLayout->setContentsMargins(26, 22, 26, 20);
    mainLayout->setSpacing(14);

    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->setSpacing(12);

    QVBoxLayout *titleCol = new QVBoxLayout();
    titleCol->setSpacing(2);

    lblMovieHeader = new QLabel("Lịch chiếu", this);
    lblMovieHeader->setStyleSheet(Theme::sectionTitleStyle());

    titleCol->addWidget(lblMovieHeader);

    topBar->addLayout(titleCol);
    topBar->addStretch();

    QLabel *lblFilter = new QLabel("Lọc theo phim:", this);
    lblFilter->setStyleSheet("color: #64748B; font-size: 13px; font-weight: 600;");

    cbMovies = new QComboBox(this);
    cbMovies->addItem("Tất cả phim", "");
    cbMovies->setMinimumWidth(260);
    cbMovies->setFixedHeight(40);

    auto movies = movieManager->getActiveMovies();
    for (int i = 0; i < movies.size(); ++i) {
        cbMovies->addItem(QString::fromStdString(movies[i].getTenPhim()), QString::fromStdString(movies[i].getMaPhim()));
    }

    topBar->addWidget(lblFilter, 0, Qt::AlignVCenter);
    topBar->addWidget(cbMovies, 0, Qt::AlignVCenter);
    mainLayout->addLayout(topBar);

    upcomingDates = SystemDate::getUpcoming7Days();

    QHBoxLayout *dayTabsLayout = new QHBoxLayout();
    dayTabsLayout->setSpacing(8);

    QPushButton *btnAll = new QPushButton("Tất Cả\nLịch Chiếu", this);
    btnAll->setCursor(Qt::PointingHandCursor);
    btnAll->setFixedHeight(50);
    dayButtons.append(btnAll);
    dayTabsLayout->addWidget(btnAll);
    connect(btnAll, &QPushButton::clicked, this, [this]() {
        onDayTabClicked(-1);
    });

    for (int i = 0; i < upcomingDates.size(); ++i) {
        const QDate& d = upcomingDates[i];
        QString dayName = SystemDate::getDayOfWeekVN(d);
        QString labelText;
        if (i == 0) {
            labelText = QString("%1\nHôm nay • %2").arg(dayName, d.toString("dd/MM"));
        } else {
            labelText = QString("%1\n%2").arg(dayName, d.toString("dd/MM"));
        }

        QPushButton *btnDay = new QPushButton(labelText, this);
        btnDay->setCursor(Qt::PointingHandCursor);
        btnDay->setFixedHeight(50);
        dayButtons.append(btnDay);
        dayTabsLayout->addWidget(btnDay);

        connect(btnDay, &QPushButton::clicked, this, [this, i]() {
            onDayTabClicked(i);
        });
    }
    mainLayout->addLayout(dayTabsLayout);
    updateDayButtonStyles();

    tableShowtimes = new QTableWidget(this);
    tableShowtimes->setColumnCount(5);
    tableShowtimes->setHorizontalHeaderLabels(QStringList() << "Tên Phim" << "Phòng Chiếu" << "Ngày Chiếu" << "Giờ Chiếu" << "Ghế Trống");
    tableShowtimes->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableShowtimes->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableShowtimes->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    tableShowtimes->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tableShowtimes->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tableShowtimes->verticalHeader()->setDefaultSectionSize(46);
    tableShowtimes->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableShowtimes->setSelectionMode(QAbstractItemView::SingleSelection);
    tableShowtimes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableShowtimes->setAlternatingRowColors(true);
    tableShowtimes->verticalHeader()->setVisible(false);
    mainLayout->addWidget(tableShowtimes, 1);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(12);

    btnBack = new QPushButton("Quay Lại", this);
    btnBack->setCursor(Qt::PointingHandCursor);
    btnBack->setStyleSheet(Theme::secondaryButtonStyle(42, 14));

    btnSelect = new QPushButton("Chọn Suất Chiếu Này", this);
    btnSelect->setCursor(Qt::PointingHandCursor);
    btnSelect->setStyleSheet(Theme::primaryButtonStyle(42, 14));

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
        int filterIndex = i - 1;
        if (filterIndex == currentDayFilter) {
            dayButtons[i]->setStyleSheet(R"(
                QPushButton {
                    background: #E05D5D;
                    color: #FFFFFF;
                    font-size: 11px;
                    font-weight: 600;
                    border: none;
                    border-radius: 6px;
                    padding: 4px 6px;
                }
            )");
        } else {
            dayButtons[i]->setStyleSheet(R"(
                QPushButton {
                    background-color: #FFFFFF;
                    color: #64748B;
                    font-size: 11px;
                    font-weight: 600;
                    border: 1px solid #E5E7EB;
                    border-radius: 6px;
                    padding: 4px 6px;
                }
                QPushButton:hover {
                    background-color: #FEF2F2;
                    border-color: #FCA5A5;
                    color: #D14343;
                }
            )");
        }
    }
}

void ShowtimeListWidget::onDayTabClicked(int tabIndex) {
    currentDayFilter = tabIndex;
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
            lblMovieHeader->setText("Lịch chiếu: " + QString::fromStdString(m->getTenPhim()));
        }
    } else {
        lblMovieHeader->setText("Lịch chiếu");
    }
}

void ShowtimeListWidget::filterByMovie() {
    QString maPhim = cbMovies->currentData().toString();

    DynamicArray<Showtime> showtimes;
    if (maPhim.isEmpty()) {
        showtimes = showtimeManager->getActiveShowtimes();
        lblMovieHeader->setText("Lịch chiếu tất cả phim");
    } else {
        showtimes = showtimeManager->getShowtimesByMovie(maPhim.toStdString());
        auto m = movieManager->findById(maPhim.toStdString());
        if (m) {
            lblMovieHeader->setText("Lịch chiếu: " + QString::fromStdString(m->getTenPhim()));
        }
    }

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

    QFont boldFont = tableShowtimes->font();
    boldFont.setBold(true);

    tableShowtimes->setRowCount(filtered.size());
    for (int i = 0; i < filtered.size(); ++i) {
        auto s = filtered[i];
        auto movie = movieManager->findById(s.getMaPhim());
        QString tenPhim = movie ? QString::fromStdString(movie->getTenPhim()) : "Không xác định";

        QTableWidgetItem* itemPhim = new QTableWidgetItem(tenPhim);
        itemPhim->setData(Qt::UserRole, QString::fromStdString(s.getMaSuatChieu()));
        itemPhim->setFont(boldFont);
        tableShowtimes->setItem(i, 0, itemPhim);

        QTableWidgetItem* roomItem = new QTableWidgetItem(QString::fromStdString(s.getMaPhong()));
        roomItem->setTextAlignment(Qt::AlignCenter);
        tableShowtimes->setItem(i, 1, roomItem);

        QTableWidgetItem* dateItem = new QTableWidgetItem(QString::fromStdString(s.getNgayChieu()));
        dateItem->setTextAlignment(Qt::AlignCenter);
        tableShowtimes->setItem(i, 2, dateItem);

        QTableWidgetItem* timeItem = new QTableWidgetItem(QString::fromStdString(s.getGioChieu()));
        timeItem->setTextAlignment(Qt::AlignCenter);
        timeItem->setFont(boldFont);
        timeItem->setForeground(QBrush(QColor("#D97706")));
        tableShowtimes->setItem(i, 3, timeItem);

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
        seatItem->setFont(boldFont);
        if (emptyCount > 0) {
            seatItem->setForeground(QBrush(QColor("#059669")));
        } else {
            seatItem->setForeground(QBrush(QColor("#E05D5D")));
        }
        tableShowtimes->setItem(i, 4, seatItem);
    }
}

void ShowtimeListWidget::onSelectShowtime() {
    int row = tableShowtimes->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Thông Báo", "Vui lòng chọn một suất chiếu!");
        return;
    }
    QString maSuatChieu = tableShowtimes->item(row, 0)->data(Qt::UserRole).toString();
    emit showtimeSelected(maSuatChieu);
}
