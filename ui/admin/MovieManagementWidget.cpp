#include "MovieManagementWidget.h"
#include "managers/MovieManager.h"
#include "models/Movie.h"
#include "utils/IDGenerator.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>
#include <QComboBox>
#include <QPixmap>
#include <QIcon>

MovieManagementWidget::MovieManagementWidget(MovieManager* movieMgr, QWidget* parent)
    : QWidget(parent), movieManager(movieMgr) {
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(12);

    QLabel *lblTitle = new QLabel("QUẢN LÝ DANH SÁCH PHIM", this);
    lblTitle->setStyleSheet("color: #F5C542; font-size: 18px; font-weight: bold;");
    layout->addWidget(lblTitle);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);
    btnAdd = new QPushButton("+ Thêm Phim Mới", this);
    btnAdd->setCursor(Qt::PointingHandCursor);
    btnAdd->setStyleSheet("background-color: #28A745; color: white; font-weight: bold; min-height: 34px; padding: 0 15px; border-radius: 5px;");

    btnEdit = new QPushButton("✏️ Sửa Phim", this);
    btnEdit->setCursor(Qt::PointingHandCursor);
    btnEdit->setStyleSheet("background-color: #E6B042; color: #1A1B41; font-weight: bold; min-height: 34px; padding: 0 15px; border-radius: 5px;");

    btnDelete = new QPushButton("🗑️ Xóa / Ngừng Chiếu", this);
    btnDelete->setCursor(Qt::PointingHandCursor);
    btnDelete->setStyleSheet("background-color: #9E221E; color: white; font-weight: bold; min-height: 34px; padding: 0 15px; border-radius: 5px;");

    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnEdit);
    btnLayout->addWidget(btnDelete);
    btnLayout->addStretch();
    
    table = new QTableWidget(0, 6, this);
    table->setHorizontalHeaderLabels({"Mã Phim", "Poster", "Tên Phim", "Thể Loại", "Thời Lượng", "Trạng Thái"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->setIconSize(QSize(36, 48));
    table->verticalHeader()->setDefaultSectionSize(54);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    
    layout->addLayout(btnLayout);
    layout->addWidget(table);
    
    connect(btnAdd, &QPushButton::clicked, this, &MovieManagementWidget::addMovie);
    connect(btnEdit, &QPushButton::clicked, this, &MovieManagementWidget::editMovie);
    connect(btnDelete, &QPushButton::clicked, this, &MovieManagementWidget::deleteMovie);
    
    loadData();
}

static QPixmap loadMoviePixmap(const QString& posterPath) {
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
    return pixmap;
}

void MovieManagementWidget::loadData() {
    table->setRowCount(0);
    auto& movies = movieManager->getAllMovies();
    for(int i = 0; i < movies.size(); ++i) {
        table->insertRow(i);
        // Col 0: Ma Phim
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::fromStdString(movies[i].getMaPhim()));
        idItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 0, idItem);

        // Col 1: Poster Thumbnail & filename
        QString posterPath = QString::fromStdString(movies[i].getPoster());
        QPixmap px = loadMoviePixmap(posterPath);
        QTableWidgetItem* posterItem = new QTableWidgetItem(QFileInfo(posterPath).fileName());
        posterItem->setTextAlignment(Qt::AlignCenter);
        if (!px.isNull()) {
            posterItem->setIcon(QIcon(px.scaled(36, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
        }
        table->setItem(i, 1, posterItem);
        
        // Col 2: Ten Phim
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(movies[i].getTenPhim())));

        // Col 3: The Loai
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(movies[i].getTheLoai())));
        
        // Col 4: Thoi Luong
        QTableWidgetItem* durationItem = new QTableWidgetItem(QString::number(movies[i].getThoiLuong()) + " phút");
        durationItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 4, durationItem);
        
        // Col 5: Trang Thai
        QString status = movies[i].getTrangThai() ? "Đang Chiếu" : "Ngừng Chiếu";
        QTableWidgetItem* statusItem = new QTableWidgetItem(status);
        statusItem->setTextAlignment(Qt::AlignCenter);
        if(!movies[i].getTrangThai()) statusItem->setBackground(Qt::gray);
        table->setItem(i, 5, statusItem);
    }
}

