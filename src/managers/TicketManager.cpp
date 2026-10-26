#include "managers/TicketManager.h"
#include "managers/ShowtimeManager.h"
#include "utils/FileManager.h"
#include "utils/IDGenerator.h"

void TicketManager::load() {
    tickets.clear();
    DynamicArray<std::string> lines = FileManager::readLines(FileManager::getDataPath("tickets.txt"));
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].empty()) continue;
        tickets.push_back(Ticket::fromString(lines[i]));
    }
}

void TicketManager::save() {
    DynamicArray<std::string> lines;
    for (int i = 0; i < tickets.size(); ++i) {
        lines.push_back(tickets[i].toString());
    }
    FileManager::writeLines(FileManager::getDataPath("tickets.txt"), lines);
}

std::string TicketManager::createTicket(const std::string& maSuatChieu, const std::string& maKhachHang, const std::string& maNhanVien, int hang, int cot, double giaVe, const std::string& maCombo, const std::string& ngayMua, const std::string& maCumVe, const std::string& trangThai) {
    DynamicArray<std::string> existingIDs;
    for (int i = 0; i < tickets.size(); ++i) {
        existingIDs.push_back(tickets[i].getMaVe());
    }
    
    Ticket t;
    std::string maVe = IDGenerator::generateID("TK", existingIDs);
    t.setMaVe(maVe);
    t.setMaSuatChieu(maSuatChieu);
    t.setMaKhachHang(maKhachHang);
    t.setMaNhanVien(maNhanVien);
    t.setHang(hang);
    t.setCot(cot);
    t.setGiaVe(giaVe);
    t.setMaCombo(maCombo);
    t.setNgayMua(ngayMua);
    t.setTrangThai(trangThai);
    t.setMaCumVe(maCumVe);
    
    tickets.push_back(t);
    save();
    return maVe;
}

std::string TicketManager::generateOrderId() {
    DynamicArray<std::string> existingOrderIDs;
    for (int i = 0; i < tickets.size(); ++i) {
        std::string oid = tickets[i].getMaCumVe();
        if (!oid.empty() && !existingOrderIDs.contains(oid)) {
            existingOrderIDs.push_back(oid);
        }
    }
    return IDGenerator::generateID("ORD", existingOrderIDs);
}

bool TicketManager::approveOrder(const std::string& maCumVe) {
    bool found = false;
    for (int i = 0; i < tickets.size(); ++i) {
        if (tickets[i].getMaCumVe() == maCumVe && tickets[i].getTrangThai() == "PENDING") {
            tickets[i].setTrangThai("VALID");
            found = true;
        }
    }
    if (found) save();
    return found;
}

bool TicketManager::cancelOrder(const std::string& maCumVe, ShowtimeManager* showtimeMgr) {
    bool found = false;
    for (int i = 0; i < tickets.size(); ++i) {
        if (tickets[i].getMaCumVe() == maCumVe && tickets[i].getTrangThai() == "PENDING") {
            tickets[i].cancel();
            // Release the seat back on the seatmap
            if (showtimeMgr) {
                SeatMap* sm = showtimeMgr->findSeatMap(tickets[i].getMaSuatChieu());
                if (sm) {
                    sm->cancelSeat(tickets[i].getHang(), tickets[i].getCot());
                }
            }
            found = true;
        }
    }
    if (found) {
        save();
        if (showtimeMgr) showtimeMgr->saveSeatMaps();
    }
    return found;
}

DynamicArray<Ticket> TicketManager::getPendingOrders() const {
    // Return one ticket per unique maCumVe that has PENDING status (as representative)
    DynamicArray<Ticket> res;
    DynamicArray<std::string> seenOrders;
    for (int i = 0; i < tickets.size(); ++i) {
        if (tickets[i].getTrangThai() == "PENDING" && !tickets[i].getMaCumVe().empty()) {
            if (!seenOrders.contains(tickets[i].getMaCumVe())) {
                seenOrders.push_back(tickets[i].getMaCumVe());
                res.push_back(tickets[i]);
            }
        }
    }
    return res;
}

DynamicArray<Ticket> TicketManager::getTicketsByOrder(const std::string& maCumVe) const {
    DynamicArray<Ticket> res;
    for (int i = 0; i < tickets.size(); ++i) {
        if (tickets[i].getMaCumVe() == maCumVe) {
            res.push_back(tickets[i]);
        }
    }
    return res;
}

bool TicketManager::markUsed(const std::string& maVe) {
    for (int i = 0; i < tickets.size(); ++i) {
        if (tickets[i].getMaVe() == maVe) {
            tickets[i].markUsed();
            save();
            return true;
        }
    }
    return false;
}

bool TicketManager::cancelTicket(const std::string& maVe) {
    for (int i = 0; i < tickets.size(); ++i) {
        if (tickets[i].getMaVe() == maVe) {
            tickets[i].cancel();
            save();
            return true;
        }
    }
    return false;
}

Ticket* TicketManager::findById(const std::string& maVe) {
    for (int i = 0; i < tickets.size(); ++i) {
        if (tickets[i].getMaVe() == maVe) return &tickets[i];
    }
    return nullptr;
}

DynamicArray<Ticket> TicketManager::getTicketsByCustomer(const std::string& maKH) const {
    DynamicArray<Ticket> res;
    for (int i = 0; i < tickets.size(); ++i) {
        if (tickets[i].getMaKhachHang() == maKH) res.push_back(tickets[i]);
    }
    return res;
}

DynamicArray<Ticket> TicketManager::getTicketsByShowtime(const std::string& maSC) const {
    DynamicArray<Ticket> res;
    for (int i = 0; i < tickets.size(); ++i) {
        if (tickets[i].getMaSuatChieu() == maSC) res.push_back(tickets[i]);
    }
    return res;
}

DynamicArray<Ticket> TicketManager::getValidTickets() const {
    DynamicArray<Ticket> res;
    for (int i = 0; i < tickets.size(); ++i) {
        if (tickets[i].getTrangThai() == "VALID") res.push_back(tickets[i]);
    }
    return res;
}

DynamicArray<Ticket>& TicketManager::getAllTickets() {
    return tickets;
}
