#include "SaleWidget.h"
#include <QDate>
#include "managers/MovieManager.h"
#include "managers/RoomManager.h"
#include "managers/ShowtimeManager.h"
#include "managers/TicketManager.h"
#include "managers/ComboManager.h"
#include "utils/PriceCalculator.h"
#include "models/Movie.h"
#include "models/Showtime.h"
#include "models/Combo.h"
#include <QHeaderView>
#include <QMessageBox>

SaleWidget::SaleWidget(const std::string& maNV, MovieManager* movieMgr, RoomManager* roomMgr,
                       ShowtimeManager* showtimeMgr, TicketManager* ticketMgr,
                       ComboManager* comboMgr, QWidget* parent)
    : QWidget(parent), maNhanVien(maNV), movieManager(movieMgr), roomManager(roomMgr),
      showtimeManager(showtimeMgr), ticketManager(ticketMgr), comboManager(comboMgr) {
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    stepWidget = new QStackedWidget(this);
    
    QWidget* step1 = new QWidget(); setupStep1(step1);
    QWidget* step2 = new QWidget(); setupStep2(step2);
    QWidget* step3 = new QWidget(); setupStep3(step3);
    QWidget* step4 = new QWidget(); setupStep4(step4);
    QWidget* step5 = new QWidget(); setupStep5(step5);
    QWidget* step6 = new QWidget(); setupStep6(step6);
    
    stepWidget->addWidget(step1);
    stepWidget->addWidget(step2);
    stepWidget->addWidget(step3);
    stepWidget->addWidget(step4);
    stepWidget->addWidget(step5);
    stepWidget->addWidget(step6);
    
    mainLayout->addWidget(stepWidget);
    
    loadMovies();
}

void SaleWidget::setupStep1(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->addWidget(new QLabel("Chon Phim:"));
    movieTable = new QTableWidget(0, 3, page);
    movieTable->setHorizontalHeaderLabels({"Ma Phim", "Ten Phim", "Thoi Luong"});
    movieTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    movieTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    movieTable->setAlternatingRowColors(true);
    movieTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(movieTable);
    
    QPushButton* nextBtn = new QPushButton("Tiep Tuc ->", page);
    nextBtn->setStyleSheet("background-color: #27ae60; color: white; padding: 10px 20px; font-weight: bold; border-radius: 6px;");
    layout->addWidget(nextBtn);
    
    connect(nextBtn, &QPushButton::clicked, [this]() {
        int row = movieTable->currentRow();
        if(row >= 0) {
            selectedMovieId = movieTable->item(row, 0)->text().toStdString();
            loadShowtimes();
            stepWidget->setCurrentIndex(1);
        } else {
            QMessageBox::warning(this, "Loi", "Vui long chon 1 bo phim.");
        }
    });
}

void SaleWidget::loadMovies() {
    movieTable->setRowCount(0);
    auto movies = movieManager->getActiveMovies();
    for(int i = 0; i < movies.size(); ++i) {
        movieTable->insertRow(i);
        movieTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(movies[i].getMaPhim())));
        movieTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(movies[i].getTenPhim())));
        
        QTableWidgetItem* durItem = new QTableWidgetItem(QString::number(movies[i].getThoiLuong()) + " phut");
        durItem->setTextAlignment(Qt::AlignCenter);
        movieTable->setItem(i, 2, durItem);
    }
}

void SaleWidget::setupStep2(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->addWidget(new QLabel("Chon Suat Chieu:"));
    showtimeTable = new QTableWidget(0, 4, page);
    showtimeTable->setHorizontalHeaderLabels({"Ma SC", "Phong", "Ngay", "Gio"});
    showtimeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    showtimeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    showtimeTable->setAlternatingRowColors(true);
    showtimeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(showtimeTable);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* backBtn = new QPushButton("<- Quay Lai", page);
    QPushButton* nextBtn = new QPushButton("Tiep Tuc ->", page);
    nextBtn->setStyleSheet("background-color: #27ae60; color: white; padding: 10px 20px; font-weight: bold; border-radius: 6px;");
    btnLayout->addWidget(backBtn);
    btnLayout->addWidget(nextBtn);
    layout->addLayout(btnLayout);
    
    connect(backBtn, &QPushButton::clicked, [this]() { stepWidget->setCurrentIndex(0); });
    connect(nextBtn, &QPushButton::clicked, [this]() {
        int row = showtimeTable->currentRow();
        if(row >= 0) {
            selectedShowtimeId = showtimeTable->item(row, 0)->text().toStdString();
            loadSeats();
            stepWidget->setCurrentIndex(2);
        } else {
            QMessageBox::warning(this, "Loi", "Vui long chon 1 suat chieu.");
        }
    });
}

