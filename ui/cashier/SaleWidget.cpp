#include "SaleWidget.h"
#include "../common/Theme.h"
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
      showtimeManager(showtimeMgr), ticketManager(ticketMgr), comboManager(comboMgr),
      lblSelectedSeats(nullptr)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(26, 20, 26, 18);
    mainLayout->setSpacing(14);

    // Top Soft Pill Stepper Bar
    QFrame* stepperFrame = new QFrame(this);
    stepperFrame->setStyleSheet("QFrame { background-color: #22263C; border: 1px solid #323859; border-radius: 16px; }");
    QHBoxLayout* stepperLayout = new QHBoxLayout(stepperFrame);
    stepperLayout->setContentsMargins(14, 10, 14, 10);
    stepperLayout->setSpacing(8);

    QStringList stepNames = {
        "1. Chọn Phim",
        "2. Suất Chiếu",
        "3. Chọn Ghế",
        "4. Combo",
        "5. Xác Nhận",
        "6. Hoàn Tất"
    };
    for (int i = 0; i < stepNames.size(); ++i) {
        QLabel* lbl = new QLabel(stepNames[i], stepperFrame);
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setMinimumHeight(32);
        stepIndicators.append(lbl);
        stepperLayout->addWidget(lbl, 1);
    }
    mainLayout->addWidget(stepperFrame);

    stepWidget = new QStackedWidget(this);
    stepWidget->setStyleSheet("QStackedWidget { background: transparent; }");

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

    mainLayout->addWidget(stepWidget, 1);

    connect(stepWidget, &QStackedWidget::currentChanged, this, &SaleWidget::updateStepIndicator);
    updateStepIndicator(0);

    loadMovies();
}

void SaleWidget::updateStepIndicator(int currentStep) {
    for (int i = 0; i < stepIndicators.size(); ++i) {
        if (i == currentStep) {
            stepIndicators[i]->setStyleSheet(
                "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FF6B6B, stop:1 #FF8E72); "
                "color: #FFFFFF; font-size: 12px; font-weight: bold; "
                "border-radius: 10px; padding: 4px 10px; border: none;");
        } else if (i < currentStep) {
            stepIndicators[i]->setStyleSheet(
                "background-color: #1B3635; color: #34D399; font-size: 12px; font-weight: bold; "
                "border-radius: 10px; padding: 4px 10px; border: 1px solid #2A6E5C;");
        } else {
            stepIndicators[i]->setStyleSheet(
                "background-color: #1A1D2F; color: #7C84A8; font-size: 12px; font-weight: 600; "
                "border-radius: 10px; padding: 4px 10px; border: 1px solid #2D3352;");
        }
    }
}

void SaleWidget::setupStep1(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 4, 0, 0);
    layout->setSpacing(12);

    QLabel *lblTitle = new QLabel("CHỌN PHIM BÁN VÉ TẠI QUẦY", page);
    lblTitle->setStyleSheet(Theme::sectionTitleStyle());
    layout->addWidget(lblTitle);

    movieTable = new QTableWidget(0, 4, page);
    movieTable->setHorizontalHeaderLabels({"Mã Phim", "Tên Phim", "Độ Tuổi", "Thời Lượng"});
    movieTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    movieTable->setSelectionMode(QAbstractItemView::SingleSelection);
    movieTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    movieTable->setAlternatingRowColors(true);
    movieTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    movieTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    movieTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    movieTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    movieTable->verticalHeader()->setDefaultSectionSize(46);
    movieTable->verticalHeader()->setVisible(false);
    layout->addWidget(movieTable, 1);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton* nextBtn = new QPushButton("Tiếp Tục Chọn Suất", page);
    nextBtn->setCursor(Qt::PointingHandCursor);
    nextBtn->setStyleSheet(Theme::primaryButtonStyle(42, 14));
    btnLayout->addStretch();
    btnLayout->addWidget(nextBtn);
    layout->addLayout(btnLayout);

    auto proceed = [this]() {
        int row = movieTable->currentRow();
        if (row >= 0) {
            selectedMovieId = movieTable->item(row, 0)->text().toStdString();
            loadShowtimes();
            stepWidget->setCurrentIndex(1);
        } else {
            QMessageBox::warning(this, "Thông Báo", "Vui lòng chọn 1 bộ phim.");
        }
    };

    connect(nextBtn, &QPushButton::clicked, this, proceed);
    connect(movieTable, &QTableWidget::doubleClicked, this, proceed);
}

