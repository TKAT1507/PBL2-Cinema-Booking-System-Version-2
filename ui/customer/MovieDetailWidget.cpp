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
    : QWidget(parent), movieManager(movieManager), customerAge(0)
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
            border: 2px solid #EAB02F;
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
    lblTenPhim->setStyleSheet("color: #EAB02F; font-size: 22px; font-weight: bold;");
    lblTenPhim->setWordWrap(true);

    QHBoxLayout *tagsLayout = new QHBoxLayout();
    tagsLayout->setSpacing(15);

    lblTheLoai = new QLabel(this);
    lblTheLoai->setStyleSheet("color: #FFFFFF; font-size: 13px; font-weight: bold; background-color: #26295C; padding: 4px 10px; border-radius: 4px; border: 1px solid #434994;");

    lblThoiLuong = new QLabel(this);
    lblThoiLuong->setStyleSheet("color: #FFFFFF; font-size: 13px; font-weight: bold; background-color: #26295C; padding: 4px 10px; border-radius: 4px; border: 1px solid #434994;");

    lblDoTuoi = new QLabel(this);
    lblDoTuoi->setStyleSheet("color: #FFFFFF; font-size: 13px; font-weight: bold; background-color: #A52C20; padding: 4px 10px; border-radius: 4px;");

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
    lblTitleMoTa->setStyleSheet("color: #EAB02F; font-size: 14px; font-weight: bold; border: none; background: transparent;");
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
    lblTitleNoiDung->setStyleSheet("color: #EAB02F; font-size: 14px; font-weight: bold; border: none; background: transparent;");
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
            background-color: #A52C20;
            color: #FFFFFF;
            font-size: 14px;
            font-weight: bold;
            border-radius: 6px;
            border: none;
            min-height: 40px;
            padding: 6px 20px;
        }
        QPushButton:hover {
            background-color: #BD362A;
            border: 1px solid #EAB02F;
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

    // Age Warning Banner
    lblAgeWarning = new QLabel(this);
    lblAgeWarning->setWordWrap(true);
    lblAgeWarning->setStyleSheet("background-color: #A52C20; color: #FFFFFF; font-size: 13px; font-weight: bold; padding: 10px 14px; border-radius: 6px; border: 1px solid #FF8080;");
    lblAgeWarning->setVisible(false);

    infoLayout->addWidget(lblTenPhim);
    infoLayout->addLayout(tagsLayout);
    infoLayout->addWidget(boxMoTa);
    infoLayout->addWidget(boxNoiDung);
    infoLayout->addWidget(lblAgeWarning);
    infoLayout->addStretch();
    infoLayout->addLayout(btnLayout);

    rootLayout->addWidget(lblPoster, 0, Qt::AlignTop);
    rootLayout->addWidget(rightContainer, 1);

    connect(btnBook, &QPushButton::clicked, this, [this]() {
        if (!currentMaPhim.isEmpty()) emit bookMovie(currentMaPhim);
    });
    connect(btnBack, &QPushButton::clicked, this, &MovieDetailWidget::goBack);
}

void MovieDetailWidget::setCustomerAge(int age) {
    customerAge = age;
}

void MovieDetailWidget::showMovie(const QString& maPhim) {
    auto m = movieManager->findById(maPhim.toStdString());
    if (m) {
        currentMaPhim = maPhim;
        QString titleText = QString::fromStdString(m->getTenPhim()).toUpper();
        lblTenPhim->setText(titleText);
        lblTheLoai->setText("Thể loại: " + QString::fromStdString(m->getTheLoai()));
        lblThoiLuong->setText(QString("Thời lượng: %1 phút").arg(m->getThoiLuong()));
        
        // Age Tag Formatting
        if (m->getDoTuoi() == 0) {
            lblDoTuoi->setText("P - Mọi lứa tuổi");
            lblDoTuoi->setStyleSheet("color: #FFFFFF; font-size: 13px; font-weight: bold; background-color: #28A745; padding: 4px 10px; border-radius: 4px;");
        } else if (m->getDoTuoi() == 13) {
            lblDoTuoi->setText("C13 - Từ 13 tuổi");
            lblDoTuoi->setStyleSheet("color: #1A1A1A; font-size: 13px; font-weight: bold; background-color: #EAB02F; padding: 4px 10px; border-radius: 4px;");
        } else if (m->getDoTuoi() == 16) {
            lblDoTuoi->setText("C16 - Từ 16 tuổi");
            lblDoTuoi->setStyleSheet("color: #FFFFFF; font-size: 13px; font-weight: bold; background-color: #E06D14; padding: 4px 10px; border-radius: 4px;");
        } else {
            lblDoTuoi->setText(QString("C%1 - Từ %1 tuổi").arg(m->getDoTuoi()));
            lblDoTuoi->setStyleSheet("color: #FFFFFF; font-size: 13px; font-weight: bold; background-color: #A52C20; padding: 4px 10px; border-radius: 4px;");
        }

        // Age check against current Customer
        if (customerAge > 0 && m->getDoTuoi() > 0 && customerAge < m->getDoTuoi()) {
            lblAgeWarning->setText(QString("⛔ GIỚI HẠN ĐỘ TUỔI: Phim này được dán nhãn C%1 (Khán giả từ đủ %1 tuổi trở lên).\n"
                                           "Tài khoản của bạn hiện %2 tuổi, chưa đủ điều kiện theo quy định của Cục Điện ảnh để đặt vé xem phim này.")
                                       .arg(m->getDoTuoi())
                                       .arg(customerAge));
            lblAgeWarning->setVisible(true);
            btnBook->setText(QString("Chưa Đủ Tuổi (C%1)").arg(m->getDoTuoi()));
            btnBook->setEnabled(false);
            btnBook->setStyleSheet("background-color: #555566; color: #BBBBBB; font-size: 14px; font-weight: bold; border-radius: 6px; min-height: 40px; padding: 6px 25px; border: none;");
        } else {
            lblAgeWarning->setVisible(false);
            btnBook->setText("Đặt Vé Ngay");
            btnBook->setEnabled(true);
            btnBook->setStyleSheet("background-color: #A52C20; color: #FFFFFF; font-size: 14px; font-weight: bold; border-radius: 6px; min-height: 40px; padding: 6px 25px; border: none;");
        }

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
