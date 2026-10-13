#ifndef TICKETMANAGER_H
#define TICKETMANAGER_H

#include "core/DynamicArray.h"
#include "models/Ticket.h"
#include <string>

class TicketManager {
private:
    DynamicArray<Ticket> tickets;
public:
    void load();
    void save();
    
    std::string createTicket(const std::string& maSuatChieu, const std::string& maKhachHang, const std::string& maNhanVien, int hang, int cot, double giaVe, const std::string& maCombo, const std::string& ngayMua);
    bool markUsed(const std::string& maVe);
    bool cancelTicket(const std::string& maVe);
    Ticket* findById(const std::string& maVe);
    
    DynamicArray<Ticket> getTicketsByCustomer(const std::string& maKH) const;
    DynamicArray<Ticket> getTicketsByShowtime(const std::string& maSC) const;
    DynamicArray<Ticket> getValidTickets() const;
    DynamicArray<Ticket>& getAllTickets();
};

#endif // TICKETMANAGER_H
