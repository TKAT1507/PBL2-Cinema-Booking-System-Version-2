#include "MovieManagementWidget.h"
#include "managers/MovieManager.h"
#include "models/Movie.h"
#include "utils/IDGenerator.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QHBoxLayout>
#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>

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
    btnAdd = new QPushButton("Thêm Phim", this);
    btnAdd->setCursor(Qt::PointingHandCursor);
    btnAdd->setStyleSheet("background-color: #28A745; color: white; font-weight: bold; min-height: 34px; border-radius: 5px;");

    btnEdit = new QPushButton("Sửa Phim", this);
    btnEdit->setCursor(Qt::PointingHandCursor);
    btnEdit->setStyleSheet("background-color: #E6B042; color: #1A1B41; font-weight: bold; min-height: 34px; border-radius: 5px;");

    btnDelete = new QPushButton("Xóa / Ngừng Chiếu", this);
    btnDelete->setCursor(Qt::PointingHandCursor);
    btnDelete->setStyleSheet("background-color: #9E221E; color: white; font-weight: bold; min-height: 34px; border-radius: 5px;");

    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnEdit);
    btnLayout->addWidget(btnDelete);
    btnLayout->addStretch();
    
    table = new QTableWidget(0, 5, this);
    table->setHorizontalHeaderLabels({"Ma Phim", "Ten Phim", "The Loai", "Thoi Luong", "Trang Thai"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    layout->addLayout(btnLayout);
    layout->addWidget(table);
    
    connect(btnAdd, &QPushButton::clicked, this, &MovieManagementWidget::addMovie);
    connect(btnEdit, &QPushButton::clicked, this, &MovieManagementWidget::editMovie);
    connect(btnDelete, &QPushButton::clicked, this, &MovieManagementWidget::deleteMovie);
    
    loadData();
}

void MovieManagementWidget::loadData() {
    table->setRowCount(0);
    auto& movies = movieManager->getAllMovies();
    for(int i = 0; i < movies.size(); ++i) {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(movies[i].getMaPhim())));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(movies[i].getTenPhim())));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(movies[i].getTheLoai())));
        
        QTableWidgetItem* durationItem = new QTableWidgetItem(QString::number(movies[i].getThoiLuong()) + " phut");
        durationItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 3, durationItem);
        
        QString status = movies[i].getTrangThai() ? "Dang Chieu" : "Ngung Chieu";
        QTableWidgetItem* statusItem = new QTableWidgetItem(status);
        if(!movies[i].getTrangThai()) statusItem->setBackground(Qt::gray);
        table->setItem(i, 4, statusItem);
    }
}

void MovieManagementWidget::addMovie() {
    QDialog dialog(this);
    dialog.setWindowTitle("Them Phim");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QLineEdit *edtTen = new QLineEdit(&dialog); edtTen->setPlaceholderText("Ten phim");
    QLineEdit *edtTheLoai = new QLineEdit(&dialog); edtTheLoai->setPlaceholderText("The loai");
    QSpinBox *spnThoiLuong = new QSpinBox(&dialog); spnThoiLuong->setRange(30, 300); spnThoiLuong->setValue(120); spnThoiLuong->setSuffix(" phut");
    QSpinBox *spnDoTuoi = new QSpinBox(&dialog); spnDoTuoi->setRange(0, 21); spnDoTuoi->setValue(13);
    QLineEdit *edtMoTa = new QLineEdit(&dialog); edtMoTa->setPlaceholderText("Mo ta ngan");
    QLineEdit *edtNoiDung = new QLineEdit(&dialog); edtNoiDung->setPlaceholderText("Noi dung");
    
    layout->addWidget(new QLabel("Ten phim:", &dialog)); layout->addWidget(edtTen);
    layout->addWidget(new QLabel("The loai:", &dialog)); layout->addWidget(edtTheLoai);
    layout->addWidget(new QLabel("Thoi luong:", &dialog)); layout->addWidget(spnThoiLuong);
    layout->addWidget(new QLabel("Do tuoi:", &dialog)); layout->addWidget(spnDoTuoi);
    layout->addWidget(new QLabel("Mo ta:", &dialog)); layout->addWidget(edtMoTa);
    layout->addWidget(new QLabel("Noi dung:", &dialog)); layout->addWidget(edtNoiDung);
    
    QPushButton *btnOk = new QPushButton("Them", &dialog);
    QPushButton *btnCancel = new QPushButton("Huy", &dialog);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnOk); btnLayout->addWidget(btnCancel);
    layout->addLayout(btnLayout);
    
    connect(btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted && !edtTen->text().isEmpty()) {
        // Manager generates the ID automatically
        Movie m("", edtTen->text().toStdString(), edtTheLoai->text().toStdString(),
                spnThoiLuong->value(), spnDoTuoi->value(), 
                edtMoTa->text().toStdString(), edtNoiDung->text().toStdString(), "poster.jpg", true);
        if(movieManager->addMovie(m)) {
            movieManager->save();
            loadData();
        }
    }
}

