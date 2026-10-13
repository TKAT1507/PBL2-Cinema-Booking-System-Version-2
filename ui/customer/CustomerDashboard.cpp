#include "CustomerDashboard.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include "MovieListWidget.h"
#include "MovieDetailWidget.h"
#include "ShowtimeListWidget.h"
#include "SeatSelectionWidget.h"
#include "ComboSelectionWidget.h"
#include "BookingConfirmWidget.h"
#include "MyTicketsWidget.h"

CustomerDashboard::CustomerDashboard(const QString& maKH, UserManager* userMgr, MovieManager* movieMgr, 
                                     ShowtimeManager* showtimeMgr, TicketManager* ticketMgr, 
                                     ComboManager* comboMgr, RoomManager* roomMgr, QWidget *parent)
    : QWidget(parent), maKhachHang(maKH), userManager(userMgr), movieManager(movieMgr),
      showtimeManager(showtimeMgr), ticketManager(ticketMgr), comboManager(comboMgr), roomManager(roomMgr)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(12);
    
    // Sidebar container
    QWidget *sidebarWidget = new QWidget(this);
    sidebarWidget->setStyleSheet("background-color: #121224; border-radius: 10px;");
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(15, 20, 15, 20);
    sidebarLayout->setSpacing(12);
    sidebarLayout->setAlignment(Qt::AlignTop);

    QLabel *lblWelcome = new QLabel("KHACH HANG\n" + maKhachHang, sidebarWidget);
    lblWelcome->setAlignment(Qt::AlignCenter);
    lblWelcome->setStyleSheet("color: #ffd700; font-size: 15px; font-weight: bold; border-bottom: 2px solid #0f3460; padding-bottom: 12px;");

    QPushButton *btnMovies = new QPushButton("Danh Sach Phim", sidebarWidget);
    QPushButton *btnTickets = new QPushButton("Ve Cua Toi", sidebarWidget);
    QPushButton *btnLogout = new QPushButton("Dang Xuat", sidebarWidget);
    btnLogout->setStyleSheet("background-color: #5a0c0e; color: #ffcccc; border: 1px solid #7b1113; font-weight: bold; border-radius: 6px;");

    sidebarLayout->addWidget(lblWelcome);
    sidebarLayout->addSpacing(10);
    sidebarLayout->addWidget(btnMovies);
    sidebarLayout->addWidget(btnTickets);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(btnLogout);

    // Main Area
    stackedWidget = new QStackedWidget(this);
    
    // Sub-widgets
    movieListWidget = new MovieListWidget(movieManager, this);
    movieDetailWidget = new MovieDetailWidget(movieManager, this);
    showtimeListWidget = new ShowtimeListWidget(showtimeManager, movieManager, this);
    seatSelectionWidget = new SeatSelectionWidget(showtimeManager, this);
    comboSelectionWidget = new ComboSelectionWidget(comboManager, this);
    bookingConfirmWidget = new BookingConfirmWidget(maKhachHang, ticketManager, showtimeManager, comboManager, this);
    myTicketsWidget = new MyTicketsWidget(maKhachHang, ticketManager, showtimeManager, movieManager, this);
    
    stackedWidget->addWidget(movieListWidget);        // 0
    stackedWidget->addWidget(movieDetailWidget);      // 1
    stackedWidget->addWidget(showtimeListWidget);     // 2
    stackedWidget->addWidget(seatSelectionWidget);    // 3
    stackedWidget->addWidget(comboSelectionWidget);   // 4
    stackedWidget->addWidget(bookingConfirmWidget);   // 5
    stackedWidget->addWidget(myTicketsWidget);        // 6

    mainLayout->addWidget(sidebarWidget, 1);
    mainLayout->addWidget(stackedWidget, 4);

    // 1. Sidebar Navigation
    connect(btnMovies, &QPushButton::clicked, this, [this]() { 
        movieListWidget->loadMovies();
        stackedWidget->setCurrentWidget(movieListWidget); 
    });
    connect(btnTickets, &QPushButton::clicked, this, [this]() { 
        myTicketsWidget->loadTickets();
        stackedWidget->setCurrentWidget(myTicketsWidget); 
    });
    connect(btnLogout, &QPushButton::clicked, this, &CustomerDashboard::logout);

    // 2. MovieList -> MovieDetail
    connect(movieListWidget, &MovieListWidget::movieSelected, this, [this](const QString& maPhim) {
        movieDetailWidget->showMovie(maPhim);
        stackedWidget->setCurrentWidget(movieDetailWidget);
    });

    // 3. MovieDetail Navigation
    connect(movieDetailWidget, &MovieDetailWidget::goBack, this, [this]() {
        stackedWidget->setCurrentWidget(movieListWidget);
    });
    connect(movieDetailWidget, &MovieDetailWidget::bookMovie, this, [this](const QString& maPhim) {
        showtimeListWidget->setMovieFilter(maPhim);
        showtimeListWidget->loadShowtimes();
        stackedWidget->setCurrentWidget(showtimeListWidget);
    });

    // 4. ShowtimeList Navigation
    connect(showtimeListWidget, &ShowtimeListWidget::goBack, this, [this]() {
        stackedWidget->setCurrentWidget(movieDetailWidget);
    });
    connect(showtimeListWidget, &ShowtimeListWidget::showtimeSelected, this, [this](const QString& maSuatChieu) {
        tempMaSuatChieu = maSuatChieu;
        seatSelectionWidget->loadSeatMap(maSuatChieu);
        stackedWidget->setCurrentWidget(seatSelectionWidget);
    });

    // 5. SeatSelection Navigation
    connect(seatSelectionWidget, &SeatSelectionWidget::goBack, this, [this]() {
        stackedWidget->setCurrentWidget(showtimeListWidget);
    });
    connect(seatSelectionWidget, &SeatSelectionWidget::seatsSelected, this, [this](const QString& maSuatChieu, const QStringList& selectedSeats) {
        tempMaSuatChieu = maSuatChieu;
        tempSelectedSeats = selectedSeats;
        comboSelectionWidget->loadCombos();
        stackedWidget->setCurrentWidget(comboSelectionWidget);
    });

    // 6. ComboSelection Navigation
    connect(comboSelectionWidget, &ComboSelectionWidget::goBack, this, [this]() {
        stackedWidget->setCurrentWidget(seatSelectionWidget);
    });
    connect(comboSelectionWidget, &ComboSelectionWidget::combosSelected, this, [this](const QStringList& comboInfos) {
        bookingConfirmWidget->setBookingData(tempMaSuatChieu, tempSelectedSeats, comboInfos);
        stackedWidget->setCurrentWidget(bookingConfirmWidget);
    });

    // 7. BookingConfirm Navigation
    connect(bookingConfirmWidget, &BookingConfirmWidget::goBack, this, [this]() {
        stackedWidget->setCurrentWidget(comboSelectionWidget);
    });
    connect(bookingConfirmWidget, &BookingConfirmWidget::bookingComplete, this, [this]() {
        myTicketsWidget->loadTickets();
        stackedWidget->setCurrentWidget(myTicketsWidget);
    });
}
