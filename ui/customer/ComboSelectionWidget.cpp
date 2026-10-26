#include "ComboSelectionWidget.h"
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
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(15);

    QLabel *lblTitle = new QLabel("CHỌN COMBO BẮP NƯỚC", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("color: #F5C542; font-size: 20px; font-weight: bold;");
    layout->addWidget(lblTitle);

    tableCombos = new QTableWidget(this);
    tableCombos->setColumnCount(5);
    tableCombos->setHorizontalHeaderLabels(QStringList() << "#" << "Tên Combo" << "Mô Tả" << "Giá Tiền" << "Số Lượng");
    tableCombos->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableCombos->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableCombos->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tableCombos->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tableCombos->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    tableCombos->setColumnWidth(4, 130);
    tableCombos->verticalHeader()->setDefaultSectionSize(48);
    tableCombos->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableCombos->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableCombos->setSelectionMode(QAbstractItemView::SingleSelection);
    tableCombos->setAlternatingRowColors(true);
    tableCombos->verticalHeader()->setVisible(false);
    layout->addWidget(tableCombos);

    // Bottom Action Buttons
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(15);

    btnSkip = new QPushButton("Bỏ Qua (Không mua)", this);
    btnSkip->setCursor(Qt::PointingHandCursor);
    btnSkip->setStyleSheet(R"(
        QPushButton {
            background-color: #E6B042;
            color: #1A1B41;
            font-size: 14px;
            font-weight: bold;
            border: 1px solid #C49129;
            border-radius: 6px;
            min-height: 38px;
            padding: 6px 18px;
        }
        QPushButton:hover {
            background-color: #EDB952;
            border: 1px solid #FFFFFF;
        }
    )");

    QPushButton *btnBack = new QPushButton("<- Quay Lại", this);
    btnBack->setCursor(Qt::PointingHandCursor);
    btnBack->setStyleSheet(R"(
        QPushButton {
            background-color: #9E221E;
            color: #FFFFFF;
            font-size: 14px;
            font-weight: bold;
            border-radius: 6px;
            border: none;
            min-height: 38px;
            padding: 6px 18px;
        }
        QPushButton:hover {
            background-color: #B52A26;
        }
    )");

    btnContinue = new QPushButton("Tiếp Tục ->", this);
    btnContinue->setCursor(Qt::PointingHandCursor);
    btnContinue->setStyleSheet(R"(
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
    )");

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
        itemTen->setFont(QFont("Segoe UI", 10, QFont::Bold));
        tableCombos->setItem(i, 1, itemTen);
        
        QTableWidgetItem* itemMoTa = new QTableWidgetItem(QString::fromStdString(c.getMoTa()));
        itemMoTa->setFlags(itemMoTa->flags() & ~Qt::ItemIsEditable);
        tableCombos->setItem(i, 2, itemMoTa);
        
        QTableWidgetItem* itemGia = new QTableWidgetItem(QString("%L1 VND").arg(c.getGia(), 0, 'f', 0));
        itemGia->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        itemGia->setFlags(itemGia->flags() & ~Qt::ItemIsEditable);
        itemGia->setForeground(QBrush(QColor("#F5C542")));
        tableCombos->setItem(i, 3, itemGia);

        QWidget* pWidget = new QWidget();
        QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
        pLayout->setContentsMargins(4, 2, 4, 2);
        pLayout->setAlignment(Qt::AlignCenter);

        QSpinBox *spinQty = new QSpinBox(pWidget);
        spinQty->setRange(0, 20);
        spinQty->setValue(0);
        spinQty->setAlignment(Qt::AlignCenter);
        spinQty->setFixedSize(85, 30);
        spinQty->setStyleSheet(R"(
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
