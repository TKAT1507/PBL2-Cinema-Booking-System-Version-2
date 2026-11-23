#include "CustomerDashboard.h"
#include "../common/Theme.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QMessageBox>
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
    Customer* currentCustomer = userManager ? userManager->findCustomerById(maKhachHang.toStdString()) : nullptr;
    int customerAge = currentCustomer ? currentCustomer->getTuoi() : 0;

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Left Sidebar — Soft Midnight Twilight
    QWidget *sidebarWidget = new QWidget(this);
    sidebarWidget->setObjectName("sidebarContainer");
    sidebarWidget->setAttribute(Qt::WA_StyledBackground, true);
    sidebarWidget->setFixedWidth(252);
    sidebarWidget->setStyleSheet(Theme::sidebarContainerStyle());

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(18, 24, 18, 20);
    sidebarLayout->setSpacing(10);
    sidebarLayout->setAlignment(Qt::AlignTop);

    QLabel *lblBrandMain = new QLabel("PBL2 CINEMA", sidebarWidget);
    lblBrandMain->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    lblBrandMain->setStyleSheet("color: #1E293B; font-size: 16px; font-weight: 600; background: transparent; padding-left: 6px;");


    QFrame *brandDivider = new QFrame(sidebarWidget);
    brandDivider->setFrameShape(QFrame::HLine);
    brandDivider->setStyleSheet("background-color: #E5E7EB; max-height: 1px; border: none;");

    sidebarLayout->addWidget(lblBrandMain);
    sidebarLayout->addSpacing(6);
    sidebarLayout->addWidget(brandDivider);
    sidebarLayout->addSpacing(6);

    // Profile Card
    QFrame *headerFrame = new QFrame(sidebarWidget);
    headerFrame->setObjectName("profileCard");
    headerFrame->setStyleSheet(Theme::sidebarProfileCardStyle());

    QVBoxLayout *headerLayout = new QVBoxLayout(headerFrame);
    headerLayout->setContentsMargins(14, 12, 14, 12);
    headerLayout->setSpacing(3);

    QLabel *lblRole = new QLabel("Khách hàng", headerFrame);
    lblRole->setStyleSheet("color: #D97706; font-size: 10px; font-weight: 600; background: transparent; border: none;");

    QString userDisplay = maKhachHang;
    if (customerAge > 0) {
        userDisplay += QString("  •  %1 tuổi").arg(customerAge);
    }
    QLabel *lblUser = new QLabel(userDisplay, headerFrame);
    lblUser->setStyleSheet("color: #1E293B; font-size: 14px; font-weight: 600; background: transparent; border: none;");

    headerLayout->addWidget(lblRole);
    headerLayout->addWidget(lblUser);
    sidebarLayout->addWidget(headerFrame);
    sidebarLayout->addSpacing(12);


    QPushButton *btnMovies = new QPushButton("  Danh Sách Phim", sidebarWidget);
    btnMovies->setCursor(Qt::PointingHandCursor);

    QPushButton *btnTickets = new QPushButton("  Vé Của Tôi", sidebarWidget);
    btnTickets->setCursor(Qt::PointingHandCursor);

    auto setActiveTab = [btnMovies, btnTickets](int index) {
        btnMovies->setStyleSheet(Theme::sidebarNavButtonStyle(index == 0));
        btnTickets->setStyleSheet(Theme::sidebarNavButtonStyle(index == 1));
    };
    setActiveTab(0);

    sidebarLayout->addWidget(btnMovies);
    sidebarLayout->addWidget(btnTickets);
    sidebarLayout->addStretch();

    QFrame *bottomDivider = new QFrame(sidebarWidget);
    bottomDivider->setFrameShape(QFrame::HLine);
    bottomDivider->setStyleSheet("background-color: #E5E7EB; max-height: 1px; border: none;");
    sidebarLayout->addWidget(bottomDivider);
    sidebarLayout->addSpacing(6);

    QPushButton *btnChangePassword = new QPushButton("Đổi Mật Khẩu", sidebarWidget);
    btnChangePassword->setCursor(Qt::PointingHandCursor);
    btnChangePassword->setStyleSheet(Theme::secondaryButtonStyle(40, 13));

    QPushButton *btnLogout = new QPushButton("Đăng Xuất", sidebarWidget);
    btnLogout->setCursor(Qt::PointingHandCursor);
    btnLogout->setStyleSheet(Theme::dangerOutlineButtonStyle(40, 13));

    sidebarLayout->addWidget(btnChangePassword);
    sidebarLayout->addWidget(btnLogout);

    // Main Content Area — Soft Midnight Canvas
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setStyleSheet("QStackedWidget { background-color: #F5F6F8; }");

    movieListWidget = new MovieListWidget(movieManager, showtimeManager, this);
    movieDetailWidget = new MovieDetailWidget(movieManager, this);
    if (customerAge > 0) {
        movieDetailWidget->setCustomerAge(customerAge);
    }
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

    connect(btnMovies, &QPushButton::clicked, this, [this, setActiveTab]() {
        setActiveTab(0);
        movieListWidget->loadMovies();
        stackedWidget->setCurrentWidget(movieListWidget);
    });
    connect(btnTickets, &QPushButton::clicked, this, [this, setActiveTab]() {
        setActiveTab(1);
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

    connect(movieListWidget, &MovieListWidget::movieSelected, this, [this, setActiveTab](const QString& maPhim) {
        setActiveTab(0);
        movieDetailWidget->showMovie(maPhim);
        stackedWidget->setCurrentWidget(movieDetailWidget);
    });

    connect(movieDetailWidget, &MovieDetailWidget::goBack, this, [this]() {
        stackedWidget->setCurrentWidget(movieListWidget);
    });
    connect(movieDetailWidget, &MovieDetailWidget::bookMovie, this, [this, customerAge](const QString& maPhim) {
        Movie* m = movieManager ? movieManager->findById(maPhim.toStdString()) : nullptr;
        if (m && m->getDoTuoi() > 0 && customerAge > 0 && customerAge < m->getDoTuoi()) {
            QMessageBox::warning(this, "Giới Hạn Độ Tuổi",
                QString("GIỚI HẠN ĐỘ TUỔI KHÁN GIẢ\n\n"
                        "Phim \"%1\" được dán nhãn C%2 (Dành cho khán giả từ đủ %2 tuổi trở lên).\n\n"
                        "Tài khoản của bạn hiện %3 tuổi, chưa đủ điều kiện theo quy định của Cục Điện ảnh để đặt vé xem phim này!")
                    .arg(QString::fromStdString(m->getTenPhim()))
                    .arg(m->getDoTuoi())
                    .arg(customerAge));
            return;
        }
        showtimeListWidget->setMovieFilter(maPhim);
        showtimeListWidget->loadShowtimes();
        stackedWidget->setCurrentWidget(showtimeListWidget);
    });

    connect(showtimeListWidget, &ShowtimeListWidget::goBack, this, [this]() {
        stackedWidget->setCurrentWidget(movieDetailWidget);
    });
    connect(showtimeListWidget, &ShowtimeListWidget::showtimeSelected, this, [this, customerAge](const QString& maSuatChieu) {
        Showtime* st = showtimeManager ? showtimeManager->findById(maSuatChieu.toStdString()) : nullptr;
        if (st) {
            Movie* m = movieManager ? movieManager->findById(st->getMaPhim()) : nullptr;
            if (m && m->getDoTuoi() > 0 && customerAge > 0 && customerAge < m->getDoTuoi()) {
                QMessageBox::warning(this, "Giới Hạn Độ Tuổi",
                    QString("GIỚI HẠN ĐỘ TUỔI KHÁN GIẢ\n\n"
                            "Suất chiếu phim \"%1\" được dán nhãn C%2 (Dành cho khán giả từ đủ %2 tuổi trở lên).\n\n"
                            "Tài khoản của bạn hiện %3 tuổi, chưa đủ điều kiện theo quy định của Cục Điện ảnh để đặt vé suất chiếu này!")
                        .arg(QString::fromStdString(m->getTenPhim()))
                        .arg(m->getDoTuoi())
                        .arg(customerAge));
                return;
            }
        }
        tempMaSuatChieu = maSuatChieu;
        seatSelectionWidget->loadSeatMap(maSuatChieu);
        stackedWidget->setCurrentWidget(seatSelectionWidget);
    });

    connect(seatSelectionWidget, &SeatSelectionWidget::goBack, this, [this]() {
        stackedWidget->setCurrentWidget(showtimeListWidget);
    });
    connect(seatSelectionWidget, &SeatSelectionWidget::seatsSelected, this, [this](const QString& maSuatChieu, const QStringList& selectedSeats) {
        tempMaSuatChieu = maSuatChieu;
        tempSelectedSeats = selectedSeats;
        comboSelectionWidget->loadCombos();
        stackedWidget->setCurrentWidget(comboSelectionWidget);
    });

    connect(comboSelectionWidget, &ComboSelectionWidget::goBack, this, [this]() {
        stackedWidget->setCurrentWidget(seatSelectionWidget);
    });
    connect(comboSelectionWidget, &ComboSelectionWidget::combosSelected, this, [this](const QStringList& comboInfos) {
        bookingConfirmWidget->setBookingData(tempMaSuatChieu, tempSelectedSeats, comboInfos);
        stackedWidget->setCurrentWidget(bookingConfirmWidget);
    });

    connect(bookingConfirmWidget, &BookingConfirmWidget::goBack, this, [this]() {
        stackedWidget->setCurrentWidget(comboSelectionWidget);
    });
    connect(bookingConfirmWidget, &BookingConfirmWidget::bookingComplete, this, [this, setActiveTab]() {
        setActiveTab(1);
        myTicketsWidget->loadTickets();
        stackedWidget->setCurrentWidget(myTicketsWidget);
    });
}
