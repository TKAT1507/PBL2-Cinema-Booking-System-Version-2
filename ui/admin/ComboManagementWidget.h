#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>

class ComboManager;

class ComboManagementWidget : public QWidget {
    Q_OBJECT
public:
    explicit ComboManagementWidget(ComboManager* comboMgr, QWidget* parent = nullptr);

private:
    ComboManager* comboManager;
    QTableWidget* table;
    QPushButton* btnAdd;
    QPushButton* btnEdit;
    QPushButton* btnDelete;

    void loadData();
    void addCombo();
    void editCombo();
    void deleteCombo();
};
