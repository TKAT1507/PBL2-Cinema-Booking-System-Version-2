#include "MovieListWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QPixmap>
#include <QFileInfo>
#include <QCoreApplication>
#include <QCursor>

MovieListWidget::MovieListWidget(MovieManager* movieManager, ShowtimeManager* showtimeManager, QWidget *parent)
    : QWidget(parent), movieManager(movieManager), showtimeManager(showtimeManager)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(12);

    // Top Bar: Title & Search Bar (Top Right)
    QHBoxLayout *topBar = new QHBoxLayout();
    
    QLabel *lblSectionTitle = new QLabel("DANH SÁCH PHIM ĐANG CHIẾU", this);
    lblSectionTitle->setStyleSheet("color: #F5C542; font-size: 18px; font-weight: bold;");
    topBar->addWidget(lblSectionTitle);
    topBar->addStretch();

    txtSearch = new QLineEdit(this);
    txtSearch->setPlaceholderText("Tìm kiếm phim...");
    txtSearch->setFixedWidth(240);
    txtSearch->setStyleSheet(R"(
        QLineEdit {
            background-color: #E6B042;
            color: #1A1B41;
            font-size: 13px;
            font-weight: bold;
            border: 1px solid #C49129;
            border-radius: 5px;
            padding: 4px 10px;
            min-height: 28px;
        }
        QLineEdit:focus {
            border: 2px solid #FFFFFF;
        }
    )");

    btnSearch = new QPushButton("Tìm", this);
    btnSearch->setCursor(Qt::PointingHandCursor);
    btnSearch->setStyleSheet(R"(
        QPushButton {
            background-color: #9E221E;
            color: #FFFFFF;
            font-size: 13px;
            font-weight: bold;
            border-radius: 5px;
            border: none;
            min-height: 30px;
            padding: 4px 14px;
        }
        QPushButton:hover {
            background-color: #B52A26;
        }
    )");

    topBar->addWidget(txtSearch);
    topBar->addWidget(btnSearch);
    mainLayout->addLayout(topBar);

    // Scroll Area for 2-column Movie Cards Grid
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { background: transparent; border: none; }");

    cardsContainer = new QWidget(scrollArea);
    cardsContainer->setStyleSheet("background: transparent;");
    gridLayout = new QGridLayout(cardsContainer);
    gridLayout->setSpacing(16);
    gridLayout->setContentsMargins(0, 5, 0, 10);

    scrollArea->setWidget(cardsContainer);
    mainLayout->addWidget(scrollArea);

    connect(btnSearch, &QPushButton::clicked, this, &MovieListWidget::onSearch);
    connect(txtSearch, &QLineEdit::returnPressed, this, &MovieListWidget::onSearch);

    loadMovies();
}

void MovieListWidget::loadMovies() {
    auto movies = movieManager->getActiveMovies();
    populateGrid(movies);
}

void MovieListWidget::onSearch() {
    std::string keyword = txtSearch->text().trimmed().toStdString();
    if (keyword.empty()) {
        loadMovies();
    } else {
        auto movies = movieManager->searchByName(keyword);
        populateGrid(movies);
    }
}

void MovieListWidget::populateGrid(const DynamicArray<Movie>& movies) {
    // Clear old items from grid layout
    QLayoutItem *item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    if (movies.isEmpty()) {
        QLabel *lblEmpty = new QLabel("Không tìm thấy phim phù hợp.", cardsContainer);
        lblEmpty->setStyleSheet("color: #FFFFFF; font-size: 16px; margin: 40px;");
        lblEmpty->setAlignment(Qt::AlignCenter);
        gridLayout->addWidget(lblEmpty, 0, 0, 1, 2);
        return;
    }

    int row = 0;
    int col = 0;
    for (int i = 0; i < movies.size(); ++i) {
        QWidget *card = createMovieCard(movies[i]);
        gridLayout->addWidget(card, row, col);
        col++;
        if (col >= 2) {
            col = 0;
            row++;
        }
    }
}