void SaleWidget::loadShowtimes() {
    showtimeTable->setRowCount(0);
    auto showtimes = showtimeManager->getShowtimesByMovie(selectedMovieId);
    int rowCount = 0;
    for(int i = 0; i < showtimes.size(); ++i) {
        if(showtimes[i].getTrangThai()) {
            showtimeTable->insertRow(rowCount);
            showtimeTable->setItem(rowCount, 0, new QTableWidgetItem(QString::fromStdString(showtimes[i].getMaSuatChieu())));
            
            QTableWidgetItem* roomItem = new QTableWidgetItem(QString::fromStdString(showtimes[i].getMaPhong()));
            roomItem->setTextAlignment(Qt::AlignCenter);
            showtimeTable->setItem(rowCount, 1, roomItem);
            
            QTableWidgetItem* dateItem = new QTableWidgetItem(QString::fromStdString(showtimes[i].getNgayChieu()));
            dateItem->setTextAlignment(Qt::AlignCenter);
            showtimeTable->setItem(rowCount, 2, dateItem);
            
            QTableWidgetItem* timeItem = new QTableWidgetItem(QString::fromStdString(showtimes[i].getGioChieu()));
            timeItem->setTextAlignment(Qt::AlignCenter);
            showtimeTable->setItem(rowCount, 3, timeItem);
            rowCount++;
        }
    }
}

void SaleWidget::setupStep3(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->addWidget(new QLabel("Chon Ghe:"));
    seatGridWidget = new QWidget(page);
    layout->addWidget(seatGridWidget);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* backBtn = new QPushButton("Quay Lai", page);
    QPushButton* nextBtn = new QPushButton("Tiep Tuc", page);
    btnLayout->addWidget(backBtn);
    btnLayout->addWidget(nextBtn);
    layout->addLayout(btnLayout);
    
    connect(backBtn, &QPushButton::clicked, [this]() { stepWidget->setCurrentIndex(1); });
    connect(nextBtn, &QPushButton::clicked, [this]() {
        if(selectedSeats.size() > 0) {
            loadCombos();
            stepWidget->setCurrentIndex(3);
        } else {
            QMessageBox::warning(this, "Loi", "Vui long chon it nhat 1 ghe.");
        }
    });
}

void SaleWidget::loadSeats() {
    qDeleteAll(seatGridWidget->children());
    selectedSeats = DynamicArray<std::pair<int, int>>();
    
    QGridLayout* grid = new QGridLayout(seatGridWidget);
    SeatMap* sm = showtimeManager->findSeatMap(selectedShowtimeId);
    if(!sm) return;
    
    for(int i = 0; i < sm->getSoHang(); ++i) {
        for(int j = 0; j < sm->getSoCot(); ++j) {
            QPushButton* btn = new QPushButton(QString("%1%2").arg((char)('A' + i)).arg(j + 1), seatGridWidget);
            int status = sm->getSeatStatus(i, j);
            btn->setFixedSize(45, 45);
            if(status == 1 || status == 3) {
                btn->setEnabled(false);
                btn->setStyleSheet("background-color: #444444; color: #777777; border: 1px solid #333333; border-radius: 6px; font-size: 11px;");
            } else if(status == 2) {
                btn->setStyleSheet("background-color: #f39c12; color: #1a1a2e; border: 2px solid #ffd700; border-radius: 6px; font-weight: bold; font-size: 11px;");
            } else {
                btn->setStyleSheet("background-color: #27ae60; color: white; border: 1px solid #1e8449; border-radius: 6px; font-weight: bold; font-size: 11px;");
            }
            
            btn->setCheckable(true);
            connect(btn, &QPushButton::toggled, [this, i, j, btn, status](bool checked) {
                if(checked) {
                    selectedSeats.push_back({i, j});
                    btn->setStyleSheet("background-color: #2980b9; color: white; border: 2px solid #00d2d3; border-radius: 6px; font-weight: bold; font-size: 11px;");
                } else {
                    for(int k = 0; k < selectedSeats.size(); ++k) {
                        if(selectedSeats[k].first == i && selectedSeats[k].second == j) {
                            selectedSeats.removeAt(k);
                            break;
                        }
                    }
                    if(status == 2) btn->setStyleSheet("background-color: #f39c12; color: #1a1a2e; border: 2px solid #ffd700; border-radius: 6px; font-weight: bold; font-size: 11px;");
                    else btn->setStyleSheet("background-color: #27ae60; color: white; border: 1px solid #1e8449; border-radius: 6px; font-weight: bold; font-size: 11px;");
                }
            });
            grid->addWidget(btn, i, j);
        }
    }
}

