#ifndef TICKETMANAGER_H
#define TICKETMANAGER_H

#include "core/DynamicArray.h"
#include "models/Ticket.h"
#include <string>

class ShowtimeManager; // Forward declaration for cancelOrder

class TicketManager {
private:
    DynamicArray<Ticket> tickets;
public:
    void load();
    void save();
    
    // Create ticket with batch order ID. status defaults to PENDING for customer, VALID for cashier
    std::string createTicket(const std::string& maSuatChieu, const std::string& maKhachHang, const std::string& maNhanVien, int hang, int cot, double giaVe, const std::string& maCombo, const std::string& ngayMua, const std::string& maCumVe = "", const std::string& trangThai = "PENDING");
    
    bool markUsed(const std::string& maVe);
    bool cancelTicket(const std::string& maVe);
    Ticket* findById(const std::string& maVe);
    
    // Batch/Order operations
    std::string generateOrderId();
    bool approveOrder(const std::string& maCumVe);
    bool cancelOrder(const std::string& maCumVe, ShowtimeManager* showtimeMgr);
    DynamicArray<Ticket> getPendingOrders() const; // unique maCumVe with PENDING status
    DynamicArray<Ticket> getTicketsByOrder(const std::string& maCumVe) const;
    
    DynamicArray<Ticket> getTicketsByCustomer(const std::string& maKH) const;
    DynamicArray<Ticket> getTicketsByShowtime(const std::string& maSC) const;
    DynamicArray<Ticket> getValidTickets() const;
    DynamicArray<Ticket>& getAllTickets();
};

#endif // TICKETMANAGER_H
