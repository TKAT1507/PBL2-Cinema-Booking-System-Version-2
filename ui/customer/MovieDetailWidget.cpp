#include "MovieDetailWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QMessageBox>

MovieDetailWidget::MovieDetailWidget(MovieManager* movieManager, QWidget *parent)
    : QWidget(parent), movieManager(movieManager)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // Left side: Poster
    lblPoster = new QLabel(this);
    lblPoster->setFixedSize(300, 450);
    lblPoster->setScaledContents(true);
    lblPoster->setStyleSheet("border: 1px solid black;");

    // Right side: Info
    QVBoxLayout *infoLayout = new QVBoxLayout();
    
    lblTenPhim = new QLabel(this);
    QFont f = lblTenPhim->font();
    f.setPointSize(20);
    f.setBold(true);
    lblTenPhim->setFont(f);

    lblTheLoai = new QLabel(this);
    lblThoiLuong = new QLabel(this);
    lblDoTuoi = new QLabel(this);
    
    lblMoTa = new QLabel(this);
    lblMoTa->setWordWrap(true);
    
    lblNoiDung = new QLabel(this);
    lblNoiDung->setWordWrap(true);

    btnBook = new QPushButton("Dat Ve", this);
    btnBack = new QPushButton("Quay Lai", this);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnBook);
    btnLayout->addWidget(btnBack);
    btnLayout->addStretch();

    infoLayout->addWidget(lblTenPhim);
    infoLayout->addWidget(lblTheLoai);
    infoLayout->addWidget(lblThoiLuong);
    infoLayout->addWidget(lblDoTuoi);
    infoLayout->addWidget(lblMoTa);
    infoLayout->addWidget(lblNoiDung);
    infoLayout->addStretch();
    infoLayout->addLayout(btnLayout);

    mainLayout->addWidget(lblPoster);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(infoLayout);

    connect(btnBook, &QPushButton::clicked, this, [this]() {
        if (!currentMaPhim.isEmpty()) emit bookMovie(currentMaPhim);
    });
    connect(btnBack, &QPushButton::clicked, this, &MovieDetailWidget::goBack);
}

void MovieDetailWidget::showMovie(const QString& maPhim) {
    auto m = movieManager->findById(maPhim.toStdString());
    if (m) {
        currentMaPhim = maPhim;
        lblTenPhim->setText(QString::fromStdString(m->getTenPhim()));
        lblTheLoai->setText("The loai: " + QString::fromStdString(m->getTheLoai()));
        lblThoiLuong->setText("Thoi luong: " + QString::number(m->getThoiLuong()) + " phut");
        lblDoTuoi->setText("Do tuoi: " + QString::number(m->getDoTuoi()) + "+");
        lblMoTa->setText("Mo ta: " + QString::fromStdString(m->getMoTa()));
        lblNoiDung->setText("Noi dung: " + QString::fromStdString(m->getNoiDung()));
        
        QPixmap pixmap(QString::fromStdString(m->getPoster()));
        if (!pixmap.isNull()) {
            lblPoster->setPixmap(pixmap);
        } else {
            lblPoster->setText("No Poster");
        }
    } else {
        QMessageBox::critical(this, "Loi", "Khong tim thay phim!");
        emit goBack();
    }
}
