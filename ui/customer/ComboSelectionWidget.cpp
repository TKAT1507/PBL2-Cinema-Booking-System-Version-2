#include "ComboSelectionWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QSpinBox>
#include <QMessageBox>

ComboSelectionWidget::ComboSelectionWidget(ComboManager* comboManager, QWidget *parent)
    : QWidget(parent), comboManager(comboManager)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    tableCombos = new QTableWidget(this);
    tableCombos->setColumnCount(4);
    tableCombos->setHorizontalHeaderLabels(QStringList() << "Ten Combo" << "Mo Ta" << "Gia" << "So Luong");
    tableCombos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableCombos->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableCombos->setAlternatingRowColors(true);

    btnContinue = new QPushButton("Tiep Tuc ->", this);
    btnContinue->setStyleSheet("background-color: #27ae60; color: white; padding: 10px 20px; font-weight: bold; border-radius: 6px;");
    
    btnSkip = new QPushButton("Bo Qua (Khong mua)", this);
    btnSkip->setStyleSheet("background-color: #0f3460; color: #ffd700; padding: 10px 16px; border-radius: 6px;");
    
    QPushButton *btnBack = new QPushButton("<- Quay Lai", this);
    btnBack->setStyleSheet("background-color: #16213e; color: #e0e0e0; border: 1px solid #0f3460; padding: 10px 16px; border-radius: 6px;");

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(btnSkip);
    bottomLayout->addStretch();
    bottomLayout->addWidget(btnBack);
    bottomLayout->addWidget(btnContinue);

    layout->addWidget(tableCombos);
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
        
        QTableWidgetItem* itemTen = new QTableWidgetItem(QString::fromStdString(c.getTenCombo()));
        itemTen->setData(Qt::UserRole, QString::fromStdString(c.getMaCombo()));
        itemTen->setFlags(itemTen->flags() & ~Qt::ItemIsEditable);
        
        QTableWidgetItem* itemMoTa = new QTableWidgetItem(QString::fromStdString(c.getMoTa()));
        itemMoTa->setFlags(itemMoTa->flags() & ~Qt::ItemIsEditable);
        
        QTableWidgetItem* itemGia = new QTableWidgetItem(QString::number(c.getGia(), 'f', 0) + " VND");
        itemGia->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        itemGia->setFlags(itemGia->flags() & ~Qt::ItemIsEditable);

        QSpinBox *spinQty = new QSpinBox(this);
        spinQty->setRange(0, 10);
        
        tableCombos->setItem(i, 0, itemTen);
        tableCombos->setItem(i, 1, itemMoTa);
        tableCombos->setItem(i, 2, itemGia);
        tableCombos->setCellWidget(i, 3, spinQty);
    }
}

void ComboSelectionWidget::onContinue() {
    QStringList selected;
    for (int i = 0; i < tableCombos->rowCount(); ++i) {
        QSpinBox *spin = qobject_cast<QSpinBox*>(tableCombos->cellWidget(i, 3));
        if (spin && spin->value() > 0) {
            QString maCombo = tableCombos->item(i, 0)->data(Qt::UserRole).toString();
            selected << QString("%1,%2").arg(maCombo).arg(spin->value());
        }
    }
    
    emit combosSelected(selected); // pass even if empty
}

void ComboSelectionWidget::onSkip() {
    emit combosSelected(QStringList());
}
