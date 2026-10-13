#pragma once
#include <QWidget>
#include <QStackedWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpinBox>
#include <QLabel>
#include <string>
#include "core/DynamicArray.h"
#include "models/SeatMap.h"

class MovieManager;
class RoomManager;
class ShowtimeManager;
class TicketManager;
class ComboManager;

class SaleWidget : public QWidget {
    Q_OBJECT
public:
    explicit SaleWidget(const std::string& maNhanVien, MovieManager* movieMgr, RoomManager* roomMgr,
                        ShowtimeManager* showtimeMgr, TicketManager* ticketMgr,
                        ComboManager* comboMgr, QWidget* parent = nullptr);

private:
    std::string maNhanVien;
    MovieManager* movieManager;
    RoomManager* roomManager;
    ShowtimeManager* showtimeManager;
    TicketManager* ticketManager;
    ComboManager* comboManager;

    QStackedWidget* stepWidget;
    
    // Step 1
    QTableWidget* movieTable;
    void setupStep1(QWidget* page);
    void loadMovies();
    std::string selectedMovieId;
    
    // Step 2
    QTableWidget* showtimeTable;
    void setupStep2(QWidget* page);
    void loadShowtimes();
    std::string selectedShowtimeId;
    
    // Step 3
    QWidget* seatGridWidget;
    void setupStep3(QWidget* page);
    void loadSeats();
    DynamicArray<std::pair<int, int>> selectedSeats;
    
    // Step 4
    QTableWidget* comboTable;
    void setupStep4(QWidget* page);
    void loadCombos();
    DynamicArray<std::pair<std::string, int>> selectedCombos;
    
    // Step 5
    QLabel* previewLabel;
    void setupStep5(QWidget* page);
    void generatePreview();
    
    // Step 6
    QLabel* confirmLabel;
    void setupStep6(QWidget* page);
    
    void createTickets();
};