void SaleWidget::loadMovies() {
    movieTable->setRowCount(0);
    movieTable->setColumnCount(4);
    movieTable->setHorizontalHeaderLabels({"Mã Phim", "Tên Phim", "Độ Tuổi", "Thời Lượng"});
    auto movies = movieManager->getActiveMovies();
    QFont boldFont = movieTable->font();
    boldFont.setBold(true);

    for (int i = 0; i < movies.size(); ++i) {
        movieTable->insertRow(i);

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::fromStdString(movies[i].getMaPhim()));
        idItem->setTextAlignment(Qt::AlignCenter);
        idItem->setForeground(QBrush(QColor("#8C92FF")));
        idItem->setFont(boldFont);
        movieTable->setItem(i, 0, idItem);

        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(movies[i].getTenPhim()));
        nameItem->setFont(boldFont);
        movieTable->setItem(i, 1, nameItem);

        QString ageStr = movies[i].getDoTuoi() == 0 ? "P (Mọi lứa tuổi)" : QString("C%1 (%1+)").arg(movies[i].getDoTuoi());
        QTableWidgetItem* ageItem = new QTableWidgetItem(ageStr);
        ageItem->setTextAlignment(Qt::AlignCenter);
        ageItem->setFont(boldFont);
        if (movies[i].getDoTuoi() >= 18) {
            ageItem->setForeground(QBrush(QColor("#FF858D")));
        } else if (movies[i].getDoTuoi() >= 13) {
            ageItem->setForeground(QBrush(QColor("#FFD166")));
        } else {
            ageItem->setForeground(QBrush(QColor("#34D399")));
        }
        movieTable->setItem(i, 2, ageItem);

        QTableWidgetItem* durItem = new QTableWidgetItem(QString::number(movies[i].getThoiLuong()) + " phút");
        durItem->setTextAlignment(Qt::AlignCenter);
        movieTable->setItem(i, 3, durItem);
    }
}

void SaleWidget::setupStep2(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 4, 0, 0);
    layout->setSpacing(12);

    QLabel *lblTitle = new QLabel("CHỌN SUẤT CHIẾU", page);
    lblTitle->setStyleSheet(Theme::sectionTitleStyle());
    layout->addWidget(lblTitle);

    showtimeTable = new QTableWidget(0, 4, page);
    showtimeTable->setHorizontalHeaderLabels({"Mã Suất Chiếu", "Phòng", "Ngày Chiếu", "Giờ Chiếu"});
    showtimeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    showtimeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    showtimeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    showtimeTable->setAlternatingRowColors(true);
    showtimeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    showtimeTable->verticalHeader()->setDefaultSectionSize(46);
    showtimeTable->verticalHeader()->setVisible(false);
    layout->addWidget(showtimeTable, 1);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);
    QPushButton* backBtn = new QPushButton("Quay Lại", page);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet(Theme::secondaryButtonStyle(42, 14));

    QPushButton* nextBtn = new QPushButton("Tiếp Tục Chọn Ghế", page);
    nextBtn->setCursor(Qt::PointingHandCursor);
    nextBtn->setStyleSheet(Theme::primaryButtonStyle(42, 14));

    btnLayout->addWidget(backBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(nextBtn);
    layout->addLayout(btnLayout);

    auto proceed = [this]() {
        int row = showtimeTable->currentRow();
        if (row >= 0) {
            selectedShowtimeId = showtimeTable->item(row, 0)->text().toStdString();

            Movie* m = movieManager ? movieManager->findById(selectedMovieId) : nullptr;
            if (m && m->getDoTuoi() > 0) {
                QMessageBox msg(this);
                msg.setWindowTitle("Lưu Ý Độ Tuổi");
                msg.setIcon(QMessageBox::Warning);
                msg.setText(QString("<b>PHIM GIỚI HẠN ĐỘ TUỔI: C%1</b> (Dành cho khán giả từ đủ %1 tuổi trở lên)<br><br>"
                                    "Phim: <b>%2</b><br><br>"
                                    "Thu ngân vui lòng <b>kiểm tra giấy tờ tùy thân (CCCD / Thẻ học sinh)</b> của khách trước khi in vé!")
                                .arg(m->getDoTuoi())
                                .arg(QString::fromStdString(m->getTenPhim())));
                msg.addButton("Đã Kiểm Tra && Tiếp Tục", QMessageBox::AcceptRole);
                msg.exec();
            }

            loadSeats();
            stepWidget->setCurrentIndex(2);
        } else {
            QMessageBox::warning(this, "Thông Báo", "Vui lòng chọn 1 suất chiếu.");
        }
    };

    connect(backBtn, &QPushButton::clicked, this, [this]() { stepWidget->setCurrentIndex(0); });
    connect(nextBtn, &QPushButton::clicked, this, proceed);
    connect(showtimeTable, &QTableWidget::doubleClicked, this, proceed);
}

