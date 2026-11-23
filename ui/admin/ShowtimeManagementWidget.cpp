#include "ShowtimeManagementWidget.h"
#include "../common/Theme.h"
#include "managers/ShowtimeManager.h"
#include "managers/MovieManager.h"
#include "managers/RoomManager.h"
#include "models/Showtime.h"
#include "models/Movie.h"
#include "models/Room.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QDialog>
#include <QComboBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QLabel>

ShowtimeManagementWidget::ShowtimeManagementWidget(ShowtimeManager* showtimeMgr, MovieManager* movieMgr, RoomManager* roomMgr, QWidget* parent)
    : QWidget(parent), showtimeManager(showtimeMgr), movieManager(movieMgr), roomManager(roomMgr)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(26, 22, 26, 20);
    layout->setSpacing(14);

    QHBoxLayout* topRow = new QHBoxLayout();
    QVBoxLayout* titleCol = new QVBoxLayout();
    titleCol->setSpacing(2);
    QLabel* lblEyebrow = new QLabel("ĐIỀU PHỐI LỊCH CHIẾU PHIM", this);
    lblEyebrow->setStyleSheet(Theme::sectionEyebrowStyle());
    QLabel* lblTitle = new QLabel("QUẢN LÝ SUẤT CHIẾU", this);
    lblTitle->setStyleSheet(Theme::sectionTitleStyle());
    titleCol->addWidget(lblEyebrow);
    titleCol->addWidget(lblTitle);

    topRow->addLayout(titleCol);
    topRow->addStretch();

    btnAdd = new QPushButton("+ Thêm Suất Chiếu", this);
    btnAdd->setCursor(Qt::PointingHandCursor);
    btnAdd->setStyleSheet(Theme::primaryButtonStyle(38, 13));

    btnDelete = new QPushButton("Hủy Suất Chiếu", this);
    btnDelete->setCursor(Qt::PointingHandCursor);
    btnDelete->setStyleSheet(Theme::dangerOutlineButtonStyle(38, 13));

    topRow->addWidget(btnAdd, 0, Qt::AlignVCenter);
    topRow->addWidget(btnDelete, 0, Qt::AlignVCenter);
    layout->addLayout(topRow);

    table = new QTableWidget(0, 6, this);
    table->setHorizontalHeaderLabels({"Mã Suất", "Tên Phim", "Phòng Chiếu", "Ngày Chiếu", "Giờ Chiếu", "Trạng Thái"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setDefaultSectionSize(46);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);

    layout->addWidget(table, 1);

    connect(btnAdd, &QPushButton::clicked, this, &ShowtimeManagementWidget::addShowtime);
    connect(btnDelete, &QPushButton::clicked, this, &ShowtimeManagementWidget::deleteShowtime);

    loadData();
}

void ShowtimeManagementWidget::loadData() {
    table->setRowCount(0);
    auto& showtimes = showtimeManager->getAllShowtimes();
    QFont boldFont = table->font();
    boldFont.setBold(true);

    for (int i = 0; i < showtimes.size(); ++i) {
        table->insertRow(i);

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::fromStdString(showtimes[i].getMaSuatChieu()));
        idItem->setTextAlignment(Qt::AlignCenter);
        idItem->setFont(boldFont);
        idItem->setForeground(QBrush(QColor("#8C92FF")));
        table->setItem(i, 0, idItem);

        Movie* mv = movieManager->findById(showtimes[i].getMaPhim());
        table->setItem(i, 1, new QTableWidgetItem(mv ? QString::fromStdString(mv->getTenPhim()) : "Không xác định"));

        QTableWidgetItem* roomItem = new QTableWidgetItem(QString::fromStdString(showtimes[i].getMaPhong()));
        roomItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 2, roomItem);

        QTableWidgetItem* dateItem = new QTableWidgetItem(QString::fromStdString(showtimes[i].getNgayChieu()));
        dateItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 3, dateItem);

        QTableWidgetItem* timeItem = new QTableWidgetItem(QString::fromStdString(showtimes[i].getGioChieu()));
        timeItem->setTextAlignment(Qt::AlignCenter);
        timeItem->setFont(boldFont);
        timeItem->setForeground(QBrush(QColor("#FFD166")));
        table->setItem(i, 4, timeItem);

        bool active = showtimes[i].getTrangThai();
        QTableWidgetItem* statusItem = new QTableWidgetItem(active ? "Sắp chiếu" : "Đã hủy / Chiếu xong");
        statusItem->setTextAlignment(Qt::AlignCenter);
        statusItem->setFont(boldFont);
        statusItem->setForeground(QBrush(QColor(active ? "#34D399" : "#707899")));
        table->setItem(i, 5, statusItem);
    }
}