void MovieManagementWidget::addMovie() {
    QDialog dialog(this);
    dialog.setWindowTitle("Thêm Phim Mới");
    dialog.setMinimumWidth(580);
    
    QVBoxLayout *mainDialogLayout = new QVBoxLayout(&dialog);
    QHBoxLayout *contentLayout = new QHBoxLayout();
    
    // Left Form: Thông tin phim
    QVBoxLayout *leftForm = new QVBoxLayout();
    QLineEdit *edtTen = new QLineEdit(&dialog); edtTen->setPlaceholderText("Tên phim");
    QLineEdit *edtTheLoai = new QLineEdit(&dialog); edtTheLoai->setPlaceholderText("Thể loại (Hành động, Hoạt hình...)");
    QSpinBox *spnThoiLuong = new QSpinBox(&dialog); spnThoiLuong->setRange(30, 300); spnThoiLuong->setValue(120); spnThoiLuong->setSuffix(" phút");
    QSpinBox *spnDoTuoi = new QSpinBox(&dialog); spnDoTuoi->setRange(0, 21); spnDoTuoi->setValue(13);
    QLineEdit *edtMoTa = new QLineEdit(&dialog); edtMoTa->setPlaceholderText("Mô tả ngắn");
    QLineEdit *edtNoiDung = new QLineEdit(&dialog); edtNoiDung->setPlaceholderText("Nội dung tóm tắt phim");
    
    leftForm->addWidget(new QLabel("Tên phim:", &dialog)); leftForm->addWidget(edtTen);
    leftForm->addWidget(new QLabel("Thể loại:", &dialog)); leftForm->addWidget(edtTheLoai);
    leftForm->addWidget(new QLabel("Thời lượng:", &dialog)); leftForm->addWidget(spnThoiLuong);
    leftForm->addWidget(new QLabel("Độ tuổi:", &dialog)); leftForm->addWidget(spnDoTuoi);
    leftForm->addWidget(new QLabel("Mô tả:", &dialog)); leftForm->addWidget(edtMoTa);
    leftForm->addWidget(new QLabel("Nội dung:", &dialog)); leftForm->addWidget(edtNoiDung);
    contentLayout->addLayout(leftForm, 3);

    // Right Form: Poster Selection & Preview
    QVBoxLayout *rightForm = new QVBoxLayout();
    rightForm->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    QLabel *lblPosterTitle = new QLabel("Poster Phim:", &dialog);
    lblPosterTitle->setStyleSheet("font-weight: bold;");
    rightForm->addWidget(lblPosterTitle);

    QLabel *lblPreview = new QLabel(&dialog);
    lblPreview->setFixedSize(130, 185);
    lblPreview->setAlignment(Qt::AlignCenter);
    lblPreview->setStyleSheet("background-color: #121330; border: 1px solid #E6B042; border-radius: 6px;");
    lblPreview->setText("Chưa chọn poster");
    lblPreview->setWordWrap(true);
    lblPreview->setScaledContents(true);
    rightForm->addWidget(lblPreview);

    QLineEdit *edtPoster = new QLineEdit(&dialog);
    edtPoster->setPlaceholderText("Đường dẫn poster");
    edtPoster->setText("posters/default.jpg");
    edtPoster->setReadOnly(true);
    rightForm->addWidget(edtPoster);

    QPushButton *btnBrowse = new QPushButton("📂 Chọn Ảnh từ Máy...", &dialog);
    btnBrowse->setCursor(Qt::PointingHandCursor);
    btnBrowse->setStyleSheet("background-color: #2C2E6A; color: white; font-weight: bold; min-height: 28px; border-radius: 4px;");
    rightForm->addWidget(btnBrowse);

    // Quick select from existing posters in posters/ folder
    QComboBox *cbExisting = new QComboBox(&dialog);
    cbExisting->addItem("-- Chọn từ kho poster có sẵn --", "");
    QDir posterDir("posters");
    QStringList posterFiles = posterDir.entryList(QStringList() << "*.jpg" << "*.png" << "*.jpeg" << "*.webp", QDir::Files);
    for (const QString& f : posterFiles) {
        cbExisting->addItem(f, "posters/" + f);
    }
    rightForm->addWidget(cbExisting);

    connect(btnBrowse, &QPushButton::clicked, [&dialog, edtPoster, lblPreview]() {
        QString file = QFileDialog::getOpenFileName(&dialog, "Chọn Ảnh Poster", "", "Images (*.png *.jpg *.jpeg *.bmp *.webp)");
        if (!file.isEmpty()) {
            edtPoster->setText(file);
            QPixmap px(file);
            if (!px.isNull()) {
                lblPreview->setPixmap(px.scaled(130, 185, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
    });

    connect(cbExisting, &QComboBox::currentIndexChanged, [cbExisting, edtPoster, lblPreview](int index) {
        if (index > 0) {
            QString relPath = cbExisting->currentData().toString();
            edtPoster->setText(relPath);
            QPixmap px = loadMoviePixmap(relPath);
            if (!px.isNull()) {
                lblPreview->setPixmap(px.scaled(130, 185, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
    });

    contentLayout->addLayout(rightForm, 2);
    mainDialogLayout->addLayout(contentLayout);
    
    // Bottom Action Buttons
    QPushButton *btnOk = new QPushButton("Thêm Phim", &dialog);
    btnOk->setStyleSheet("background-color: #28A745; color: white; font-weight: bold; min-height: 32px; border-radius: 5px;");
    QPushButton *btnCancel = new QPushButton("Hủy", &dialog);
    btnCancel->setStyleSheet("background-color: #9E221E; color: white; font-weight: bold; min-height: 32px; border-radius: 5px;");

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);
    mainDialogLayout->addLayout(btnLayout);
    
    connect(btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted && !edtTen->text().isEmpty()) {
        QString finalPoster = edtPoster->text().trimmed();
        if (finalPoster.isEmpty()) {
            finalPoster = "posters/default.jpg";
        } else {
            QFileInfo fi(finalPoster);
            if (fi.isAbsolute()) {
                QString fileName = fi.fileName();
                QString destRel = "posters/" + fileName;
                QDir().mkpath("posters");
                QString runtimeDir = QCoreApplication::applicationDirPath() + "/posters";
                QDir().mkpath(runtimeDir);

                QFile::remove(destRel);
                QFile::copy(finalPoster, destRel);
                QFile::remove(runtimeDir + "/" + fileName);
                QFile::copy(finalPoster, runtimeDir + "/" + fileName);

                finalPoster = destRel;
            }
        }

        // Manager generates the ID automatically
        Movie m("", edtTen->text().toStdString(), edtTheLoai->text().toStdString(),
                spnThoiLuong->value(), spnDoTuoi->value(), 
                edtMoTa->text().toStdString(), edtNoiDung->text().toStdString(), finalPoster.toStdString(), true);
        if(movieManager->addMovie(m)) {
            movieManager->save();
            loadData();
        }
    }
}

void MovieManagementWidget::editMovie() {
    int row = table->currentRow();
    if(row < 0) {
        QMessageBox::warning(this, "Cảnh Báo", "Vui lòng chọn phim cần sửa!");
        return;
    }
    std::string id = table->item(row, 0)->text().toStdString();
    Movie* m = movieManager->findById(id);
    if(!m) return;
    
    QDialog dialog(this);
    dialog.setWindowTitle("Sửa Phim: " + QString::fromStdString(m->getTenPhim()));
    dialog.setMinimumWidth(580);
    
    QVBoxLayout *mainDialogLayout = new QVBoxLayout(&dialog);
    QHBoxLayout *contentLayout = new QHBoxLayout();
    
    // Left Form: Thông tin phim
    QVBoxLayout *leftForm = new QVBoxLayout();
    QLineEdit *edtTen = new QLineEdit(QString::fromStdString(m->getTenPhim()), &dialog);
    QLineEdit *edtTheLoai = new QLineEdit(QString::fromStdString(m->getTheLoai()), &dialog);
    QSpinBox *spnThoiLuong = new QSpinBox(&dialog); spnThoiLuong->setRange(30, 300); spnThoiLuong->setValue(m->getThoiLuong()); spnThoiLuong->setSuffix(" phút");
    QSpinBox *spnDoTuoi = new QSpinBox(&dialog); spnDoTuoi->setRange(0, 21); spnDoTuoi->setValue(m->getDoTuoi());
    QLineEdit *edtMoTa = new QLineEdit(QString::fromStdString(m->getMoTa()), &dialog);
    QLineEdit *edtNoiDung = new QLineEdit(QString::fromStdString(m->getNoiDung()), &dialog);
    
    leftForm->addWidget(new QLabel("Tên phim:", &dialog)); leftForm->addWidget(edtTen);
    leftForm->addWidget(new QLabel("Thể loại:", &dialog)); leftForm->addWidget(edtTheLoai);
    leftForm->addWidget(new QLabel("Thời lượng:", &dialog)); leftForm->addWidget(spnThoiLuong);
    leftForm->addWidget(new QLabel("Độ tuổi:", &dialog)); leftForm->addWidget(spnDoTuoi);
    leftForm->addWidget(new QLabel("Mô tả:", &dialog)); leftForm->addWidget(edtMoTa);
    leftForm->addWidget(new QLabel("Nội dung:", &dialog)); leftForm->addWidget(edtNoiDung);
    contentLayout->addLayout(leftForm, 3);

    // Right Form: Poster Selection & Preview
    QVBoxLayout *rightForm = new QVBoxLayout();
    rightForm->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    QLabel *lblPosterTitle = new QLabel("Poster Phim:", &dialog);
    lblPosterTitle->setStyleSheet("font-weight: bold;");
    rightForm->addWidget(lblPosterTitle);

    QLabel *lblPreview = new QLabel(&dialog);
    lblPreview->setFixedSize(130, 185);
    lblPreview->setAlignment(Qt::AlignCenter);
    lblPreview->setStyleSheet("background-color: #121330; border: 1px solid #E6B042; border-radius: 6px;");
    lblPreview->setScaledContents(true);

    QString currentPoster = QString::fromStdString(m->getPoster());
    QPixmap initPx = loadMoviePixmap(currentPoster);
    if (!initPx.isNull()) {
        lblPreview->setPixmap(initPx.scaled(130, 185, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        lblPreview->setText("Chưa có ảnh");
    }
    rightForm->addWidget(lblPreview);

    QLineEdit *edtPoster = new QLineEdit(&dialog);
    edtPoster->setText(currentPoster);
    edtPoster->setReadOnly(true);
    rightForm->addWidget(edtPoster);

    QPushButton *btnBrowse = new QPushButton("📂 Thay Đổi Ảnh từ Máy...", &dialog);
    btnBrowse->setCursor(Qt::PointingHandCursor);
    btnBrowse->setStyleSheet("background-color: #2C2E6A; color: white; font-weight: bold; min-height: 28px; border-radius: 4px;");
    rightForm->addWidget(btnBrowse);

    // Quick select from existing posters in posters/ folder
    QComboBox *cbExisting = new QComboBox(&dialog);
    cbExisting->addItem("-- Chọn từ kho poster có sẵn --", "");
    QDir posterDir("posters");
    QStringList posterFiles = posterDir.entryList(QStringList() << "*.jpg" << "*.png" << "*.jpeg" << "*.webp", QDir::Files);
    for (const QString& f : posterFiles) {
        cbExisting->addItem(f, "posters/" + f);
    }
    rightForm->addWidget(cbExisting);

    connect(btnBrowse, &QPushButton::clicked, [&dialog, edtPoster, lblPreview]() {
        QString file = QFileDialog::getOpenFileName(&dialog, "Chọn Ảnh Poster", "", "Images (*.png *.jpg *.jpeg *.bmp *.webp)");
        if (!file.isEmpty()) {
            edtPoster->setText(file);
            QPixmap px(file);
            if (!px.isNull()) {
                lblPreview->setPixmap(px.scaled(130, 185, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
    });

    connect(cbExisting, &QComboBox::currentIndexChanged, [cbExisting, edtPoster, lblPreview](int index) {
        if (index > 0) {
            QString relPath = cbExisting->currentData().toString();
            edtPoster->setText(relPath);
            QPixmap px = loadMoviePixmap(relPath);
            if (!px.isNull()) {
                lblPreview->setPixmap(px.scaled(130, 185, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
    });

    contentLayout->addLayout(rightForm, 2);
    mainDialogLayout->addLayout(contentLayout);

    // Bottom Action Buttons
    QPushButton *btnOk = new QPushButton("Lưu Thay Đổi", &dialog);
    btnOk->setStyleSheet("background-color: #28A745; color: white; font-weight: bold; min-height: 32px; border-radius: 5px;");
    QPushButton *btnCancel = new QPushButton("Hủy", &dialog);
    btnCancel->setStyleSheet("background-color: #9E221E; color: white; font-weight: bold; min-height: 32px; border-radius: 5px;");

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);
    mainDialogLayout->addLayout(btnLayout);

    connect(btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted && !edtTen->text().isEmpty()) {
        QString finalPoster = edtPoster->text().trimmed();
        if (finalPoster.isEmpty()) {
            finalPoster = "posters/default.jpg";
        } else {
            QFileInfo fi(finalPoster);
            if (fi.isAbsolute()) {
                QString fileName = fi.fileName();
                QString destRel = "posters/" + fileName;
                QDir().mkpath("posters");
                QString runtimeDir = QCoreApplication::applicationDirPath() + "/posters";
                QDir().mkpath(runtimeDir);

                QFile::remove(destRel);
                QFile::copy(finalPoster, destRel);
                QFile::remove(runtimeDir + "/" + fileName);
                QFile::copy(finalPoster, runtimeDir + "/" + fileName);

                finalPoster = destRel;
            }
        }

        m->setTenPhim(edtTen->text().toStdString());
        m->setTheLoai(edtTheLoai->text().toStdString());
        m->setThoiLuong(spnThoiLuong->value());
        m->setDoTuoi(spnDoTuoi->value());
        m->setMoTa(edtMoTa->text().toStdString());
        m->setNoiDung(edtNoiDung->text().toStdString());
        m->setPoster(finalPoster.toStdString());

        movieManager->updateMovie(id, *m);
        movieManager->save();
        loadData();
    }
}

void MovieManagementWidget::deleteMovie() {
    int row = table->currentRow();
    if(row >= 0) {
        std::string id = table->item(row, 0)->text().toStdString();
        if(QMessageBox::question(this, "Xác Nhận", "Bạn có muốn xóa (ngừng chiếu) phim này?") == QMessageBox::Yes) {
            if(movieManager->deleteMovie(id)) {
                movieManager->save();
                loadData();
            }
        }
    }
}
