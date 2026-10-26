#ifndef TICKETAPPROVALWIDGET_H
#define TICKETAPPROVALWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include "managers/TicketManager.h"
#include "managers/ShowtimeManager.h"
#include "managers/MovieManager.h"

class TicketApprovalWidget : public QWidget {
    Q_OBJECT

public:
    explicit TicketApprovalWidget(TicketManager* ticketMgr, ShowtimeManager* showtimeMgr, 
                                 MovieManager* movieMgr, QWidget* parent = nullptr);

public slots:
    void loadPendingOrders();

private slots:
    void onApproveClicked();
    void onCancelClicked();
    void onPrintClicked();

private:
    TicketManager* ticketManager;
    ShowtimeManager* showtimeManager;
    MovieManager* movieManager;

    QLineEdit *txtSearch;
    QTableWidget *tableOrders;
    QPushButton *btnApprove;
    QPushButton *btnCancel;
    QPushButton *btnPrint;
    QPushButton *btnRefresh;
    QLabel *lblStatusCount;

    QString getSelectedOrderId() const;
};

#endif // TICKETAPPROVALWIDGET_H
