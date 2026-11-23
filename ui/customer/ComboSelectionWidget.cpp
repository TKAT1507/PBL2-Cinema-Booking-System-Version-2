#include "ComboSelectionWidget.h"
#include "../common/Theme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QSpinBox>
#include <QMessageBox>
#include <QLabel>

ComboSelectionWidget::ComboSelectionWidget(ComboManager* comboManager, QWidget *parent)
    : QWidget(parent), comboManager(comboManager)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(26, 22, 26, 20);
    layout->setSpacing(14);

    QVBoxLayout *titleCol = new QVBoxLayout();
    titleCol->setSpacing(2);
    QLabel *lblEyebrow = new QLabel("BƯỚC 3 / 4 • QUẦY BẮP NƯỚC", this);
    lblEyebrow->setStyleSheet(Theme::sectionEyebrowStyle());
    QLabel *lblTitle = new QLabel("CHỌN COMBO BẮP NƯỚC", this);
    lblTitle->setStyleSheet(Theme::sectionTitleStyle());
    titleCol->addWidget(lblEyebrow);
    titleCol->addWidget(lblTitle);
    layout->addLayout(titleCol);

    tableCombos = new QTableWidget(this);
    tableCombos->setColumnCount(5);
    tableCombos->setHorizontalHeaderLabels(QStringList() << "#" << "Tên Combo" << "Mô Tả" << "Giá Tiền" << "Số Lượng");
    tableCombos->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableCombos->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableCombos->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tableCombos->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tableCombos->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    tableCombos->setColumnWidth(4, 130);
    tableCombos->verticalHeader()->setDefaultSectionSize(52);
    tableCombos->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableCombos->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableCombos->setSelectionMode(QAbstractItemView::SingleSelection);
    tableCombos->setAlternatingRowColors(true);
    tableCombos->verticalHeader()->setVisible(false);
    layout->addWidget(tableCombos, 1);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(12);

    QPushButton *btnBack = new QPushButton("Quay Lại", this);
    btnBack->setCursor(Qt::PointingHandCursor);
    btnBack->setStyleSheet(Theme::secondaryButtonStyle(42, 14));

    btnSkip = new QPushButton("Bỏ Qua Combo", this);
    btnSkip->setCursor(Qt::PointingHandCursor);
    btnSkip->setStyleSheet(Theme::goldOutlineButtonStyle(42, 14));

    btnContinue = new QPushButton("Tiếp Tục Xác Nhận", this);
    btnContinue->setCursor(Qt::PointingHandCursor);
    btnContinue->setStyleSheet(Theme::primaryButtonStyle(42, 14));

    bottomLayout->addWidget(btnBack);
    bottomLayout->addWidget(btnSkip);
    bottomLayout->addStretch();
    bottomLayout->addWidget(btnContinue);

    layout->addLayout(bottomLayout);

    connect(btnContinue, &QPushButton::clicked, this, &ComboSelectionWidget::onContinue);
    connect(btnSkip, &QPushButton::clicked, this, &ComboSelectionWidget::onSkip);
    connect(btnBack, &QPushButton::clicked, this, &ComboSelectionWidget::goBack);
}

void ComboSelectionWidget::loadCombos() {
    auto combos = comboManager->getActiveCombos();
    tableCombos->setRowCount(combos.size());
    for (int i = 0; i < combos.size(); ++i) {
        auto c = combos[i];

        QTableWidgetItem* itemIndex = new QTableWidgetItem(QString::number(i + 1));
        itemIndex->setTextAlignment(Qt::AlignCenter);
        itemIndex->setFlags(itemIndex->flags() & ~Qt::ItemIsEditable);
        tableCombos->setItem(i, 0, itemIndex);

        QTableWidgetItem* itemTen = new QTableWidgetItem(QString::fromStdString(c.getTenCombo()));
        itemTen->setData(Qt::UserRole, QString::fromStdString(c.getMaCombo()));
        itemTen->setFlags(itemTen->flags() & ~Qt::ItemIsEditable);
        QFont boldFont = itemTen->font();
        boldFont.setBold(true);
        itemTen->setFont(boldFont);
        tableCombos->setItem(i, 1, itemTen);

        QTableWidgetItem* itemMoTa = new QTableWidgetItem(QString::fromStdString(c.getMoTa()));
        itemMoTa->setFlags(itemMoTa->flags() & ~Qt::ItemIsEditable);
        itemMoTa->setForeground(QBrush(QColor("#A3ABC9")));
        tableCombos->setItem(i, 2, itemMoTa);

        QTableWidgetItem* itemGia = new QTableWidgetItem(QString("%L1 VND").arg(c.getGia(), 0, 'f', 0));
        itemGia->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        itemGia->setFlags(itemGia->flags() & ~Qt::ItemIsEditable);
        itemGia->setFont(boldFont);
        itemGia->setForeground(QBrush(QColor("#FFD166")));
        tableCombos->setItem(i, 3, itemGia);

        QWidget* pWidget = new QWidget();
        pWidget->setStyleSheet("background: transparent;");
        QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
        pLayout->setContentsMargins(4, 2, 4, 2);
        pLayout->setAlignment(Qt::AlignCenter);

        QSpinBox *spinQty = new QSpinBox(pWidget);
        spinQty->setRange(0, 20);
        spinQty->setValue(0);
        spinQty->setAlignment(Qt::AlignCenter);
        spinQty->setFixedSize(94, 34);
        spinQty->setStyleSheet(R"(
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
        pLayout->addWidget(spinQty);
        tableCombos->setCellWidget(i, 4, pWidget);
    }
}

void ComboSelectionWidget::onContinue() {
    QStringList selected;
    for (int i = 0; i < tableCombos->rowCount(); ++i) {
        QWidget* pWidget = tableCombos->cellWidget(i, 4);
        QSpinBox *spin = pWidget ? pWidget->findChild<QSpinBox*>() : nullptr;
        if (spin && spin->value() > 0) {
            QString maCombo = tableCombos->item(i, 1)->data(Qt::UserRole).toString();
            selected << QString("%1,%2").arg(maCombo).arg(spin->value());
        }
    }

    emit combosSelected(selected);
}

void ComboSelectionWidget::onSkip() {
    emit combosSelected(QStringList());
}
