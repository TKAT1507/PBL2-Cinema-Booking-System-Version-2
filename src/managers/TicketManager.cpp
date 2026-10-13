#include "managers/TicketManager.h"
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

std::string TicketManager::createTicket(const std::string& maSuatChieu, const std::string& maKhachHang, const std::string& maNhanVien, int hang, int cot, double giaVe, const std::string& maCombo, const std::string& ngayMua) {
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
    t.setTrangThai("VALID");
    
    tickets.push_back(t);
    save();
    return maVe;
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
