#include "MovieManagementWidget.h"
#include "../common/Theme.h"
#include "managers/MovieManager.h"
#include "models/Movie.h"
#include <QHeaderView>
#include <QMessageBox>
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
    : QWidget(parent), movieManager(movieMgr)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(26, 22, 26, 20);
    layout->setSpacing(14);

    QHBoxLayout* topRow = new QHBoxLayout();
    QVBoxLayout* titleCol = new QVBoxLayout();
    titleCol->setSpacing(2);
    QLabel* lblTitle = new QLabel("Quản lý phim", this);
    lblTitle->setStyleSheet(Theme::sectionTitleStyle());
    titleCol->addWidget(lblTitle);

    topRow->addLayout(titleCol);
    topRow->addStretch();

    btnAdd = new QPushButton("+ Thêm Phim Mới", this);
    btnAdd->setCursor(Qt::PointingHandCursor);
    btnAdd->setStyleSheet(Theme::primaryButtonStyle(38, 13));

    btnEdit = new QPushButton("Sửa Phim", this);
    btnEdit->setCursor(Qt::PointingHandCursor);
    btnEdit->setStyleSheet(Theme::goldOutlineButtonStyle(38, 13));

    btnDelete = new QPushButton("Ngừng Chiếu", this);
    btnDelete->setCursor(Qt::PointingHandCursor);
    btnDelete->setStyleSheet(Theme::dangerOutlineButtonStyle(38, 13));

    topRow->addWidget(btnAdd, 0, Qt::AlignVCenter);
    topRow->addWidget(btnEdit, 0, Qt::AlignVCenter);
    topRow->addWidget(btnDelete, 0, Qt::AlignVCenter);
    layout->addLayout(topRow);

    table = new QTableWidget(0, 6, this);
    table->setHorizontalHeaderLabels({"Mã Phim", "Poster", "Tên Phim", "Thể Loại", "Thời Lượng", "Trạng Thái"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->setIconSize(QSize(38, 50));
    table->verticalHeader()->setDefaultSectionSize(58);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    table->setColumnWidth(1, 76);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);

    layout->addWidget(table, 1);

    connect(btnAdd, &QPushButton::clicked, this, &MovieManagementWidget::addMovie);
    connect(btnEdit, &QPushButton::clicked, this, &MovieManagementWidget::editMovie);
    connect(btnDelete, &QPushButton::clicked, this, &MovieManagementWidget::deleteMovie);

    loadData();
}

void MovieManagementWidget::loadData() {
    table->setRowCount(0);
    auto& movies = movieManager->getAllMovies();
    QFont boldFont = table->font();
    boldFont.setBold(true);

    for (int i = 0; i < movies.size(); ++i) {
        table->insertRow(i);

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::fromStdString(movies[i].getMaPhim()));
        idItem->setTextAlignment(Qt::AlignCenter);
        idItem->setFont(boldFont);
        idItem->setForeground(QBrush(QColor("#64748B")));
        table->setItem(i, 0, idItem);

        QString posterPath = QString::fromStdString(movies[i].getPoster());
        QPixmap px = Theme::loadPosterPixmap(posterPath);
        QWidget* posterCell = new QWidget(table);
        posterCell->setStyleSheet("background: transparent;");
        QHBoxLayout* posterLayout = new QHBoxLayout(posterCell);
        posterLayout->setContentsMargins(0, 4, 0, 4);
        posterLayout->setAlignment(Qt::AlignCenter);
        QLabel* posterLbl = new QLabel(posterCell);
        posterLbl->setFixedSize(36, 48);
        posterLbl->setAlignment(Qt::AlignCenter);
        if (!px.isNull()) {
            posterLbl->setPixmap(Theme::roundedCoverPixmap(px, 36, 48, 5));
        } else {
            posterLbl->setText("N/A");
            posterLbl->setStyleSheet("background-color: #F1F5F9; color: #94A3B8; font-size: 9px; font-weight: 600; border-radius: 5px; border: 1px solid #E2E8F0;");
        }
        posterLayout->addWidget(posterLbl);
        table->setCellWidget(i, 1, posterCell);

        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(movies[i].getTenPhim()));
        nameItem->setFont(boldFont);
        table->setItem(i, 2, nameItem);

        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(movies[i].getTheLoai())));

        QTableWidgetItem* durationItem = new QTableWidgetItem(QString::number(movies[i].getThoiLuong()) + " phút");
        durationItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 4, durationItem);

        bool active = movies[i].getTrangThai();
        QTableWidgetItem* statusItem = new QTableWidgetItem(active ? "Đang chiếu" : "Ngừng chiếu");
        statusItem->setTextAlignment(Qt::AlignCenter);
        statusItem->setFont(boldFont);
        statusItem->setForeground(QBrush(QColor(active ? "#059669" : "#94A3B8")));
        table->setItem(i, 5, statusItem);
    }
}

