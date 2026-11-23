#include "MovieDetailWidget.h"
#include "../common/Theme.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QMessageBox>
#include <QFrame>

MovieDetailWidget::MovieDetailWidget(MovieManager* movieManager, QWidget *parent)
    : QWidget(parent), movieManager(movieManager), customerAge(0)
{
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(26, 22, 26, 20);
    outerLayout->setSpacing(16);

    QLabel *lblEyebrow = new QLabel("THÔNG TIN TÁC PHẨM", this);
    lblEyebrow->setStyleSheet(Theme::sectionEyebrowStyle());
    outerLayout->addWidget(lblEyebrow);

    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(26);

    // Left side: Soft Framed Poster
    QFrame *posterFrame = new QFrame(this);
    posterFrame->setFixedSize(316, 460);
    posterFrame->setStyleSheet(R"(
        QFrame {
            background-color: #22263C;
            border: 1.5px solid #3A4166;
            border-radius: 20px;
        }
    )");
    QVBoxLayout *posterLayout = new QVBoxLayout(posterFrame);
    posterLayout->setContentsMargins(9, 9, 9, 9);

    lblPoster = new QLabel(posterFrame);
    lblPoster->setFixedSize(298, 442);
    lblPoster->setAlignment(Qt::AlignCenter);
    lblPoster->setStyleSheet("background-color: #181B2C; border-radius: 14px; border: none;");
    posterLayout->addWidget(lblPoster);

    // Right side: Info Container
    QWidget *rightContainer = new QWidget(this);
    rightContainer->setStyleSheet("background: transparent;");
    QVBoxLayout *infoLayout = new QVBoxLayout(rightContainer);
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(14);

    lblTenPhim = new QLabel(rightContainer);
    lblTenPhim->setStyleSheet("color: #F3F4FA; font-size: 26px; font-weight: bold;");
    lblTenPhim->setWordWrap(true);

    QHBoxLayout *tagsLayout = new QHBoxLayout();
    tagsLayout->setSpacing(10);

    QString pillStyle = "color: #C5CAFF; font-size: 12px; font-weight: 600; background-color: #282D4F; padding: 6px 14px; border-radius: 10px; border: 1px solid #444C80;";

    lblTheLoai = new QLabel(rightContainer);
    lblTheLoai->setStyleSheet(pillStyle);

    lblThoiLuong = new QLabel(rightContainer);
    lblThoiLuong->setStyleSheet(pillStyle);

    lblDoTuoi = new QLabel(rightContainer);
    lblDoTuoi->setStyleSheet(Theme::ageBadgeStyle(18));

    tagsLayout->addWidget(lblTheLoai);
    tagsLayout->addWidget(lblThoiLuong);
    tagsLayout->addWidget(lblDoTuoi);
    tagsLayout->addStretch();

    // Box: MÔ TẢ
    QFrame *boxMoTa = new QFrame(rightContainer);
    boxMoTa->setObjectName("detailBox");
    boxMoTa->setStyleSheet(R"(
        QFrame#detailBox {
            background-color: #22263C;
            border: 1px solid #323859;
            border-radius: 16px;
        }
    )");
    QVBoxLayout *layoutMoTa = new QVBoxLayout(boxMoTa);
    layoutMoTa->setContentsMargins(18, 14, 18, 14);
    layoutMoTa->setSpacing(6);

    QLabel *lblTitleMoTa = new QLabel("MÔ TẢ NGẮN", boxMoTa);
    lblTitleMoTa->setStyleSheet("color: #FFD166; font-size: 11px; font-weight: bold; letter-spacing: 1.5px; border: none; background: transparent;");
    lblMoTa = new QLabel(boxMoTa);
    lblMoTa->setStyleSheet("color: #E2E5F5; font-size: 13px; border: none; background: transparent;");
    lblMoTa->setWordWrap(true);

    layoutMoTa->addWidget(lblTitleMoTa);
    layoutMoTa->addWidget(lblMoTa);

    // Box: NỘI DUNG PHIM
    QFrame *boxNoiDung = new QFrame(rightContainer);
    boxNoiDung->setObjectName("detailBoxContent");
    boxNoiDung->setStyleSheet(R"(
        QFrame#detailBoxContent {
            background-color: #22263C;
            border: 1px solid #323859;
            border-radius: 16px;
        }
    )");
    QVBoxLayout *layoutNoiDung = new QVBoxLayout(boxNoiDung);
    layoutNoiDung->setContentsMargins(18, 14, 18, 14);
    layoutNoiDung->setSpacing(6);

    QLabel *lblTitleNoiDung = new QLabel("NỘI DUNG PHIM", boxNoiDung);
    lblTitleNoiDung->setStyleSheet("color: #8C92FF; font-size: 11px; font-weight: bold; letter-spacing: 1.5px; border: none; background: transparent;");
    lblNoiDung = new QLabel(boxNoiDung);
    lblNoiDung->setStyleSheet("color: #C8CEE8; font-size: 13px; border: none; background: transparent;");
    lblNoiDung->setWordWrap(true);

    layoutNoiDung->addWidget(lblTitleNoiDung);
    layoutNoiDung->addWidget(lblNoiDung);

    // Age Warning Banner
    lblAgeWarning = new QLabel(rightContainer);
    lblAgeWarning->setWordWrap(true);
    lblAgeWarning->setStyleSheet("background-color: #3B2030; color: #FFA1AA; font-size: 13px; font-weight: 600; padding: 14px 16px; border-radius: 12px; border: 1px solid #8F3448;");
    lblAgeWarning->setVisible(false);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);

    btnBack = new QPushButton("Quay Lại", rightContainer);
    btnBack->setCursor(Qt::PointingHandCursor);
    btnBack->setStyleSheet(Theme::secondaryButtonStyle(44, 14));

    btnBook = new QPushButton("Đặt Vé Ngay", rightContainer);
    btnBook->setCursor(Qt::PointingHandCursor);
    btnBook->setStyleSheet(Theme::primaryButtonStyle(44, 14));

    btnLayout->addWidget(btnBack);
    btnLayout->addWidget(btnBook);
    btnLayout->addStretch();

    infoLayout->addWidget(lblTenPhim);
    infoLayout->addLayout(tagsLayout);
    infoLayout->addWidget(boxMoTa);
    infoLayout->addWidget(boxNoiDung);
    infoLayout->addWidget(lblAgeWarning);
    infoLayout->addSpacing(14);
    infoLayout->addLayout(btnLayout);
    infoLayout->addStretch();

    contentLayout->addWidget(posterFrame, 0, Qt::AlignTop);
    contentLayout->addWidget(rightContainer, 1);
    outerLayout->addLayout(contentLayout, 1);

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
        QString titleText = QString::fromStdString(m->getTenPhim());
        lblTenPhim->setText(titleText);
        lblTheLoai->setText("Thể loại: " + QString::fromStdString(m->getTheLoai()));
        lblThoiLuong->setText(QString("Thời lượng: %1 phút").arg(m->getThoiLuong()));

        lblDoTuoi->setText(Theme::ageBadgeText(m->getDoTuoi(), true));
        lblDoTuoi->setStyleSheet(Theme::ageBadgeStyle(m->getDoTuoi()));

        if (customerAge > 0 && m->getDoTuoi() > 0 && customerAge < m->getDoTuoi()) {
            lblAgeWarning->setText(QString("GIỚI HẠN ĐỘ TUỔI: Phim này được dán nhãn C%1 (dành cho khán giả từ đủ %1 tuổi trở lên).\n"
                                           "Tài khoản của bạn hiện %2 tuổi, chưa đủ điều kiện theo quy định của Cục Điện ảnh để đặt vé xem phim này.")
                                       .arg(m->getDoTuoi())
                                       .arg(customerAge));
            lblAgeWarning->setVisible(true);
            btnBook->setText(QString("Chưa Đủ Tuổi (C%1)").arg(m->getDoTuoi()));
            btnBook->setEnabled(false);
            btnBook->setStyleSheet(Theme::primaryButtonStyle(44, 14));
        } else {
            lblAgeWarning->setVisible(false);
            btnBook->setText("Đặt Vé Ngay");
            btnBook->setEnabled(true);
            btnBook->setStyleSheet(Theme::primaryButtonStyle(44, 14));
        }

        lblMoTa->setText(QString::fromStdString(m->getMoTa()));
        lblNoiDung->setText(QString::fromStdString(m->getNoiDung()));

        QPixmap pixmap = Theme::loadPosterPixmap(QString::fromStdString(m->getPoster()));
        if (!pixmap.isNull()) {
            lblPoster->setPixmap(Theme::roundedCoverPixmap(pixmap, 298, 442, 14));
        } else {
            lblPoster->setPixmap(QPixmap());
            lblPoster->setText("POSTER PHIM\n" + QString::fromStdString(m->getTenPhim()));
            lblPoster->setWordWrap(true);
        }
    } else {
        QMessageBox::critical(this, "Lỗi", "Không tìm thấy phim!");
        emit goBack();
    }
}