void SaleWidget::setupStep4(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->addWidget(new QLabel("Chon Combo:"));
    comboTable = new QTableWidget(0, 4, page);
    comboTable->setHorizontalHeaderLabels({"Ma Combo", "Ten Combo", "Gia", "So Luong"});
    comboTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    comboTable->setAlternatingRowColors(true);
    comboTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(comboTable);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* backBtn = new QPushButton("<- Quay Lai", page);
    QPushButton* nextBtn = new QPushButton("Tiep Tuc ->", page);
    nextBtn->setStyleSheet("background-color: #27ae60; color: white; padding: 10px 20px; font-weight: bold; border-radius: 6px;");
    btnLayout->addWidget(backBtn);
    btnLayout->addWidget(nextBtn);
    layout->addLayout(btnLayout);
    
    connect(backBtn, &QPushButton::clicked, [this]() { stepWidget->setCurrentIndex(2); });
    connect(nextBtn, &QPushButton::clicked, [this]() {
        selectedCombos = DynamicArray<std::pair<std::string, int>>();
        for(int i = 0; i < comboTable->rowCount(); ++i) {
            QSpinBox* spin = qobject_cast<QSpinBox*>(comboTable->cellWidget(i, 3));
            if(spin && spin->value() > 0) {
                selectedCombos.push_back({comboTable->item(i, 0)->text().toStdString(), spin->value()});
            }
        }
        generatePreview();
        stepWidget->setCurrentIndex(4);
    });
}

void SaleWidget::loadCombos() {
    comboTable->setRowCount(0);
    auto combos = comboManager->getActiveCombos();
    for(int i = 0; i < combos.size(); ++i) {
        comboTable->insertRow(i);
        comboTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(combos[i].getMaCombo())));
        comboTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(combos[i].getTenCombo())));
        
        QTableWidgetItem* priceItem = new QTableWidgetItem(QString::number(combos[i].getGia(), 'f', 0) + " VND");
        priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        comboTable->setItem(i, 2, priceItem);
        
        QSpinBox* spinBox = new QSpinBox();
        spinBox->setMinimum(0);
        spinBox->setMaximum(10);
        comboTable->setCellWidget(i, 3, spinBox);
    }
}

void SaleWidget::setupStep5(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->addWidget(new QLabel("Xac Nhan Ve (Ve Gia):"));
    previewLabel = new QLabel(page);
    layout->addWidget(previewLabel);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* backBtn = new QPushButton("Huy", page);
    QPushButton* nextBtn = new QPushButton("Xac Nhan Thanh Toan", page);
    btnLayout->addWidget(backBtn);
    btnLayout->addWidget(nextBtn);
    layout->addLayout(btnLayout);
    
    connect(backBtn, &QPushButton::clicked, [this]() { stepWidget->setCurrentIndex(3); });
    connect(nextBtn, &QPushButton::clicked, [this]() {
        createTickets();
        stepWidget->setCurrentIndex(5);
    });
}

