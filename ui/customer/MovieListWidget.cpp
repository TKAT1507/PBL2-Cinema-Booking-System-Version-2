#include "MovieListWidget.h"
#include "../common/Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QPixmap>
#include <QCursor>

MovieListWidget::MovieListWidget(MovieManager* movieManager, ShowtimeManager* showtimeManager, QWidget *parent)
    : QWidget(parent), movieManager(movieManager), showtimeManager(showtimeManager)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(26, 22, 26, 18);
    mainLayout->setSpacing(16);

    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->setSpacing(12);

    QVBoxLayout *titleCol = new QVBoxLayout();
    titleCol->setSpacing(2);

    QLabel *lblEyebrow = new QLabel("NOW SHOWING • PHÒNG VÉ TRỰC TUYẾN", this);
    lblEyebrow->setStyleSheet(Theme::sectionEyebrowStyle());

    QLabel *lblSectionTitle = new QLabel("PHIM ĐANG CHIẾU", this);
    lblSectionTitle->setStyleSheet(Theme::sectionTitleStyle());

    titleCol->addWidget(lblEyebrow);
    titleCol->addWidget(lblSectionTitle);

    topBar->addLayout(titleCol);
    topBar->addStretch();

    txtSearch = new QLineEdit(this);
    txtSearch->setPlaceholderText("Tìm kiếm tên phim...");
    txtSearch->setFixedWidth(260);
    txtSearch->setFixedHeight(40);

    btnSearch = new QPushButton("Tìm Kiếm", this);
    btnSearch->setCursor(Qt::PointingHandCursor);
    btnSearch->setStyleSheet(Theme::primaryButtonStyle(38, 13));

    topBar->addWidget(txtSearch, 0, Qt::AlignVCenter);
    topBar->addWidget(btnSearch, 0, Qt::AlignVCenter);
    mainLayout->addLayout(topBar);

    QFrame *divider = new QFrame(this);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("background-color: #2B304D; max-height: 1px; border: none;");
    mainLayout->addWidget(divider);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { background: transparent; border: none; }");

    cardsContainer = new QWidget(scrollArea);
    cardsContainer->setStyleSheet("background: transparent;");
    gridLayout = new QGridLayout(cardsContainer);
    gridLayout->setSpacing(18);
    gridLayout->setContentsMargins(0, 4, 6, 16);
    gridLayout->setAlignment(Qt::AlignTop);
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);

    scrollArea->setWidget(cardsContainer);
    mainLayout->addWidget(scrollArea, 1);

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
    QLayoutItem *item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    if (movies.isEmpty()) {
        QLabel *lblEmpty = new QLabel("Không tìm thấy bộ phim phù hợp với từ khóa của bạn.", cardsContainer);
        lblEmpty->setStyleSheet("color: #9AA2C2; font-size: 15px; padding: 50px;");
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
    card->setFixedHeight(220);
    card->setStyleSheet(R"(
        QFrame#movieCard {
            background-color: #22263C;
            border: 1px solid #323859;
            border-radius: 18px;
        }
        QFrame#movieCard:hover {
            background-color: #282D47;
            border: 1.5px solid #FF6B6B;
        }
    )");

    QHBoxLayout *cardLayout = new QHBoxLayout(card);
    cardLayout->setContentsMargins(14, 14, 18, 14);
    cardLayout->setSpacing(16);

    QLabel *lblPoster = new QLabel(card);
    lblPoster->setFixedSize(130, 190);
    lblPoster->setAlignment(Qt::AlignCenter);
    lblPoster->setStyleSheet("background-color: #181B2C; border-radius: 14px; border: none;");

    QPixmap pixmap = Theme::loadPosterPixmap(QString::fromStdString(movie.getPoster()));
    if (!pixmap.isNull()) {
        lblPoster->setPixmap(Theme::roundedCoverPixmap(pixmap, 130, 190, 14));
    } else {
        lblPoster->setText("POSTER\n" + QString::fromStdString(movie.getTenPhim()));
        lblPoster->setWordWrap(true);
        lblPoster->setStyleSheet("color: #848DB0; font-size: 11px; font-weight: bold; background-color: #181B2C; border-radius: 14px; padding: 8px;");
    }

    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(6);
    infoLayout->setContentsMargins(0, 4, 0, 2);

    QString titleText = QString::fromStdString(movie.getTenPhim());
    QLabel *lblTitle = new QLabel(titleText, card);
    lblTitle->setStyleSheet("color: #F3F4FA; font-size: 16px; font-weight: bold; background: transparent; border: none;");
    lblTitle->setWordWrap(true);

    QLabel *lblGenre = new QLabel("Thể loại:  " + QString::fromStdString(movie.getTheLoai()), card);
    lblGenre->setStyleSheet("color: #A3ABC9; font-size: 12px; background: transparent; border: none;");
    lblGenre->setWordWrap(true);

    QLabel *lblDuration = new QLabel(QString("Thời lượng:  %1 phút").arg(movie.getThoiLuong()), card);
    lblDuration->setStyleSheet("color: #A3ABC9; font-size: 12px; background: transparent; border: none;");

    QHBoxLayout *ageRow = new QHBoxLayout();
    ageRow->setSpacing(6);
    QLabel *lblAge = new QLabel(Theme::ageBadgeText(movie.getDoTuoi(), true), card);
    lblAge->setStyleSheet(Theme::ageBadgeStyle(movie.getDoTuoi()));
    ageRow->addWidget(lblAge);
    ageRow->addStretch();

    infoLayout->addWidget(lblTitle);
    infoLayout->addWidget(lblGenre);
    infoLayout->addWidget(lblDuration);
    infoLayout->addSpacing(2);
    infoLayout->addLayout(ageRow);
    infoLayout->addStretch();

    QPushButton *btnBook = new QPushButton("Đặt Vé && Chi Tiết", card);
    btnBook->setCursor(Qt::PointingHandCursor);
    btnBook->setStyleSheet(Theme::primaryButtonStyle(36, 13));

    QString maPhim = QString::fromStdString(movie.getMaPhim());
    connect(btnBook, &QPushButton::clicked, this, [this, maPhim]() {
        emit movieSelected(maPhim);
    });

    infoLayout->addWidget(btnBook);

    cardLayout->addWidget(lblPoster);
    cardLayout->addLayout(infoLayout, 1);

    return card;
}