static void styleAdminDialog(QDialog& dialog) {
    dialog.setStyleSheet(R"(
        QDialog {
            background-color: #FFFFFF;
            border: 1px solid #E5E7EB;
            border-radius: 10px;
        }
    )");
}

void MovieManagementWidget::addMovie() {
    QDialog dialog(this);
    dialog.setWindowTitle("Thêm Phim Mới");
    dialog.setMinimumWidth(650);
    styleAdminDialog(dialog);

    QVBoxLayout *mainDialogLayout = new QVBoxLayout(&dialog);
    mainDialogLayout->setContentsMargins(26, 24, 26, 24);
    mainDialogLayout->setSpacing(16);

    QLabel *dlgTitle = new QLabel("Thêm Phim Mới", &dialog);
    dlgTitle->setStyleSheet("color: #D97706; font-size: 19px; font-weight: 600;");
    mainDialogLayout->addWidget(dlgTitle);

    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(20);

    QVBoxLayout *leftForm = new QVBoxLayout();
    leftForm->setSpacing(6);
    auto lbl = [&dialog](const QString& t) {
        QLabel* l = new QLabel(t, &dialog);
        l->setStyleSheet("color: #64748B; font-size: 12px; font-weight: 600;");
        return l;
    };

    QLineEdit *edtTen = new QLineEdit(&dialog); edtTen->setPlaceholderText("Nhập tên phim");
    QLineEdit *edtTheLoai = new QLineEdit(&dialog); edtTheLoai->setPlaceholderText("Hành động, Tâm lý, Hoạt hình...");
    QSpinBox *spnThoiLuong = new QSpinBox(&dialog); spnThoiLuong->setRange(30, 300); spnThoiLuong->setValue(120); spnThoiLuong->setSuffix(" phút");
    QSpinBox *spnDoTuoi = new QSpinBox(&dialog); spnDoTuoi->setRange(0, 21); spnDoTuoi->setValue(13); spnDoTuoi->setSuffix(" tuổi");
    QLineEdit *edtMoTa = new QLineEdit(&dialog); edtMoTa->setPlaceholderText("Mô tả ngắn gọn");
    QLineEdit *edtNoiDung = new QLineEdit(&dialog); edtNoiDung->setPlaceholderText("Nội dung tóm tắt phim");

    leftForm->addWidget(lbl("Tên phim:")); leftForm->addWidget(edtTen);
    leftForm->addWidget(lbl("Thể loại:")); leftForm->addWidget(edtTheLoai);
    leftForm->addWidget(lbl("Thời lượng:")); leftForm->addWidget(spnThoiLuong);
    leftForm->addWidget(lbl("Giới hạn độ tuổi (0 = P):")); leftForm->addWidget(spnDoTuoi);
    leftForm->addWidget(lbl("Mô tả ngắn:")); leftForm->addWidget(edtMoTa);
    leftForm->addWidget(lbl("Nội dung chi tiết:")); leftForm->addWidget(edtNoiDung);
    contentLayout->addLayout(leftForm, 3);

    QVBoxLayout *rightForm = new QVBoxLayout();
    rightForm->setSpacing(8);
    rightForm->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    rightForm->addWidget(lbl("Ảnh Poster:"));

    QLabel *lblPreview = new QLabel(&dialog);
    lblPreview->setFixedSize(140, 200);
    lblPreview->setAlignment(Qt::AlignCenter);
    lblPreview->setStyleSheet("background-color: #F9FAFB; border: 1.5px solid #E5E7EB; border-radius: 8px; color: #94A3B8;");
    lblPreview->setText("Chưa chọn poster");
    lblPreview->setWordWrap(true);
    rightForm->addWidget(lblPreview, 0, Qt::AlignHCenter);

    QLineEdit *edtPoster = new QLineEdit(&dialog);
    edtPoster->setPlaceholderText("Đường dẫn poster");
    edtPoster->setText("posters/default.jpg");
    edtPoster->setReadOnly(true);
    rightForm->addWidget(edtPoster);

    QPushButton *btnBrowse = new QPushButton("Chọn Ảnh từ Máy...", &dialog);
    btnBrowse->setCursor(Qt::PointingHandCursor);
    btnBrowse->setStyleSheet(Theme::secondaryButtonStyle(36, 12));
    rightForm->addWidget(btnBrowse);

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
                lblPreview->setPixmap(Theme::roundedCoverPixmap(px, 138, 198, 12));
            }
        }
    });

    connect(cbExisting, &QComboBox::currentIndexChanged, [cbExisting, edtPoster, lblPreview](int index) {
        if (index > 0) {
            QString relPath = cbExisting->currentData().toString();
            edtPoster->setText(relPath);
            QPixmap px = Theme::loadPosterPixmap(relPath);
            if (!px.isNull()) {
                lblPreview->setPixmap(Theme::roundedCoverPixmap(px, 138, 198, 12));
            }
        }
    });

    contentLayout->addLayout(rightForm, 2);
    mainDialogLayout->addLayout(contentLayout);

    QPushButton *btnCancel = new QPushButton("Hủy Bỏ", &dialog);
    btnCancel->setCursor(Qt::PointingHandCursor);
    btnCancel->setStyleSheet(Theme::secondaryButtonStyle(40, 13));

    QPushButton *btnOk = new QPushButton("Thêm Phim", &dialog);
    btnOk->setCursor(Qt::PointingHandCursor);
    btnOk->setStyleSheet(Theme::primaryButtonStyle(40, 13));

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);
    btnLayout->addStretch();
    btnLayout->addWidget(btnCancel);
    btnLayout->addWidget(btnOk);
    mainDialogLayout->addLayout(btnLayout);

    connect(btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted && !edtTen->text().trimmed().isEmpty()) {
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

        Movie m("", edtTen->text().trimmed().toStdString(), edtTheLoai->text().trimmed().toStdString(),
                spnThoiLuong->value(), spnDoTuoi->value(),
                edtMoTa->text().trimmed().toStdString(), edtNoiDung->text().trimmed().toStdString(), finalPoster.toStdString(), true);
        if (movieManager->addMovie(m)) {
            movieManager->save();
            loadData();
        }
    }
}

