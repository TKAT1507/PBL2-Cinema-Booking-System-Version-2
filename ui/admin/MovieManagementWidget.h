#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>

class MovieManager;

class MovieManagementWidget : public QWidget {
    Q_OBJECT
public:
    explicit MovieManagementWidget(MovieManager* movieMgr, QWidget* parent = nullptr);

private:
    MovieManager* movieManager;
    QTableWidget* table;
    QPushButton* btnAdd;
    QPushButton* btnEdit;
    QPushButton* btnDelete;

    void loadData();
    void addMovie();
    void editMovie();
    void deleteMovie();
};