void MovieManagementWidget::editMovie() {
    int row = table->currentRow();
    if(row < 0) {
        QMessageBox::warning(this, "Canh bao", "Vui long chon phim can sua!");
        return;
    }
    std::string id = table->item(row, 0)->text().toStdString();
    Movie* m = movieManager->findById(id);
    if(!m) return;
    
    QDialog dialog(this);
    dialog.setWindowTitle("Sua Phim");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QLineEdit *edtTen = new QLineEdit(QString::fromStdString(m->getTenPhim()), &dialog);
    QLineEdit *edtTheLoai = new QLineEdit(QString::fromStdString(m->getTheLoai()), &dialog);
    QSpinBox *spnThoiLuong = new QSpinBox(&dialog); spnThoiLuong->setRange(30, 300); spnThoiLuong->setValue(m->getThoiLuong()); spnThoiLuong->setSuffix(" phut");
    QSpinBox *spnDoTuoi = new QSpinBox(&dialog); spnDoTuoi->setRange(0, 21); spnDoTuoi->setValue(m->getDoTuoi());
    QLineEdit *edtMoTa = new QLineEdit(QString::fromStdString(m->getMoTa()), &dialog);
    QLineEdit *edtNoiDung = new QLineEdit(QString::fromStdString(m->getNoiDung()), &dialog);
    
    layout->addWidget(new QLabel("Ten phim:", &dialog)); layout->addWidget(edtTen);
    layout->addWidget(new QLabel("The loai:", &dialog)); layout->addWidget(edtTheLoai);
    layout->addWidget(new QLabel("Thoi luong:", &dialog)); layout->addWidget(spnThoiLuong);
    layout->addWidget(new QLabel("Do tuoi:", &dialog)); layout->addWidget(spnDoTuoi);
    layout->addWidget(new QLabel("Mo ta:", &dialog)); layout->addWidget(edtMoTa);
    layout->addWidget(new QLabel("Noi dung:", &dialog)); layout->addWidget(edtNoiDung);
    
    QPushButton *btnOk = new QPushButton("Luu", &dialog);
    QPushButton *btnCancel = new QPushButton("Huy", &dialog);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnOk); btnLayout->addWidget(btnCancel);
    layout->addLayout(btnLayout);
    connect(btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted && !edtTen->text().isEmpty()) {
        m->setTenPhim(edtTen->text().toStdString());
        m->setTheLoai(edtTheLoai->text().toStdString());
        m->setThoiLuong(spnThoiLuong->value());
        m->setDoTuoi(spnDoTuoi->value());
        m->setMoTa(edtMoTa->text().toStdString());
        m->setNoiDung(edtNoiDung->text().toStdString());
        movieManager->updateMovie(id, *m);
        movieManager->save();
        loadData();
    }
}

void MovieManagementWidget::deleteMovie() {
    int row = table->currentRow();
    if(row >= 0) {
        std::string id = table->item(row, 0)->text().toStdString();
        if(QMessageBox::question(this, "Xac Nhan", "Ban co muon xoa (ngung chieu) phim nay?") == QMessageBox::Yes) {
            if(movieManager->deleteMovie(id)) {
                movieManager->save();
                loadData();
            }
        }
    }
}
