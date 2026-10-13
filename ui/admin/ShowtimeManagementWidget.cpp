#include "ShowtimeManagementWidget.h"
#include "managers/ShowtimeManager.h"
#include "managers/MovieManager.h"
#include "managers/RoomManager.h"
#include "models/Showtime.h"
#include "models/Movie.h"
#include "models/Room.h"
#include "models/SeatMap.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QDialog>
#include <QComboBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QLabel>
#include "utils/IDGenerator.h"

ShowtimeManagementWidget::ShowtimeManagementWidget(ShowtimeManager* showtimeMgr, MovieManager* movieMgr, RoomManager* roomMgr, QWidget* parent)
    : QWidget(parent), showtimeManager(showtimeMgr), movieManager(movieMgr), roomManager(roomMgr) {
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnAdd = new QPushButton("Them Suat Chieu", this);
    btnDelete = new QPushButton("Huy Suat Chieu", this);
    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnDelete);
    
    table = new QTableWidget(0, 6, this);
    table->setHorizontalHeaderLabels({"Ma SC", "Phim", "Phong", "Ngay", "Gio", "Trang Thai"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    layout->addLayout(btnLayout);
    layout->addWidget(table);
    
    connect(btnAdd, &QPushButton::clicked, this, &ShowtimeManagementWidget::addShowtime);
    connect(btnDelete, &QPushButton::clicked, this, &ShowtimeManagementWidget::deleteShowtime);
    
    loadData();
}

void ShowtimeManagementWidget::loadData() {
    table->setRowCount(0);
    auto& showtimes = showtimeManager->getAllShowtimes();
    for(int i = 0; i < showtimes.size(); ++i) {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(showtimes[i].getMaSuatChieu())));
        
        Movie* mv = movieManager->findById(showtimes[i].getMaPhim());
        table->setItem(i, 1, new QTableWidgetItem(mv ? QString::fromStdString(mv->getTenPhim()) : "Unknown"));
        
        QTableWidgetItem* roomItem = new QTableWidgetItem(QString::fromStdString(showtimes[i].getMaPhong()));
        roomItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 2, roomItem);
        
        QTableWidgetItem* dateItem = new QTableWidgetItem(QString::fromStdString(showtimes[i].getNgayChieu()));
        dateItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 3, dateItem);
        
        QTableWidgetItem* timeItem = new QTableWidgetItem(QString::fromStdString(showtimes[i].getGioChieu()));
        timeItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 4, timeItem);
        
        QString status = showtimes[i].getTrangThai() ? "Sap Chieu" : "Da Huy/Chieu Xong";
        QTableWidgetItem* statusItem = new QTableWidgetItem(status);
        if(!showtimes[i].getTrangThai()) statusItem->setBackground(Qt::gray);
        table->setItem(i, 5, statusItem);
    }
}

void ShowtimeManagementWidget::addShowtime() {
    auto movies = movieManager->getActiveMovies();
    auto rooms = roomManager->getActiveRooms();
    if (movies.size() == 0 || rooms.size() == 0) {
        QMessageBox::warning(this, "Loi", "Khong du du lieu phim/phong.");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Them Suat Chieu");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
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
    
    layout->addWidget(new QLabel("Phim:", &dialog)); layout->addWidget(cbPhim);
    layout->addWidget(new QLabel("Phong:", &dialog)); layout->addWidget(cbPhong);
    layout->addWidget(new QLabel("Ngay chieu:", &dialog)); layout->addWidget(edtNgay);
    layout->addWidget(new QLabel("Gio chieu:", &dialog)); layout->addWidget(edtGio);
    
    QPushButton *btnOk = new QPushButton("Them", &dialog);
    QPushButton *btnCancel = new QPushButton("Huy", &dialog);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnOk); btnLayout->addWidget(btnCancel);
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
            QMessageBox::warning(this, "Loi", "Trung gio chieu!");
            return;
        }
        
        // Manager generates the ID automatically
        Showtime st("", maPhong, maPhim, ngay, gio, true);
        if(showtimeManager->addShowtime(st, rooms[roomIdx].getSoHang(), rooms[roomIdx].getSoCot(), 2)) {
            showtimeManager->save();
            showtimeManager->saveSeatMaps();
            loadData();
        }
    }
}

void ShowtimeManagementWidget::deleteShowtime() {
    int row = table->currentRow();
    if(row >= 0) {
        std::string id = table->item(row, 0)->text().toStdString();
        if(QMessageBox::question(this, "Xac Nhan", "Ban co muon huy suat chieu nay?") == QMessageBox::Yes) {
            if(showtimeManager->cancelShowtime(id)) {
                showtimeManager->save();
                loadData();
            }
        }
    }
}