void MovieManagementWidget::editMovie() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Cảnh Báo", "Vui lòng chọn phim cần sửa!");
        return;
    }
    std::string id = table->item(row, 0)->text().toStdString();
    Movie* m = movieManager->findById(id);
    if (!m) return;

    QDialog dialog(this);
    dialog.setWindowTitle("Sửa Phim: " + QString::fromStdString(m->getTenPhim()));
    dialog.setMinimumWidth(650);
    styleAdminDialog(dialog);

    QVBoxLayout *mainDialogLayout = new QVBoxLayout(&dialog);
    mainDialogLayout->setContentsMargins(26, 24, 26, 24);
    mainDialogLayout->setSpacing(16);

    QLabel *dlgTitle = new QLabel("Cập Nhật Thông Tin Phim", &dialog);
    dlgTitle->setStyleSheet("color: #D97706; font-size: 19px; font-weight: 600;");
    mainDialogLayout->addWidget(dlgTitle);

    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(20);

    QVBoxLayout *leftForm = new QVBoxLayout();
    leftForm->setSpacing(6);
    auto lbl = [&dialog](const QString& t) {
        QLabel* l = new QLabel(t, &dialog);
        l->setStyleSheet("color: #64748B; font-size: 12px; font-weight: 600;");
        return l;
    };

    QLineEdit *edtTen = new QLineEdit(QString::fromStdString(m->getTenPhim()), &dialog);
    QLineEdit *edtTheLoai = new QLineEdit(QString::fromStdString(m->getTheLoai()), &dialog);
    QSpinBox *spnThoiLuong = new QSpinBox(&dialog); spnThoiLuong->setRange(30, 300); spnThoiLuong->setValue(m->getThoiLuong()); spnThoiLuong->setSuffix(" phút");
    QSpinBox *spnDoTuoi = new QSpinBox(&dialog); spnDoTuoi->setRange(0, 21); spnDoTuoi->setValue(m->getDoTuoi()); spnDoTuoi->setSuffix(" tuổi");
    QLineEdit *edtMoTa = new QLineEdit(QString::fromStdString(m->getMoTa()), &dialog);
    QLineEdit *edtNoiDung = new QLineEdit(QString::fromStdString(m->getNoiDung()), &dialog);

    leftForm->addWidget(lbl("Tên phim:")); leftForm->addWidget(edtTen);
    leftForm->addWidget(lbl("Thể loại:")); leftForm->addWidget(edtTheLoai);
    leftForm->addWidget(lbl("Thời lượng:")); leftForm->addWidget(spnThoiLuong);
    leftForm->addWidget(lbl("Giới hạn độ tuổi (0 = P):")); leftForm->addWidget(spnDoTuoi);
    leftForm->addWidget(lbl("Mô tả ngắn:")); leftForm->addWidget(edtMoTa);
    leftForm->addWidget(lbl("Nội dung chi tiết:")); leftForm->addWidget(edtNoiDung);
    contentLayout->addLayout(leftForm, 3);

    QVBoxLayout *rightForm = new QVBoxLayout();
    rightForm->setSpacing(8);
    rightForm->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    rightForm->addWidget(lbl("Ảnh Poster:"));

    QLabel *lblPreview = new QLabel(&dialog);
    lblPreview->setFixedSize(140, 200);
    lblPreview->setAlignment(Qt::AlignCenter);
    lblPreview->setStyleSheet("background-color: #F9FAFB; border: 1.5px solid #E5E7EB; border-radius: 8px; color: #94A3B8;");

    QString currentPoster = QString::fromStdString(m->getPoster());
    QPixmap initPx = Theme::loadPosterPixmap(currentPoster);
    if (!initPx.isNull()) {
        lblPreview->setPixmap(Theme::roundedCoverPixmap(initPx, 138, 198, 12));
    } else {
        lblPreview->setText("Chưa có ảnh");
    }
    rightForm->addWidget(lblPreview, 0, Qt::AlignHCenter);

    QLineEdit *edtPoster = new QLineEdit(&dialog);
    edtPoster->setText(currentPoster);
    edtPoster->setReadOnly(true);
    rightForm->addWidget(edtPoster);

    QPushButton *btnBrowse = new QPushButton("Thay Đổi Ảnh từ Máy...", &dialog);
    btnBrowse->setCursor(Qt::PointingHandCursor);
    btnBrowse->setStyleSheet(Theme::secondaryButtonStyle(36, 12));
    rightForm->addWidget(btnBrowse);

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
                lblPreview->setPixmap(Theme::roundedCoverPixmap(px, 138, 198, 12));
            }
        }
    });

    connect(cbExisting, &QComboBox::currentIndexChanged, [cbExisting, edtPoster, lblPreview](int index) {
        if (index > 0) {
            QString relPath = cbExisting->currentData().toString();
            edtPoster->setText(relPath);
            QPixmap px = Theme::loadPosterPixmap(relPath);
            if (!px.isNull()) {
                lblPreview->setPixmap(Theme::roundedCoverPixmap(px, 138, 198, 12));
            }
        }
    });

    contentLayout->addLayout(rightForm, 2);
    mainDialogLayout->addLayout(contentLayout);

    QPushButton *btnCancel = new QPushButton("Hủy Bỏ", &dialog);
    btnCancel->setCursor(Qt::PointingHandCursor);
    btnCancel->setStyleSheet(Theme::secondaryButtonStyle(40, 13));

    QPushButton *btnOk = new QPushButton("Lưu Thay Đổi", &dialog);
    btnOk->setCursor(Qt::PointingHandCursor);
    btnOk->setStyleSheet(Theme::primaryButtonStyle(40, 13));

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);
    btnLayout->addStretch();
    btnLayout->addWidget(btnCancel);
    btnLayout->addWidget(btnOk);
    mainDialogLayout->addLayout(btnLayout);

    connect(btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted && !edtTen->text().trimmed().isEmpty()) {
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

        m->setTenPhim(edtTen->text().trimmed().toStdString());
        m->setTheLoai(edtTheLoai->text().trimmed().toStdString());
        m->setThoiLuong(spnThoiLuong->value());
        m->setDoTuoi(spnDoTuoi->value());
        m->setMoTa(edtMoTa->text().trimmed().toStdString());
        m->setNoiDung(edtNoiDung->text().trimmed().toStdString());
        m->setPoster(finalPoster.toStdString());

        movieManager->updateMovie(id, *m);
        movieManager->save();
        loadData();
    }
}

void MovieManagementWidget::deleteMovie() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Cảnh Báo", "Vui lòng chọn phim cần ngừng chiếu!");
        return;
    }
    std::string id = table->item(row, 0)->text().toStdString();
    if (QMessageBox::question(this, "Xác Nhận", "Bạn có muốn xóa (ngừng chiếu) phim này?") == QMessageBox::Yes) {
        if (movieManager->deleteMovie(id)) {
            movieManager->save();
            loadData();
        }
    }
}