void SaleWidget::loadShowtimes() {
    showtimeTable->setRowCount(0);
    auto showtimes = showtimeManager->getShowtimesByMovie(selectedMovieId);
    QFont boldFont = showtimeTable->font();
    boldFont.setBold(true);

    int rowCount = 0;
    for (int i = 0; i < showtimes.size(); ++i) {
        if (showtimes[i].getTrangThai()) {
            showtimeTable->insertRow(rowCount);

            QTableWidgetItem* idItem = new QTableWidgetItem(QString::fromStdString(showtimes[i].getMaSuatChieu()));
            idItem->setTextAlignment(Qt::AlignCenter);
            idItem->setFont(boldFont);
            idItem->setForeground(QBrush(QColor("#8C92FF")));
            showtimeTable->setItem(rowCount, 0, idItem);

            QTableWidgetItem* roomItem = new QTableWidgetItem(QString::fromStdString(showtimes[i].getMaPhong()));
            roomItem->setTextAlignment(Qt::AlignCenter);
            showtimeTable->setItem(rowCount, 1, roomItem);

            QTableWidgetItem* dateItem = new QTableWidgetItem(QString::fromStdString(showtimes[i].getNgayChieu()));
            dateItem->setTextAlignment(Qt::AlignCenter);
            showtimeTable->setItem(rowCount, 2, dateItem);

            QTableWidgetItem* timeItem = new QTableWidgetItem(QString::fromStdString(showtimes[i].getGioChieu()));
            timeItem->setTextAlignment(Qt::AlignCenter);
            timeItem->setFont(boldFont);
            timeItem->setForeground(QBrush(QColor("#FFD166")));
            showtimeTable->setItem(rowCount, 3, timeItem);
            rowCount++;
        }
    }
}

void SaleWidget::updateSelectedSeatsLabel() {
    if (!lblSelectedSeats) return;
    if (selectedSeats.size() == 0) {
        lblSelectedSeats->setText("Ghế đã chọn: Chưa chọn ghế");
        return;
    }
    QStringList names;
    for (int i = 0; i < selectedSeats.size(); ++i) {
        names << QString(QChar('A' + selectedSeats[i].first)) + QString::number(selectedSeats[i].second + 1);
    }
    lblSelectedSeats->setText(QString("Đã chọn %1 ghế:  %2").arg(selectedSeats.size()).arg(names.join(", ")));
}

