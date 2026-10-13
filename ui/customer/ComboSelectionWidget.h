#ifndef COMBOSELECTIONWIDGET_H
#define COMBOSELECTIONWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include "managers/ComboManager.h"
#include <QStringList>

class ComboSelectionWidget : public QWidget {
    Q_OBJECT

public:
    explicit ComboSelectionWidget(ComboManager* comboManager, QWidget *parent = nullptr);
    void loadCombos();

signals:
    // Format: "maCombo,quantity"
    void combosSelected(const QStringList& comboInfos);
    void goBack();

private slots:
    void onContinue();
    void onSkip();

private:
    ComboManager* comboManager;
    QTableWidget *tableCombos;
    QPushButton *btnContinue;
    QPushButton *btnSkip;
};

#endif // COMBOSELECTIONWIDGET_H