void ShowtimeManagementWidget::addShowtime() {
    auto movies = movieManager->getActiveMovies();
    auto rooms = roomManager->getActiveRooms();
    if (movies.size() == 0 || rooms.size() == 0) {
        QMessageBox::warning(this, "Lỗi", "Không đủ dữ liệu phim hoặc phòng chiếu đang hoạt động.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Thêm Suất Chiếu Mới");
    dialog.setFixedWidth(450);
    dialog.setStyleSheet("QDialog { background-color: #1F2338; border: 1px solid #353C61; border-radius: 18px; }");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(26, 24, 26, 24);
    layout->setSpacing(10);

    QLabel *dlgTitle = new QLabel("Tạo Suất Chiếu Mới", &dialog);
    dlgTitle->setStyleSheet("color: #FFD166; font-size: 18px; font-weight: bold;");
    layout->addWidget(dlgTitle);

    auto lbl = [&dialog](const QString& t) {
        QLabel* l = new QLabel(t, &dialog);
        l->setStyleSheet("color: #A3ABC9; font-size: 12px; font-weight: 600;");
        return l;
    };

    QComboBox *cbPhim = new QComboBox(&dialog);
    for (int i = 0; i < movies.size(); i++)
        cbPhim->addItem(QString::fromStdString(movies[i].getTenPhim()), QString::fromStdString(movies[i].getMaPhim()));

    QComboBox *cbPhong = new QComboBox(&dialog);
    for (int i = 0; i < rooms.size(); i++)
        cbPhong->addItem(QString::fromStdString(rooms[i].getTenPhong()), QString::fromStdString(rooms[i].getMaPhong()));

    QDateEdit *edtNgay = new QDateEdit(QDate::currentDate(), &dialog);
    edtNgay->setCalendarPopup(true);
    edtNgay->setDisplayFormat("dd/MM/yyyy");

    QTimeEdit *edtGio = new QTimeEdit(QTime(18, 0), &dialog);
    edtGio->setDisplayFormat("HH:mm");

    layout->addWidget(lbl("Chọn phim:")); layout->addWidget(cbPhim);
    layout->addWidget(lbl("Chọn phòng chiếu:")); layout->addWidget(cbPhong);
    layout->addWidget(lbl("Ngày chiếu:")); layout->addWidget(edtNgay);
    layout->addWidget(lbl("Giờ chiếu:")); layout->addWidget(edtGio);
    layout->addSpacing(8);

    QPushButton *btnCancel = new QPushButton("Hủy Bỏ", &dialog);
    btnCancel->setCursor(Qt::PointingHandCursor);
    btnCancel->setStyleSheet(Theme::secondaryButtonStyle(40, 13));

    QPushButton *btnOk = new QPushButton("Thêm Suất Chiếu", &dialog);
    btnOk->setCursor(Qt::PointingHandCursor);
    btnOk->setStyleSheet(Theme::primaryButtonStyle(40, 13));

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);
    btnLayout->addWidget(btnCancel);
    btnLayout->addWidget(btnOk);
    layout->addLayout(btnLayout);

    connect(btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        std::string maPhim = cbPhim->currentData().toString().toStdString();
        std::string maPhong = cbPhong->currentData().toString().toStdString();
        std::string ngay = edtNgay->date().toString("dd/MM/yyyy").toStdString();
        std::string gio = edtGio->time().toString("HH:mm").toStdString();

        int movieIdx = cbPhim->currentIndex();
        int roomIdx = cbPhong->currentIndex();

        if (showtimeManager->hasTimeConflict(maPhong, ngay, gio, movies[movieIdx].getThoiLuong())) {
            QMessageBox::warning(this, "Trùng Lịch Chiếu", "Phòng chiếu đã có suất chiếu trùng khung giờ này!");
            return;
        }

        Showtime st("", maPhong, maPhim, ngay, gio, true);
        if (showtimeManager->addShowtime(st, rooms[roomIdx].getSoHang(), rooms[roomIdx].getSoCot(), 2)) {
            showtimeManager->save();
            showtimeManager->saveSeatMaps();
            loadData();
        }
    }
}

void ShowtimeManagementWidget::deleteShowtime() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Cảnh Báo", "Vui lòng chọn suất chiếu cần hủy!");
        return;
    }
    std::string id = table->item(row, 0)->text().toStdString();
    if (QMessageBox::question(this, "Xác Nhận", "Bạn có muốn hủy suất chiếu này?") == QMessageBox::Yes) {
        if (showtimeManager->cancelShowtime(id)) {
            showtimeManager->save();
            loadData();
        }
    }
}