void SaleWidget::generatePreview() {
    Showtime* st = showtimeManager->findById(selectedShowtimeId);
    Movie* mv = movieManager->findById(selectedMovieId);
    if(!st || !mv) return;
    
    SeatMap* sm = showtimeManager->findSeatMap(selectedShowtimeId);
    
    QString preview = QString("Phim: %1\nSuat chieu: %2 %3\nPhong: %4\n\n").arg(
        QString::fromStdString(mv->getTenPhim()),
        QString::fromStdString(st->getNgayChieu()),
        QString::fromStdString(st->getGioChieu()),
        QString::fromStdString(st->getMaPhong())
    );
    
    double total = 0;
    preview += "Ghe:\n";
    for(int i = 0; i < selectedSeats.size(); ++i) {
        int r = selectedSeats[i].first;
        int c = selectedSeats[i].second;
        int type = sm->getSeatStatus(r, c);
        double price = PriceCalculator::calculateTicketPrice(type, st->getNgayChieu(), st->getGioChieu());
        total += price;
        preview += QString("- Ghe %1%2: %3 VND\n").arg((char)('A' + r)).arg(c + 1).arg(price);
    }
    
    if(selectedCombos.size() > 0) {
        preview += "\nCombo:\n";
        for(int i = 0; i < selectedCombos.size(); ++i) {
            Combo* cb = comboManager->findById(selectedCombos[i].first);
            if(cb) {
                double cbTotal = cb->getGia() * selectedCombos[i].second;
                total += cbTotal;
                preview += QString("- %1 x%2: %3 VND\n").arg(QString::fromStdString(cb->getTenCombo())).arg(selectedCombos[i].second).arg(cbTotal);
            }
        }
    }
    
    preview += QString("\nTong cong: %1 VND").arg(total);
    previewLabel->setText(preview);
}

void SaleWidget::setupStep6(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    confirmLabel = new QLabel("Thanh toan thanh cong!", page);
    layout->addWidget(confirmLabel);
    
    QPushButton* inVeBtn = new QPushButton("Hoan Tat & In Ve", page);
    layout->addWidget(inVeBtn);
    
    connect(inVeBtn, &QPushButton::clicked, [this]() {
        loadMovies();
        stepWidget->setCurrentIndex(0);
    });
}

void SaleWidget::createTickets() {
    Showtime* st = showtimeManager->findById(selectedShowtimeId);
    SeatMap* sm = showtimeManager->findSeatMap(selectedShowtimeId);
    if(!st || !sm) return;
    
    // Build combo string and calculate total combo price
    std::string comboStr = "";
    double totalComboPrice = 0.0;
    for(int i = 0; i < selectedCombos.size(); ++i) {
        Combo* cb = comboManager->findById(selectedCombos[i].first);
        if(cb) {
            if (!comboStr.empty()) comboStr += "; ";
            comboStr += cb->getTenCombo() + " x" + std::to_string(selectedCombos[i].second);
            totalComboPrice += cb->getGia() * selectedCombos[i].second;
        }
    }
    // Distribute combo cost evenly across seats
    double comboPricePerTicket = (selectedSeats.size() > 0) ? (totalComboPrice / selectedSeats.size()) : 0;
    
    for(int i = 0; i < selectedSeats.size(); ++i) {
        int r = selectedSeats[i].first;
        int c = selectedSeats[i].second;
        if (!sm->isSeatAvailable(r, c)) {
            QMessageBox::warning(this, "Loi", QString("Ghe %1%2 da co nguoi dat!").arg((char)('A' + r)).arg(c + 1));
            return;
        }
    }

    for(int i = 0; i < selectedSeats.size(); ++i) {
        int r = selectedSeats[i].first;
        int c = selectedSeats[i].second;
        double seatPrice = PriceCalculator::calculateTicketPrice(sm->getSeatStatus(r, c), st->getNgayChieu(), st->getGioChieu());
        double finalPrice = seatPrice + comboPricePerTicket;
        
        std::string ngayMua = QDate::currentDate().toString("dd/MM/yyyy").toStdString();
        ticketManager->createTicket(selectedShowtimeId, "", maNhanVien, r, c, finalPrice, comboStr, ngayMua);
        sm->bookSeat(r, c);
    }
    showtimeManager->saveSeatMaps();
}