QWidget* MovieListWidget::createMovieCard(const Movie& movie) {
    QFrame *card = new QFrame(cardsContainer);
    card->setObjectName("movieCard");
    card->setStyleSheet(R"(
        QFrame#movieCard {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2C2E6A, stop:1 #20224F);
            border: 1px solid #4A4D94;
            border-radius: 10px;
        }
        QFrame#movieCard:hover {
            border: 1px solid #E6B042;
        }
    )");

    QHBoxLayout *cardLayout = new QHBoxLayout(card);
    cardLayout->setContentsMargins(12, 12, 12, 12);
    cardLayout->setSpacing(14);

    // Left: Poster
    QLabel *lblPoster = new QLabel(card);
    lblPoster->setFixedSize(130, 185);
    lblPoster->setAlignment(Qt::AlignCenter);
    lblPoster->setStyleSheet("background-color: #121330; border-radius: 6px; border: 1px solid #3D4080;");

    QString posterPath = QString::fromStdString(movie.getPoster());
    QPixmap pixmap;
    if (QFileInfo::exists(posterPath)) {
        pixmap.load(posterPath);
    } else {
        QString alt1 = "posters/" + QFileInfo(posterPath).fileName();
        if (QFileInfo::exists(alt1)) {
            pixmap.load(alt1);
        } else {
            QString alt2 = QCoreApplication::applicationDirPath() + "/" + posterPath;
            if (QFileInfo::exists(alt2)) {
                pixmap.load(alt2);
            }
        }
    }

    if (!pixmap.isNull()) {
        lblPoster->setPixmap(pixmap.scaled(130, 185, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    } else {
        lblPoster->setText("POSTER\n" + QString::fromStdString(movie.getTenPhim()));
        lblPoster->setWordWrap(true);
        lblPoster->setStyleSheet("color: #F5C542; font-size: 11px; font-weight: bold; background-color: #1A1B41; border-radius: 6px;");
    }

    // Right: Info
    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(6);
    infoLayout->setContentsMargins(0, 0, 0, 0);

    QString titleText = QString::fromStdString(movie.getTenPhim()).toUpper();
    if (movie.getDoTuoi() > 0) {
        titleText += QString(" (T%1)").arg(movie.getDoTuoi());
    }
    QLabel *lblTitle = new QLabel(titleText, card);
    lblTitle->setStyleSheet("color: #FFFFFF; font-size: 15px; font-weight: bold; background: transparent;");
    lblTitle->setWordWrap(true);

    QLabel *lblGenre = new QLabel("🏷️ Thể loại: " + QString::fromStdString(movie.getTheLoai()), card);
    lblGenre->setStyleSheet("color: #D6D6E6; font-size: 12px; background: transparent;");

    QLabel *lblDuration = new QLabel(QString("⏱️ Thời lượng: %1 phút").arg(movie.getThoiLuong()), card);
    lblDuration->setStyleSheet("color: #D6D6E6; font-size: 12px; background: transparent;");

    QLabel *lblAge = new QLabel(QString("🔞 Giới hạn: %1+").arg(movie.getDoTuoi()), card);
    lblAge->setStyleSheet("color: #D6D6E6; font-size: 12px; background: transparent;");

    infoLayout->addWidget(lblTitle);
    infoLayout->addWidget(lblGenre);
    infoLayout->addWidget(lblDuration);
    infoLayout->addWidget(lblAge);
    infoLayout->addStretch();

    // Button: Dat ve / Chi tiet
    QPushButton *btnBook = new QPushButton("Đặt Vé / Chi Tiết", card);
    btnBook->setCursor(Qt::PointingHandCursor);
    btnBook->setStyleSheet(R"(
        QPushButton {
            background-color: #9E221E;
            color: #FFFFFF;
            font-size: 13px;
            font-weight: bold;
            border-radius: 5px;
            border: none;
            min-height: 32px;
            padding: 4px 12px;
        }
        QPushButton:hover {
            background-color: #B52A26;
            border: 1px solid #E6B042;
        }
    )");

    QString maPhim = QString::fromStdString(movie.getMaPhim());
    connect(btnBook, &QPushButton::clicked, this, [this, maPhim]() {
        emit movieSelected(maPhim);
    });

    infoLayout->addWidget(btnBook);

    cardLayout->addWidget(lblPoster);
    cardLayout->addLayout(infoLayout, 1);

    return card;
}
