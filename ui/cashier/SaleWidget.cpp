#include "SaleWidget.h"
#include <QDate>
#include <QScrollArea>
#include <QFrame>
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
    layout->setContentsMargins(15, 12, 15, 12);
    layout->setSpacing(12);

    QLabel *lblTitle = new QLabel("BƯỚC 1: CHỌN PHIM BÁN VÉ", page);
    lblTitle->setStyleSheet("color: #F5C542; font-size: 16px; font-weight: bold;");
    layout->addWidget(lblTitle);

    movieTable = new QTableWidget(0, 3, page);
    movieTable->setHorizontalHeaderLabels({"Mã Phim", "Tên Phim", "Thời Lượng"});
    movieTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    movieTable->setSelectionMode(QAbstractItemView::SingleSelection);
    movieTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    movieTable->setAlternatingRowColors(true);
    movieTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    movieTable->verticalHeader()->setDefaultSectionSize(40);
    movieTable->verticalHeader()->setVisible(false);
    layout->addWidget(movieTable);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton* nextBtn = new QPushButton("Tiếp Tục ->", page);
    nextBtn->setCursor(Qt::PointingHandCursor);
    nextBtn->setStyleSheet("background-color: #28A745; color: white; padding: 8px 24px; font-weight: bold; border-radius: 6px; min-height: 36px;");
    btnLayout->addStretch();
    btnLayout->addWidget(nextBtn);
    layout->addLayout(btnLayout);
    
    connect(nextBtn, &QPushButton::clicked, [this]() {
        int row = movieTable->currentRow();
        if(row >= 0) {
            selectedMovieId = movieTable->item(row, 0)->text().toStdString();
            loadShowtimes();
            stepWidget->setCurrentIndex(1);
        } else {
            QMessageBox::warning(this, "Thông báo", "Vui lòng chọn 1 bộ phim.");
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
        
        QTableWidgetItem* durItem = new QTableWidgetItem(QString::number(movies[i].getThoiLuong()) + " phút");
        durItem->setTextAlignment(Qt::AlignCenter);
        movieTable->setItem(i, 2, durItem);
    }
}

void SaleWidget::setupStep2(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(15, 12, 15, 12);
    layout->setSpacing(12);

    QLabel *lblTitle = new QLabel("BƯỚC 2: CHỌN SUẤT CHIẾU", page);
    lblTitle->setStyleSheet("color: #F5C542; font-size: 16px; font-weight: bold;");
    layout->addWidget(lblTitle);

    showtimeTable = new QTableWidget(0, 4, page);
    showtimeTable->setHorizontalHeaderLabels({"Mã Suất Chiếu", "Phòng", "Ngày", "Giờ"});
    showtimeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    showtimeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    showtimeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    showtimeTable->setAlternatingRowColors(true);
    showtimeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    showtimeTable->verticalHeader()->setDefaultSectionSize(40);
    showtimeTable->verticalHeader()->setVisible(false);
    layout->addWidget(showtimeTable);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(15);
    QPushButton* backBtn = new QPushButton("<- Quay Lại", page);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet("background-color: #9E221E; color: white; padding: 8px 20px; font-weight: bold; border-radius: 6px; min-height: 36px;");

    QPushButton* nextBtn = new QPushButton("Tiếp Tục ->", page);
    nextBtn->setCursor(Qt::PointingHandCursor);
    nextBtn->setStyleSheet("background-color: #28A745; color: white; padding: 8px 24px; font-weight: bold; border-radius: 6px; min-height: 36px;");

    btnLayout->addWidget(backBtn);
    btnLayout->addStretch();
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
            QMessageBox::warning(this, "Thông báo", "Vui lòng chọn 1 suất chiếu.");
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

void SaleWidget::updateSelectedSeatsLabel() {
    if (lblSelectedSeats) {
        lblSelectedSeats->setText(QString("Ghế đã chọn: %1 ghế").arg(selectedSeats.size()));
    }
}

void SaleWidget::setupStep3(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(15, 12, 15, 12);
    layout->setSpacing(10);

    // 1. Legend
    QHBoxLayout *legendLayout = new QHBoxLayout();
    legendLayout->setAlignment(Qt::AlignCenter);
    legendLayout->setSpacing(14);

    auto createLegendItem = [page](const QString& text, const QString& style) {
        QWidget* item = new QWidget(page);
        QHBoxLayout* l = new QHBoxLayout(item);
        l->setContentsMargins(2, 2, 2, 2);
        l->setSpacing(6);
        QLabel* badge = new QLabel(item);
        badge->setFixedSize(18, 18);
        badge->setStyleSheet(style);
        QLabel* lbl = new QLabel(text, item);
        lbl->setStyleSheet("color: #FFFFFF; font-size: 12px; font-weight: bold;");
        l->addWidget(badge);
        l->addWidget(lbl);
        return item;
    };

    legendLayout->addWidget(createLegendItem("Thường trống", "background-color: #2E8B57; border-radius: 4px; border: 1px solid #1E5E3A;"));
    legendLayout->addWidget(createLegendItem("Thường đã đặt", "background-color: #3D3D5C; border-radius: 4px; border: 1px solid #28283D;"));
    legendLayout->addWidget(createLegendItem("VIP trống", "background-color: #E6B042; border-radius: 4px; border: 1px solid #B88626;"));
    legendLayout->addWidget(createLegendItem("VIP đã đặt", "background-color: #7A1916; border-radius: 4px; border: 1px solid #52100E;"));
    legendLayout->addWidget(createLegendItem("Đang chọn", "background-color: #2980B9; border-radius: 4px; border: 1px solid #00D2D3;"));
    layout->addLayout(legendLayout);

    // 2. Simulated Screen Bar
    QLabel *lblScreen = new QLabel("--- MÀN HÌNH CHIẾU PHIM ---", page);
    lblScreen->setAlignment(Qt::AlignCenter);
    lblScreen->setFixedHeight(36);
    lblScreen->setStyleSheet(R"(
        QLabel {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B22824, stop:1 #7D1916);
            color: #FFFFFF;
            border: 1px solid #E6B042;
            border-radius: 6px;
            font-weight: bold;
            letter-spacing: 4px;
            font-size: 13px;
        }
    )");
    layout->addWidget(lblScreen);

    // 3. Scroll Area for Seat Grid
    QScrollArea *scrollArea = new QScrollArea(page);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { background: transparent; border: none; }");

    seatGridWidget = new QWidget(scrollArea);
    seatGridWidget->setStyleSheet("background: transparent;");
    seatGridLayout = new QGridLayout(seatGridWidget);
    seatGridLayout->setSpacing(5);
    seatGridLayout->setAlignment(Qt::AlignCenter);

    scrollArea->setWidget(seatGridWidget);
    layout->addWidget(scrollArea, 1);

    // 4. Bottom Info and Actions
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(15);

    lblSelectedSeats = new QLabel("Ghế đã chọn: 0 ghế", page);
    lblSelectedSeats->setStyleSheet("font-size: 15px; font-weight: bold; color: #F5C542;");

    QString btnBackStyle = R"(
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

    QString btnNextStyle = R"(
        QPushButton {
            background-color: #28A745;
            color: #FFFFFF;
            font-size: 14px;
            font-weight: bold;
            border-radius: 6px;
            border: none;
            min-height: 38px;
            padding: 6px 20px;
        }
        QPushButton:hover {
            background-color: #34C759;
            border: 1px solid #FFFFFF;
        }
    )";

    QPushButton* backBtn = new QPushButton("<- Quay Lại", page);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet(btnBackStyle);

    QPushButton* nextBtn = new QPushButton("Tiếp Tục ->", page);
    nextBtn->setCursor(Qt::PointingHandCursor);
    nextBtn->setStyleSheet(btnNextStyle);

    bottomLayout->addWidget(lblSelectedSeats);
    bottomLayout->addStretch();
    bottomLayout->addWidget(backBtn);
    bottomLayout->addWidget(nextBtn);
    layout->addLayout(bottomLayout);

    connect(backBtn, &QPushButton::clicked, [this]() { stepWidget->setCurrentIndex(1); });
    connect(nextBtn, &QPushButton::clicked, [this]() {
        if(selectedSeats.size() > 0) {
            loadCombos();
            stepWidget->setCurrentIndex(3);
        } else {
            QMessageBox::warning(this, "Thông báo", "Vui lòng chọn ít nhất 1 ghế.");
        }
    });
}

void SaleWidget::loadSeats() {
    QLayoutItem* item;
    while ((item = seatGridLayout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }
    selectedSeats = DynamicArray<std::pair<int, int>>();
    updateSelectedSeatsLabel();

    SeatMap* sm = showtimeManager->findSeatMap(selectedShowtimeId);
    if(!sm) return;

    int rows = sm->getSoHang();
    int cols = sm->getSoCot();

    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            QString seatName = QString(QChar('A' + i)) + QString::number(j + 1);
            QPushButton* btn = new QPushButton(seatName, seatGridWidget);
            btn->setFixedSize(38, 36);

            int status = sm->getSeatStatus(i, j);

            if(status == 0) { // Thuong trong
                btn->setStyleSheet(R"(
                    QPushButton {
                        background-color: #2E8B57;
                        color: #FFFFFF;
                        border: 1px solid #1E5E3A;
                        border-radius: 4px;
                        font-weight: bold;
                        font-size: 10px;
                        padding: 0;
                    }
                    QPushButton:hover {
                        border: 1px solid #FFFFFF;
                    }
                )");
                btn->setCursor(Qt::PointingHandCursor);
            } else if(status == 1) { // Thuong da dat
                btn->setStyleSheet(R"(
                    QPushButton {
                        background-color: #3D3D5C;
                        color: #888899;
                        border: 1px solid #28283D;
                        border-radius: 4px;
                        font-weight: bold;
                        font-size: 10px;
                        padding: 0;
                    }
                )");
                btn->setEnabled(false);
                btn->setCursor(Qt::ForbiddenCursor);
            } else if(status == 2) { // VIP trong
                btn->setStyleSheet(R"(
                    QPushButton {
                        background-color: #E6B042;
                        color: #1A1B41;
                        border: 1px solid #B88626;
                        border-radius: 4px;
                        font-weight: bold;
                        font-size: 10px;
                        padding: 0;
                    }
                    QPushButton:hover {
                        border: 1px solid #FFFFFF;
                    }
                )");
                btn->setCursor(Qt::PointingHandCursor);
            } else if(status == 3) { // VIP da dat
                btn->setStyleSheet(R"(
                    QPushButton {
                        background-color: #7A1916;
                        color: #B37D7C;
                        border: 1px solid #52100E;
                        border-radius: 4px;
                        font-weight: bold;
                        font-size: 10px;
                        padding: 0;
                    }
                )");
                btn->setEnabled(false);
                btn->setCursor(Qt::ForbiddenCursor);
            }

            if(status == 0 || status == 2) {
                btn->setCheckable(true);
                connect(btn, &QPushButton::toggled, [this, i, j, btn, status](bool checked) {
                    if(checked) {
                        selectedSeats.push_back({i, j});
                        btn->setStyleSheet(R"(
                            QPushButton {
                                background-color: #2980B9;
                                color: #FFFFFF;
                                border: 2px solid #00D2D3;
                                border-radius: 4px;
                                font-weight: bold;
                                font-size: 10px;
                                padding: 0;
                            }
                        )");
                    } else {
                        for(int k = 0; k < selectedSeats.size(); ++k) {
                            if(selectedSeats[k].first == i && selectedSeats[k].second == j) {
                                selectedSeats.removeAt(k);
                                break;
                            }
                        }
                        if(status == 2) {
                            btn->setStyleSheet(R"(
                                QPushButton {
                                    background-color: #E6B042;
                                    color: #1A1B41;
                                    border: 1px solid #B88626;
                                    border-radius: 4px;
                                    font-weight: bold;
                                    font-size: 10px;
                                    padding: 0;
                                }
                                QPushButton:hover {
                                    border: 1px solid #FFFFFF;
                                }
                            )");
                        } else {
                            btn->setStyleSheet(R"(
                                QPushButton {
                                    background-color: #2E8B57;
                                    color: #FFFFFF;
                                    border: 1px solid #1E5E3A;
                                    border-radius: 4px;
                                    font-weight: bold;
                                    font-size: 10px;
                                    padding: 0;
                                }
                                QPushButton:hover {
                                    border: 1px solid #FFFFFF;
                                }
                            )");
                        }
                    }
                    updateSelectedSeatsLabel();
                });
            }

            seatGridLayout->addWidget(btn, i, j);
        }
    }
}

