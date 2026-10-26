#include "MovieDetailWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QMessageBox>
#include <QFileInfo>
#include <QCoreApplication>
#include <QFrame>
#include <QScrollArea>

MovieDetailWidget::MovieDetailWidget(MovieManager* movieManager, QWidget *parent)
    : QWidget(parent), movieManager(movieManager)
{
    QHBoxLayout *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(15, 15, 15, 15);
    rootLayout->setSpacing(20);

    // Left side: Large Poster
    lblPoster = new QLabel(this);
    lblPoster->setFixedSize(300, 440);
    lblPoster->setAlignment(Qt::AlignCenter);
    lblPoster->setStyleSheet(R"(
        QLabel {
            background-color: #121330;
            border: 2px solid #E6B042;
            border-radius: 10px;
        }
    )");
    lblPoster->setScaledContents(true);

    // Right side: Info Container
    QWidget *rightContainer = new QWidget(this);
    QVBoxLayout *infoLayout = new QVBoxLayout(rightContainer);
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(12);

    lblTenPhim = new QLabel(this);
    lblTenPhim->setStyleSheet("color: #F5C542; font-size: 22px; font-weight: bold;");
    lblTenPhim->setWordWrap(true);

    QHBoxLayout *tagsLayout = new QHBoxLayout();
    tagsLayout->setSpacing(15);

    lblTheLoai = new QLabel(this);
    lblTheLoai->setStyleSheet("color: #FFFFFF; font-size: 13px; font-weight: bold; background-color: #2C2E6A; padding: 4px 10px; border-radius: 4px; border: 1px solid #4A4D94;");

    lblThoiLuong = new QLabel(this);
    lblThoiLuong->setStyleSheet("color: #FFFFFF; font-size: 13px; font-weight: bold; background-color: #2C2E6A; padding: 4px 10px; border-radius: 4px; border: 1px solid #4A4D94;");

    lblDoTuoi = new QLabel(this);
    lblDoTuoi->setStyleSheet("color: #FFFFFF; font-size: 13px; font-weight: bold; background-color: #9E221E; padding: 4px 10px; border-radius: 4px;");

    tagsLayout->addWidget(lblTheLoai);
    tagsLayout->addWidget(lblThoiLuong);
    tagsLayout->addWidget(lblDoTuoi);
    tagsLayout->addStretch();

    // Box: MÔ TẢ
    QFrame *boxMoTa = new QFrame(this);
    boxMoTa->setStyleSheet(R"(
        QFrame {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2C2E6A, stop:1 #20224F);
            border: 1px solid #4A4D94;
            border-radius: 8px;
            padding: 10px;
        }
    )");
    QVBoxLayout *layoutMoTa = new QVBoxLayout(boxMoTa);
    layoutMoTa->setContentsMargins(10, 8, 10, 8);
    layoutMoTa->setSpacing(6);

    QLabel *lblTitleMoTa = new QLabel("MÔ TẢ", boxMoTa);
    lblTitleMoTa->setStyleSheet("color: #F5C542; font-size: 14px; font-weight: bold; border: none; background: transparent;");
    lblMoTa = new QLabel(boxMoTa);
    lblMoTa->setStyleSheet("color: #E6E6FA; font-size: 13px; border: none; background: transparent;");
    lblMoTa->setWordWrap(true);

    layoutMoTa->addWidget(lblTitleMoTa);
    layoutMoTa->addWidget(lblMoTa);

    // Box: NỘI DUNG PHIM
    QFrame *boxNoiDung = new QFrame(this);
    boxNoiDung->setStyleSheet(R"(
        QFrame {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2C2E6A, stop:1 #20224F);
            border: 1px solid #4A4D94;
            border-radius: 8px;
            padding: 10px;
        }
    )");
    QVBoxLayout *layoutNoiDung = new QVBoxLayout(boxNoiDung);
    layoutNoiDung->setContentsMargins(10, 8, 10, 8);
    layoutNoiDung->setSpacing(6);

    QLabel *lblTitleNoiDung = new QLabel("NỘI DUNG PHIM", boxNoiDung);
    lblTitleNoiDung->setStyleSheet("color: #F5C542; font-size: 14px; font-weight: bold; border: none; background: transparent;");
    lblNoiDung = new QLabel(boxNoiDung);
    lblNoiDung->setStyleSheet("color: #E6E6FA; font-size: 13px; border: none; background: transparent;");
    lblNoiDung->setWordWrap(true);

    layoutNoiDung->addWidget(lblTitleNoiDung);
    layoutNoiDung->addWidget(lblNoiDung);

    // Action buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(15);

    QString btnStyle = R"(
        QPushButton {
            background-color: #9E221E;
            color: #FFFFFF;
            font-size: 14px;
            font-weight: bold;
            border-radius: 6px;
            border: none;
            min-height: 40px;
            padding: 6px 20px;
        }
        QPushButton:hover {
            background-color: #B52A26;
            border: 1px solid #E6B042;
        }
    )";

    btnBook = new QPushButton("Đặt Vé", this);
    btnBook->setCursor(Qt::PointingHandCursor);
    btnBook->setStyleSheet(btnStyle);

    btnBack = new QPushButton("<- Quay Lại", this);
    btnBack->setCursor(Qt::PointingHandCursor);
    btnBack->setStyleSheet(btnStyle);

    btnLayout->addWidget(btnBook);
    btnLayout->addWidget(btnBack);
    btnLayout->addStretch();

    infoLayout->addWidget(lblTenPhim);
    infoLayout->addLayout(tagsLayout);
    infoLayout->addWidget(boxMoTa);
    infoLayout->addWidget(boxNoiDung);
    infoLayout->addStretch();
    infoLayout->addLayout(btnLayout);

    rootLayout->addWidget(lblPoster, 0, Qt::AlignTop);
    rootLayout->addWidget(rightContainer, 1);

    connect(btnBook, &QPushButton::clicked, this, [this]() {
        if (!currentMaPhim.isEmpty()) emit bookMovie(currentMaPhim);
    });
    connect(btnBack, &QPushButton::clicked, this, &MovieDetailWidget::goBack);
}

void MovieDetailWidget::showMovie(const QString& maPhim) {
    auto m = movieManager->findById(maPhim.toStdString());
    if (m) {
        currentMaPhim = maPhim;
        QString titleText = QString::fromStdString(m->getTenPhim()).toUpper();
        if (m->getDoTuoi() > 0) {
            titleText += QString(" (T%1)").arg(m->getDoTuoi());
        }
        lblTenPhim->setText(titleText);
        lblTheLoai->setText("Thể loại: " + QString::fromStdString(m->getTheLoai()));
        lblThoiLuong->setText(QString("Thời lượng: %1 phút").arg(m->getThoiLuong()));
        lblDoTuoi->setText(QString("Độ tuổi: %1+").arg(m->getDoTuoi()));
        lblMoTa->setText(QString::fromStdString(m->getMoTa()));
        lblNoiDung->setText(QString::fromStdString(m->getNoiDung()));
        
        QString posterPath = QString::fromStdString(m->getPoster());
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

        if (!pixmap.isNull()) {
            lblPoster->setPixmap(pixmap.scaled(300, 440, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            lblPoster->setText("POSTER PHIM\n" + QString::fromStdString(m->getTenPhim()));
        }
    } else {
        QMessageBox::critical(this, "Lỗi", "Không tìm thấy phim!");
        emit goBack();
    }
}