void SaleWidget::setupStep3(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 4, 0, 0);
    layout->setSpacing(12);

    QFrame* legendFrame = new QFrame(page);
    legendFrame->setStyleSheet("QFrame { background-color: #22263C; border: 1px solid #323859; border-radius: 14px; }");
    QHBoxLayout *legendLayout = new QHBoxLayout(legendFrame);
    legendLayout->setAlignment(Qt::AlignCenter);
    legendLayout->setContentsMargins(16, 8, 16, 8);
    legendLayout->setSpacing(16);

    auto createLegendItem = [legendFrame](const QString& text, int swatchType) {
        QWidget* item = new QWidget(legendFrame);
        item->setStyleSheet("background: transparent; border: none;");
        QHBoxLayout* l = new QHBoxLayout(item);
        l->setContentsMargins(0, 0, 0, 0);
        l->setSpacing(6);
        QLabel* badge = new QLabel(item);
        badge->setFixedSize(16, 16);
        badge->setStyleSheet(Theme::seatLegendSwatchStyle(swatchType));
        QLabel* lbl = new QLabel(text, item);
        lbl->setStyleSheet("color: #C8CEE8; font-size: 11px; font-weight: 600; background: transparent; border: none;");
        l->addWidget(badge);
        l->addWidget(lbl);
        return item;
    };

    legendLayout->addWidget(createLegendItem("Thường trống", 0));
    legendLayout->addWidget(createLegendItem("Thường đã đặt", 1));
    legendLayout->addWidget(createLegendItem("VIP trống", 2));
    legendLayout->addWidget(createLegendItem("VIP đã đặt", 3));
    legendLayout->addWidget(createLegendItem("Đang chọn", 4));
    layout->addWidget(legendFrame);

    QLabel *lblScreen = new QLabel("MÀN HÌNH CHIẾU PHIM", page);
    lblScreen->setAlignment(Qt::AlignCenter);
    lblScreen->setFixedHeight(34);
    lblScreen->setStyleSheet(Theme::screenBannerStyle());
    layout->addWidget(lblScreen);

    QFrame* auditoriumFrame = new QFrame(page);
    auditoriumFrame->setObjectName("cashierAuditorium");
    auditoriumFrame->setStyleSheet("QFrame#cashierAuditorium { background-color: #1D2034; border: 1px solid #303654; border-radius: 20px; }");
    QVBoxLayout* audLayout = new QVBoxLayout(auditoriumFrame);
    audLayout->setContentsMargins(12, 12, 12, 12);

    QScrollArea *scrollArea = new QScrollArea(auditoriumFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { background: transparent; border: none; }");

    seatGridWidget = new QWidget(scrollArea);
    seatGridWidget->setStyleSheet("background: transparent;");
    seatGridLayout = new QGridLayout(seatGridWidget);
    seatGridLayout->setSpacing(7);
    seatGridLayout->setAlignment(Qt::AlignCenter);

    scrollArea->setWidget(seatGridWidget);
    audLayout->addWidget(scrollArea);
    layout->addWidget(auditoriumFrame, 1);

    QFrame* bottomBar = new QFrame(page);
    bottomBar->setStyleSheet("QFrame { background-color: #22263C; border: 1px solid #323859; border-radius: 16px; }");
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomBar);
    bottomLayout->setContentsMargins(18, 10, 18, 10);
    bottomLayout->setSpacing(12);

    lblSelectedSeats = new QLabel("Ghế đã chọn: Chưa chọn ghế", bottomBar);
    lblSelectedSeats->setStyleSheet("font-size: 14px; font-weight: bold; color: #FFD166; background: transparent; border: none;");
    lblSelectedSeats->setWordWrap(true);

    QPushButton* backBtn = new QPushButton("Quay Lại", bottomBar);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet(Theme::secondaryButtonStyle(42, 14));

    QPushButton* nextBtn = new QPushButton("Tiếp Tục Chọn Combo", bottomBar);
    nextBtn->setCursor(Qt::PointingHandCursor);
    nextBtn->setStyleSheet(Theme::primaryButtonStyle(42, 14));

    bottomLayout->addWidget(lblSelectedSeats, 1);
    bottomLayout->addWidget(backBtn);
    bottomLayout->addWidget(nextBtn);
    layout->addWidget(bottomBar);

    connect(backBtn, &QPushButton::clicked, this, [this]() { stepWidget->setCurrentIndex(1); });
    connect(nextBtn, &QPushButton::clicked, this, [this]() {
        if (selectedSeats.size() > 0) {
            loadCombos();
            stepWidget->setCurrentIndex(3);
        } else {
            QMessageBox::warning(this, "Thông Báo", "Vui lòng chọn ít nhất 1 ghế.");
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
    if (!sm) return;

    int rows = sm->getSoHang();
    int cols = sm->getSoCot();

    for (int i = 0; i < rows; ++i) {
        QLabel* rowLblLeft = new QLabel(QString(QChar('A' + i)), seatGridWidget);
        rowLblLeft->setFixedSize(22, 36);
        rowLblLeft->setAlignment(Qt::AlignCenter);
        rowLblLeft->setStyleSheet("color: #707899; font-size: 11px; font-weight: bold; background: transparent;");
        seatGridLayout->addWidget(rowLblLeft, i, 0);

        for (int j = 0; j < cols; ++j) {
            QString seatName = QString(QChar('A' + i)) + QString::number(j + 1);
            QPushButton* btn = new QPushButton(seatName, seatGridWidget);
            btn->setFixedSize(38, 36);

            int status = sm->getSeatStatus(i, j);
            btn->setStyleSheet(Theme::seatButtonStyle(status, false));

            if (status == 1 || status == 3) {
                btn->setEnabled(false);
                btn->setCursor(Qt::ForbiddenCursor);
            } else {
                btn->setCursor(Qt::PointingHandCursor);
                btn->setCheckable(true);
                connect(btn, &QPushButton::toggled, this, [this, i, j, btn, status](bool checked) {
                    if (checked) {
                        selectedSeats.push_back({i, j});
                        btn->setStyleSheet(Theme::seatButtonStyle(status, true));
                    } else {
                        for (int k = 0; k < selectedSeats.size(); ++k) {
                            if (selectedSeats[k].first == i && selectedSeats[k].second == j) {
                                selectedSeats.removeAt(k);
                                break;
                            }
                        }
                        btn->setStyleSheet(Theme::seatButtonStyle(status, false));
                    }
                    updateSelectedSeatsLabel();
                });
            }

            seatGridLayout->addWidget(btn, i, j + 1);
        }

        QLabel* rowLblRight = new QLabel(QString(QChar('A' + i)), seatGridWidget);
        rowLblRight->setFixedSize(22, 36);
        rowLblRight->setAlignment(Qt::AlignCenter);
        rowLblRight->setStyleSheet("color: #707899; font-size: 11px; font-weight: bold; background: transparent;");
        seatGridLayout->addWidget(rowLblRight, i, cols + 1);
    }
}

void SaleWidget::setupStep4(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 4, 0, 0);
    layout->setSpacing(12);

    QLabel *lblTitle = new QLabel("CHỌN COMBO BẮP NƯỚC (NẾU CÓ)", page);
    lblTitle->setStyleSheet(Theme::sectionTitleStyle());
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
    comboTable->setColumnWidth(3, 130);
    comboTable->verticalHeader()->setDefaultSectionSize(52);
    comboTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    comboTable->verticalHeader()->setVisible(false);
    layout->addWidget(comboTable, 1);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);

    QPushButton* backBtn = new QPushButton("Quay Lại", page);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet(Theme::secondaryButtonStyle(42, 14));

    QPushButton* skipBtn = new QPushButton("Bỏ Qua Combo", page);
    skipBtn->setCursor(Qt::PointingHandCursor);
    skipBtn->setStyleSheet(Theme::goldOutlineButtonStyle(42, 14));

    QPushButton* nextBtn = new QPushButton("Tiếp Tục Xác Nhận", page);
    nextBtn->setCursor(Qt::PointingHandCursor);
    nextBtn->setStyleSheet(Theme::primaryButtonStyle(42, 14));

    btnLayout->addWidget(backBtn);
    btnLayout->addWidget(skipBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(nextBtn);
    layout->addLayout(btnLayout);

    connect(backBtn, &QPushButton::clicked, this, [this]() { stepWidget->setCurrentIndex(2); });
    connect(skipBtn, &QPushButton::clicked, this, [this]() {
        selectedCombos = DynamicArray<std::pair<std::string, int>>();
        generatePreview();
        stepWidget->setCurrentIndex(4);
    });
    connect(nextBtn, &QPushButton::clicked, this, [this]() {
        selectedCombos = DynamicArray<std::pair<std::string, int>>();
        for (int i = 0; i < comboTable->rowCount(); ++i) {
            QWidget* pWidget = comboTable->cellWidget(i, 3);
            QSpinBox* spin = pWidget ? pWidget->findChild<QSpinBox*>() : nullptr;
            if (spin && spin->value() > 0) {
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
    QFont boldFont = comboTable->font();
    boldFont.setBold(true);

    for (int i = 0; i < combos.size(); ++i) {
        comboTable->insertRow(i);

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::fromStdString(combos[i].getMaCombo()));
        idItem->setTextAlignment(Qt::AlignCenter);
        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
        idItem->setForeground(QBrush(QColor("#8C92FF")));
        idItem->setFont(boldFont);
        comboTable->setItem(i, 0, idItem);

        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(combos[i].getTenCombo()));
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        nameItem->setFont(boldFont);
        comboTable->setItem(i, 1, nameItem);

        QTableWidgetItem* priceItem = new QTableWidgetItem(QString("%L1 VND").arg(combos[i].getGia(), 0, 'f', 0));
        priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        priceItem->setFlags(priceItem->flags() & ~Qt::ItemIsEditable);
        priceItem->setFont(boldFont);
        priceItem->setForeground(QBrush(QColor("#FFD166")));
        comboTable->setItem(i, 2, priceItem);

        QWidget* pWidget = new QWidget();
        pWidget->setStyleSheet("background: transparent;");
        QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
        pLayout->setContentsMargins(4, 2, 4, 2);
        pLayout->setAlignment(Qt::AlignCenter);

        QSpinBox* spinBox = new QSpinBox(pWidget);
        spinBox->setRange(0, 20);
        spinBox->setValue(0);
        spinBox->setAlignment(Qt::AlignCenter);
        spinBox->setFixedSize(94, 34);
        spinBox->setStyleSheet(R"(
            QSpinBox {
                background-color: #1A1D2F;
                color: #F3F4FA;
                font-size: 13px;
                font-weight: bold;
                border: 1.5px solid #3D446B;
                border-radius: 10px;
                padding: 0px 6px;
            }
            QSpinBox:focus {
                border: 1.5px solid #FF6B6B;
            }
        )");
        pLayout->addWidget(spinBox);
        comboTable->setCellWidget(i, 3, pWidget);
    }
}

void SaleWidget::setupStep5(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(14);
    layout->setAlignment(Qt::AlignCenter);

    QFrame *card = new QFrame(page);
    card->setObjectName("saleConfirmCard");
    card->setMinimumWidth(560);
    card->setMaximumWidth(660);
    card->setStyleSheet(Theme::luxuryCardStyle("saleConfirmCard"));

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(30, 26, 30, 26);
    cardLayout->setSpacing(14);

    QLabel* eyebrow = new QLabel("BÁN VÉ TRỰC TIẾP TẠI QUẦY", card);
    eyebrow->setAlignment(Qt::AlignCenter);
    eyebrow->setStyleSheet(Theme::sectionEyebrowStyle());

    QLabel* title = new QLabel("Xác Nhận Thanh Toán", card);
    title->setStyleSheet("color: #F3F4FA; font-size: 21px; font-weight: bold; border: none; background: transparent;");
    title->setAlignment(Qt::AlignCenter);

    QFrame* divider = new QFrame(card);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("background-color: #323859; max-height: 1px; border: none;");

    previewLabel = new QLabel(card);
    previewLabel->setStyleSheet("color: #F3F4FA; font-size: 13px; border: none; background: transparent; padding: 10px;");
    previewLabel->setWordWrap(true);
    previewLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    QScrollArea* scroll = new QScrollArea(card);
    scroll->setWidgetResizable(true);
    scroll->setMinimumHeight(240);
    scroll->setStyleSheet("QScrollArea { background-color: #1A1D2F; border: 1px solid #303654; border-radius: 14px; }");
    scroll->setWidget(previewLabel);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);

    QPushButton* backBtn = new QPushButton("Quay Lại", card);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet(Theme::secondaryButtonStyle(44, 14));

    QPushButton* nextBtn = new QPushButton("Xác Nhận && Thanh Toán", card);
    nextBtn->setCursor(Qt::PointingHandCursor);
    nextBtn->setStyleSheet(Theme::successButtonStyle(44, 14));

    btnLayout->addWidget(backBtn, 1);
    btnLayout->addWidget(nextBtn, 2);

    cardLayout->addWidget(eyebrow);
    cardLayout->addWidget(title);
    cardLayout->addWidget(divider);
    cardLayout->addWidget(scroll, 1);
    cardLayout->addLayout(btnLayout);

    layout->addWidget(card);

    connect(backBtn, &QPushButton::clicked, this, [this]() { stepWidget->setCurrentIndex(3); });
    connect(nextBtn, &QPushButton::clicked, this, [this]() {
        createTickets();
        stepWidget->setCurrentIndex(5);
    });
}

void SaleWidget::generatePreview() {
    Showtime* st = showtimeManager->findById(selectedShowtimeId);
    Movie* mv = movieManager->findById(selectedMovieId);
    if (!st || !mv) return;

    SeatMap* sm = showtimeManager->findSeatMap(selectedShowtimeId);

    QString preview = QString(
        "<div style='line-height: 1.7; color: #F3F4FA;'>"
        "<div style='color:#8C92FF; font-weight:bold; letter-spacing:1px; margin-bottom:4px;'>THÔNG TIN SUẤT CHIẾU</div>"
        "• <span style='color:#9AA2C2;'>Phim:</span> <b>%1</b><br>"
        "• <span style='color:#9AA2C2;'>Suất chiếu:</span> <b>%2</b> &nbsp;|&nbsp; <b style='color:#FFD166;'>%3</b> &nbsp;|&nbsp; <span style='color:#9AA2C2;'>Phòng:</span> <b>%4</b><br><br>")
        .arg(QString::fromStdString(mv->getTenPhim()).toUpper())
        .arg(QString::fromStdString(st->getNgayChieu()))
        .arg(QString::fromStdString(st->getGioChieu()))
        .arg(QString::fromStdString(st->getMaPhong()));

    double total = 0;
    preview += "<div style='color:#8C92FF; font-weight:bold; letter-spacing:1px; margin-bottom:4px;'>DANH SÁCH GHẾ</div>";
    for (int i = 0; i < selectedSeats.size(); ++i) {
        int r = selectedSeats[i].first;
        int c = selectedSeats[i].second;
        int type = sm ? sm->getSeatStatus(r, c) : 0;
        double price = PriceCalculator::calculateTicketPrice(type, st->getNgayChieu(), st->getGioChieu());
        total += price;
        bool isVip = (type == 2 || type == 3);
        preview += QString("• Ghế <b>%1%2</b> (%3): <b>%L4 VND</b><br>")
            .arg((char)('A' + r))
            .arg(c + 1)
            .arg(isVip ? "<span style='color: #FFD166; font-weight:bold;'>VIP</span>" : "<span style='color:#A3ABC9;'>Thường</span>")
            .arg(price, 0, 'f', 0);
    }

    if (selectedCombos.size() > 0) {
        preview += "<br><div style='color:#8C92FF; font-weight:bold; letter-spacing:1px; margin-bottom:4px;'>COMBO BẮP NƯỚC</div>";
        for (int i = 0; i < selectedCombos.size(); ++i) {
            Combo* cb = comboManager->findById(selectedCombos[i].first);
            if (cb) {
                double cbTotal = cb->getGia() * selectedCombos[i].second;
                total += cbTotal;
                preview += QString("• %1 &times; %2 = <b style='color:#FFD166;'>%L3 VND</b><br>")
                    .arg(QString::fromStdString(cb->getTenCombo()))
                    .arg(selectedCombos[i].second)
                    .arg(cbTotal, 0, 'f', 0);
            }
        }
    }

    preview += QString("<br><div style='background-color:#2E2538; border:1px solid #6E4458; padding:10px; text-align:center; font-size:17px; color:#FFD166;'>"
                       "<b>TỔNG THANH TOÁN: %L1 VND</b></div></div>").arg(total, 0, 'f', 0);
    previewLabel->setText(preview);
}

void SaleWidget::setupStep6(QWidget* page) {
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setAlignment(Qt::AlignCenter);

    QFrame *card = new QFrame(page);
    card->setObjectName("saleDoneCard");
    card->setFixedWidth(480);
    card->setStyleSheet(R"(
        QFrame#saleDoneCard {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1B3335, stop:1 #1F2338);
            border: 1.5px solid #2A6E5C;
            border-radius: 22px;
        }
    )");
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(34, 34, 34, 34);
    cardLayout->setSpacing(14);
    cardLayout->setAlignment(Qt::AlignCenter);

    QLabel *badge = new QLabel("HOÀN TẤT GIAO DỊCH", card);
    badge->setAlignment(Qt::AlignCenter);
    badge->setStyleSheet("color: #34D399; font-size: 11px; font-weight: bold; letter-spacing: 2.5px; border: none; background: transparent;");

    confirmLabel = new QLabel("Thanh Toán Thành Công!", card);
    confirmLabel->setStyleSheet("color: #F3F4FA; font-size: 22px; font-weight: bold; border: none; background: transparent;");
    confirmLabel->setAlignment(Qt::AlignCenter);

    QLabel *sub = new QLabel("Vé đã được xuất và kích hoạt hợp lệ ngay lập tức.", card);
    sub->setStyleSheet("color: #A3ABC9; font-size: 13px; border: none; background: transparent;");
    sub->setAlignment(Qt::AlignCenter);
    sub->setWordWrap(true);

    QPushButton* inVeBtn = new QPushButton("Hoàn Tất / Bán Đơn Tiếp", card);
    inVeBtn->setCursor(Qt::PointingHandCursor);
    inVeBtn->setStyleSheet(Theme::primaryButtonStyle(46, 14));

    cardLayout->addWidget(badge);
    cardLayout->addWidget(confirmLabel);
    cardLayout->addWidget(sub);
    cardLayout->addSpacing(8);
    cardLayout->addWidget(inVeBtn);
    layout->addWidget(card);

    connect(inVeBtn, &QPushButton::clicked, this, [this]() {
        loadMovies();
        stepWidget->setCurrentIndex(0);
    });
}

void SaleWidget::createTickets() {
    Showtime* st = showtimeManager->findById(selectedShowtimeId);
    SeatMap* sm = showtimeManager->findSeatMap(selectedShowtimeId);
    if (!st || !sm) return;

    std::string comboStr = "";
    double totalComboPrice = 0.0;
    for (int i = 0; i < selectedCombos.size(); ++i) {
        Combo* cb = comboManager->findById(selectedCombos[i].first);
        if (cb) {
            if (!comboStr.empty()) comboStr += "; ";
            comboStr += cb->getTenCombo() + " x" + std::to_string(selectedCombos[i].second);
            totalComboPrice += cb->getGia() * selectedCombos[i].second;
        }
    }
    double comboPricePerTicket = (selectedSeats.size() > 0) ? (totalComboPrice / selectedSeats.size()) : 0;

    for (int i = 0; i < selectedSeats.size(); ++i) {
        int r = selectedSeats[i].first;
        int c = selectedSeats[i].second;
        if (!sm->isSeatAvailable(r, c)) {
            QMessageBox::warning(this, "Lỗi", QString("Ghế %1%2 đã có người đặt!").arg((char)('A' + r)).arg(c + 1));
            return;
        }
    }

    for (int i = 0; i < selectedSeats.size(); ++i) {
        int r = selectedSeats[i].first;
        int c = selectedSeats[i].second;
        double seatPrice = PriceCalculator::calculateTicketPrice(sm->getSeatStatus(r, c), st->getNgayChieu(), st->getGioChieu());
        double finalPrice = seatPrice + comboPricePerTicket;

        std::string ngayMua = QDate::currentDate().toString("dd/MM/yyyy").toStdString();
        ticketManager->createTicket(selectedShowtimeId, "", maNhanVien, r, c, finalPrice, comboStr, ngayMua, "", "VALID");
        sm->bookSeat(r, c);
    }
    showtimeManager->saveSeatMaps();
    ticketManager->save();
}