void SaleWidget::setupStep4(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(15, 12, 15, 12);
    layout->setSpacing(12);

    QLabel *lblTitle = new QLabel("CHỌN COMBO BẮP NƯỚC (NẾU CÓ)", page);
    lblTitle->setStyleSheet("color: #F5C542; font-size: 16px; font-weight: bold;");
    layout->addWidget(lblTitle);

    comboTable = new QTableWidget(0, 4, page);
    comboTable->setHorizontalHeaderLabels({"Mã Combo", "Tên Combo", "Đơn Giá", "Số Lượng"});
    comboTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    comboTable->setSelectionMode(QAbstractItemView::SingleSelection);
    comboTable->setAlternatingRowColors(true);
    comboTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    comboTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    comboTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    comboTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    comboTable->setColumnWidth(3, 120);
    comboTable->verticalHeader()->setDefaultSectionSize(48);
    comboTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    comboTable->verticalHeader()->setVisible(false);
    layout->addWidget(comboTable);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(15);
    QPushButton* backBtn = new QPushButton("<- Quay Lại", page);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet("background-color: #9E221E; color: white; padding: 8px 20px; font-weight: bold; border-radius: 6px; min-height: 36px;");

    QPushButton* skipBtn = new QPushButton("Bỏ Qua Combo", page);
    skipBtn->setCursor(Qt::PointingHandCursor);
    skipBtn->setStyleSheet("background-color: #E6B042; color: #1A1B41; padding: 8px 16px; font-weight: bold; border-radius: 6px; min-height: 36px;");

    QPushButton* nextBtn = new QPushButton("Tiếp Tục ->", page);
    nextBtn->setCursor(Qt::PointingHandCursor);
    nextBtn->setStyleSheet("background-color: #28A745; color: white; padding: 8px 20px; font-weight: bold; border-radius: 6px; min-height: 36px;");

    btnLayout->addWidget(backBtn);
    btnLayout->addWidget(skipBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(nextBtn);
    layout->addLayout(btnLayout);
    
    connect(backBtn, &QPushButton::clicked, [this]() { stepWidget->setCurrentIndex(2); });
    connect(skipBtn, &QPushButton::clicked, [this]() {
        selectedCombos = DynamicArray<std::pair<std::string, int>>();
        generatePreview();
        stepWidget->setCurrentIndex(4);
    });
    connect(nextBtn, &QPushButton::clicked, [this]() {
        selectedCombos = DynamicArray<std::pair<std::string, int>>();
        for(int i = 0; i < comboTable->rowCount(); ++i) {
            QWidget* pWidget = comboTable->cellWidget(i, 3);
            QSpinBox* spin = pWidget ? pWidget->findChild<QSpinBox*>() : nullptr;
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
        
        QTableWidgetItem* priceItem = new QTableWidgetItem(QString("%L1 VND").arg(combos[i].getGia(), 0, 'f', 0));
        priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        priceItem->setForeground(QBrush(QColor("#F5C542")));
        comboTable->setItem(i, 2, priceItem);
        
        QWidget* pWidget = new QWidget();
        QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
        pLayout->setContentsMargins(4, 2, 4, 2);
        pLayout->setAlignment(Qt::AlignCenter);

        QSpinBox* spinBox = new QSpinBox(pWidget);
        spinBox->setRange(0, 20);
        spinBox->setValue(0);
        spinBox->setAlignment(Qt::AlignCenter);
        spinBox->setFixedSize(85, 30);
        spinBox->setStyleSheet(R"(
            QSpinBox {
                background-color: #E6B042;
                color: #1A1B41;
                font-size: 13px;
                font-weight: bold;
                border: 1px solid #C49129;
                border-radius: 4px;
                padding: 0px 4px;
                min-height: 26px;
                max-height: 28px;
            }
            QSpinBox::up-button, QSpinBox::down-button {
                width: 18px;
            }
        )");
        pLayout->addWidget(spinBox);
        comboTable->setCellWidget(i, 3, pWidget);
    }
}

void SaleWidget::setupStep5(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(20, 15, 20, 15);
    layout->setSpacing(14);
    layout->setAlignment(Qt::AlignCenter);

    QFrame *card = new QFrame(page);
    card->setMaximumWidth(600);
    card->setStyleSheet(R"(
        QFrame {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2C2E6A, stop:1 #1E1F4B);
            border: 2px solid #E6B042;
            border-radius: 10px;
            padding: 20px;
        }
    )");
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(14);

    QLabel* title = new QLabel("XÁC NHẬN BÁN VÉ TẠI QUẦY", card);
    title->setStyleSheet("color: #F5C542; font-size: 18px; font-weight: bold; border: none; background: transparent;");
    title->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(title);

    previewLabel = new QLabel(card);
    previewLabel->setStyleSheet("color: #FFFFFF; font-size: 13px; line-height: 1.5; border: none; background: transparent;");
    previewLabel->setWordWrap(true);
    cardLayout->addWidget(previewLabel);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(15);

    QPushButton* backBtn = new QPushButton("Hủy Bỏ", card);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet("background-color: #9E221E; color: white; padding: 8px 24px; font-weight: bold; border-radius: 6px; min-height: 38px; border: none;");

    QPushButton* nextBtn = new QPushButton("Xác Nhận & Thanh Toán", card);
    nextBtn->setCursor(Qt::PointingHandCursor);
    nextBtn->setStyleSheet("background-color: #28A745; color: white; padding: 8px 24px; font-weight: bold; border-radius: 6px; min-height: 38px; border: none;");

    btnLayout->addWidget(backBtn);
    btnLayout->addWidget(nextBtn);
    cardLayout->addLayout(btnLayout);

    layout->addWidget(card);
    
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
    
    QString preview = QString("• <b>Phim:</b> %1<br>"
                              "• <b>Suất chiếu:</b> %2 %3 | <b>Phòng:</b> %4<br><br>")
        .arg(QString::fromStdString(mv->getTenPhim()).toUpper())
        .arg(QString::fromStdString(st->getNgayChieu()))
        .arg(QString::fromStdString(st->getGioChieu()))
        .arg(QString::fromStdString(st->getMaPhong()));
    
    double total = 0;
    preview += "<b>Danh sách ghế:</b><br>";
    for(int i = 0; i < selectedSeats.size(); ++i) {
        int r = selectedSeats[i].first;
        int c = selectedSeats[i].second;
        int type = sm ? sm->getSeatStatus(r, c) : 0;
        double price = PriceCalculator::calculateTicketPrice(type, st->getNgayChieu(), st->getGioChieu());
        total += price;
        bool isVip = (type == 2 || type == 3);
        preview += QString(" - Ghế <b>%1%2</b> (%3): %L4 VND<br>")
            .arg((char)('A' + r))
            .arg(c + 1)
            .arg(isVip ? "<span style='color: #F5C542;'>VIP</span>" : "Thường")
            .arg(price, 0, 'f', 0);
    }
    
    if(selectedCombos.size() > 0) {
        preview += "<br><b>Combo bắp nước:</b><br>";
        for(int i = 0; i < selectedCombos.size(); ++i) {
            Combo* cb = comboManager->findById(selectedCombos[i].first);
            if(cb) {
                double cbTotal = cb->getGia() * selectedCombos[i].second;
                total += cbTotal;
                preview += QString(" - %1 x%2 = %L3 VND<br>")
                    .arg(QString::fromStdString(cb->getTenCombo()))
                    .arg(selectedCombos[i].second)
                    .arg(cbTotal, 0, 'f', 0);
            }
        }
    }
    
    preview += QString("<br><div style='font-size: 16px; color: #F5C542;'><b>TỔNG CỘNG: %L1 VND</b></div>").arg(total, 0, 'f', 0);
    previewLabel->setText(preview);
}

void SaleWidget::setupStep6(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setAlignment(Qt::AlignCenter);

    QFrame *card = new QFrame(page);
    card->setMaximumWidth(450);
    card->setStyleSheet(R"(
        QFrame {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2C2E6A, stop:1 #1E1F4B);
            border: 2px solid #28A745;
            border-radius: 10px;
            padding: 25px;
        }
    )");
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(15);
    cardLayout->setAlignment(Qt::AlignCenter);

    QLabel *icon = new QLabel("✅", card);
    icon->setStyleSheet("font-size: 36px; border: none; background: transparent;");
    icon->setAlignment(Qt::AlignCenter);

    confirmLabel = new QLabel("THANH TOÁN THÀNH CÔNG!\nVé đã được in và kích hoạt hợp lệ.", card);
    confirmLabel->setStyleSheet("color: #FFFFFF; font-size: 15px; font-weight: bold; border: none; background: transparent;");
    confirmLabel->setAlignment(Qt::AlignCenter);

    QPushButton* inVeBtn = new QPushButton("Hoàn Tất / Bán Đơn Tiếp", card);
    inVeBtn->setCursor(Qt::PointingHandCursor);
    inVeBtn->setStyleSheet("background-color: #28A745; color: white; padding: 10px 24px; font-weight: bold; border-radius: 6px; font-size: 14px;");

    cardLayout->addWidget(icon);
    cardLayout->addWidget(confirmLabel);
    cardLayout->addWidget(inVeBtn);
    layout->addWidget(card);
    
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
        // Counter sales are paid and valid immediately
        ticketManager->createTicket(selectedShowtimeId, "", maNhanVien, r, c, finalPrice, comboStr, ngayMua, "", "VALID");
        sm->bookSeat(r, c);
    }
    showtimeManager->saveSeatMaps();
    ticketManager->save();
}
