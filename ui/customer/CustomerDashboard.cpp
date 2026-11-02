#include "CustomerDashboard.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QFrame>
#include "MovieListWidget.h"
#include "MovieDetailWidget.h"
#include "ShowtimeListWidget.h"
#include "SeatSelectionWidget.h"
#include "ComboSelectionWidget.h"
#include "BookingConfirmWidget.h"
#include "MyTicketsWidget.h"
#include "../common/ChangePasswordDialog.h"

CustomerDashboard::CustomerDashboard(const QString& maKH, UserManager* userMgr, MovieManager* movieMgr, 
                                     ShowtimeManager* showtimeMgr, TicketManager* ticketMgr, 
                                     ComboManager* comboMgr, RoomManager* roomMgr, QWidget *parent)
    : QWidget(parent), maKhachHang(maKH), userManager(userMgr), movieManager(movieMgr),
      showtimeManager(showtimeMgr), ticketManager(ticketMgr), comboManager(comboMgr), roomManager(roomMgr)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 14, 0);
    mainLayout->setSpacing(14);
    
    // Left Sidebar: Dính sát viền trái, vuông vức (border-radius: 0px), màu vàng mù tạt #EAB02F
    QWidget *sidebarWidget = new QWidget(this);
    sidebarWidget->setFixedWidth(240);
    sidebarWidget->setStyleSheet(R"(
        QWidget {
            background-color: #EAB02F;
            border: none;
            border-radius: 0px;
        }
    )");

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(12, 16, 12, 16);
    sidebarLayout->setSpacing(10);
    sidebarLayout->setAlignment(Qt::AlignTop);

    // Top Header: Bo tròn như cũ, màu đỏ gạch #A52C20
    QFrame *headerFrame = new QFrame(sidebarWidget);
    headerFrame->setStyleSheet(R"(
        QFrame {
            background-color: #A52C20;
            border-radius: 8px;
            padding: 8px;
        }
    )");
    QVBoxLayout *headerLayout = new QVBoxLayout(headerFrame);
    headerLayout->setContentsMargins(5, 8, 5, 8);
    headerLayout->setSpacing(4);

    QLabel *lblGreeting = new QLabel("Xin chào", headerFrame);
    lblGreeting->setAlignment(Qt::AlignCenter);
    lblGreeting->setStyleSheet("color: #FFFFFF; font-size: 13px; font-weight: normal; background: transparent; border: none;");

    QLabel *lblUser = new QLabel("Khách hàng\n" + maKhachHang, headerFrame);
    lblUser->setAlignment(Qt::AlignCenter);
    lblUser->setStyleSheet("color: #FFFFFF; font-size: 15px; font-weight: bold; background: transparent; border: none;");

    headerLayout->addWidget(lblGreeting);
    headerLayout->addWidget(lblUser);

    // Navigation Tabs Style: Bo tròn như cũ, màu kem #F9F1E5
    QString tabBtnStyle = R"(
        QPushButton {
            background-color: #F9F1E5;
            color: #1A1A1A;
            border: 1px solid #2B1810;
            border-radius: 6px;
            font-size: 14px;
            font-weight: bold;
            min-height: 40px;
            padding: 6px 12px;
            text-align: center;
        }
        QPushButton:hover {
            background-color: #FFFFFF;
            border-color: #A52C20;
        }
        QPushButton:pressed {
            background-color: #EAB02F;
        }
    )";

    QPushButton *btnMovies = new QPushButton("Danh Sách Phim", sidebarWidget);
    btnMovies->setCursor(Qt::PointingHandCursor);
    btnMovies->setStyleSheet(tabBtnStyle);

    QPushButton *btnTickets = new QPushButton("Vé Của Tôi", sidebarWidget);
    btnTickets->setCursor(Qt::PointingHandCursor);
    btnTickets->setStyleSheet(tabBtnStyle);

    // Bottom action buttons: Bo tròn như cũ
    QPushButton *btnChangePassword = new QPushButton("Đổi Mật Khẩu", sidebarWidget);
    btnChangePassword->setCursor(Qt::PointingHandCursor);
    btnChangePassword->setStyleSheet(R"(
        QPushButton {
            background-color: #F9F1E5;
            color: #A52C20;
            border: 1px solid #A52C20;
            border-radius: 6px;
            font-size: 13px;
            font-weight: bold;
            min-height: 38px;
        }
        QPushButton:hover {
            background-color: #A52C20;
            color: #FFFFFF;
        }
    )");

    QPushButton *btnLogout = new QPushButton("Đăng Xuất", sidebarWidget);
    btnLogout->setCursor(Qt::PointingHandCursor);
    btnLogout->setStyleSheet(R"(
        QPushButton {
            background-color: #A52C20;
            color: #FFFFFF;
            border: 1px solid #822017;
            border-radius: 6px;
            font-size: 14px;
            font-weight: bold;
            min-height: 38px;
        }
        QPushButton:hover {
            background-color: #BD362A;
        }
    )");

    sidebarLayout->addWidget(headerFrame);
    sidebarLayout->addSpacing(15);
    sidebarLayout->addWidget(btnMovies);
    sidebarLayout->addWidget(btnTickets);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(btnChangePassword);
    sidebarLayout->addWidget(btnLogout);

    // Main Content Area (flat background #212156)
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setStyleSheet(R"(
        QStackedWidget {
            background: transparent;
        }
    )");
    
    // Sub-widgets
    movieListWidget = new MovieListWidget(movieManager, showtimeManager, this);
    movieDetailWidget = new MovieDetailWidget(movieManager, this);
    showtimeListWidget = new ShowtimeListWidget(showtimeManager, movieManager, this);
    seatSelectionWidget = new SeatSelectionWidget(showtimeManager, ticketManager, this);
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

    mainLayout->addWidget(sidebarWidget, 0);
    mainLayout->addWidget(stackedWidget, 1);

    // Sidebar Navigation
    connect(btnMovies, &QPushButton::clicked, this, [this]() { 
        movieListWidget->loadMovies();
        stackedWidget->setCurrentWidget(movieListWidget); 
    });
    connect(btnTickets, &QPushButton::clicked, this, [this]() { 
        myTicketsWidget->loadTickets();
        stackedWidget->setCurrentWidget(myTicketsWidget); 
    });
    connect(btnChangePassword, &QPushButton::clicked, this, [this]() {
        std::string taiKhoan = maKhachHang.toStdString();
        Customer* c = userManager ? userManager->findCustomerById(taiKhoan) : nullptr;
        if (c) taiKhoan = c->getTaiKhoan();
        ChangePasswordDialog dlg(userManager, taiKhoan, this);
        dlg.exec();
    });
    connect(btnLogout, &QPushButton::clicked, this, &CustomerDashboard::logout);

    // MovieList -> MovieDetail / Direct Booking
    connect(movieListWidget, &MovieListWidget::movieSelected, this, [this](const QString& maPhim) {
        movieDetailWidget->showMovie(maPhim);
        stackedWidget->setCurrentWidget(movieDetailWidget);
    });

    // MovieDetail Navigation
    connect(movieDetailWidget, &MovieDetailWidget::goBack, this, [this]() {
        stackedWidget->setCurrentWidget(movieListWidget);
    });
    connect(movieDetailWidget, &MovieDetailWidget::bookMovie, this, [this](const QString& maPhim) {
        showtimeListWidget->setMovieFilter(maPhim);
        showtimeListWidget->loadShowtimes();
        stackedWidget->setCurrentWidget(showtimeListWidget);
    });

    // ShowtimeList Navigation
    connect(showtimeListWidget, &ShowtimeListWidget::goBack, this, [this]() {
        stackedWidget->setCurrentWidget(movieDetailWidget);
    });
    connect(showtimeListWidget, &ShowtimeListWidget::showtimeSelected, this, [this](const QString& maSuatChieu) {
        tempMaSuatChieu = maSuatChieu;
        seatSelectionWidget->loadSeatMap(maSuatChieu);
        stackedWidget->setCurrentWidget(seatSelectionWidget);
    });

    // SeatSelection Navigation
    connect(seatSelectionWidget, &SeatSelectionWidget::goBack, this, [this]() {
        stackedWidget->setCurrentWidget(showtimeListWidget);
    });
    connect(seatSelectionWidget, &SeatSelectionWidget::seatsSelected, this, [this](const QString& maSuatChieu, const QStringList& selectedSeats) {
        tempMaSuatChieu = maSuatChieu;
        tempSelectedSeats = selectedSeats;
        comboSelectionWidget->loadCombos();
        stackedWidget->setCurrentWidget(comboSelectionWidget);
    });

    // ComboSelection Navigation
    connect(comboSelectionWidget, &ComboSelectionWidget::goBack, this, [this]() {
        stackedWidget->setCurrentWidget(seatSelectionWidget);
    });
    connect(comboSelectionWidget, &ComboSelectionWidget::combosSelected, this, [this](const QStringList& comboInfos) {
        bookingConfirmWidget->setBookingData(tempMaSuatChieu, tempSelectedSeats, comboInfos);
        stackedWidget->setCurrentWidget(bookingConfirmWidget);
    });

    // BookingConfirm Navigation
    connect(bookingConfirmWidget, &BookingConfirmWidget::goBack, this, [this]() {
        stackedWidget->setCurrentWidget(comboSelectionWidget);
    });
    connect(bookingConfirmWidget, &BookingConfirmWidget::bookingComplete, this, [this]() {
        myTicketsWidget->loadTickets();
        stackedWidget->setCurrentWidget(myTicketsWidget);
    });
}
