#include "managers/StatisticManager.h"

double StatisticManager::getRevenueByDate(const DynamicArray<Ticket>& tickets, const std::string& date) {
    double total = 0;
    for (int i = 0; i < tickets.size(); ++i) {
        if (tickets[i].getNgayMua() == date && tickets[i].getTrangThai() != "CANCELLED") {
            total += tickets[i].getGiaVe();
        }
    }
    return total;
}

double StatisticManager::getRevenueByMovie(const DynamicArray<Ticket>& tickets, const DynamicArray<Showtime>& showtimes, const std::string& maPhim) {
    DynamicArray<std::string> scIds;
    for (int i = 0; i < showtimes.size(); ++i) {
        if (showtimes[i].getMaPhim() == maPhim) {
            scIds.push_back(showtimes[i].getMaSuatChieu());
        }
    }

    double total = 0;
    for (int i = 0; i < tickets.size(); ++i) {
        if (tickets[i].getTrangThai() != "CANCELLED") {
            bool matches = false;
            for (int j = 0; j < scIds.size(); ++j) {
                if (tickets[i].getMaSuatChieu() == scIds[j]) {
                    matches = true;
                    break;
                }
            }
            if (matches) total += tickets[i].getGiaVe();
        }
    }
    return total;
}

int StatisticManager::countTicketsSold(const DynamicArray<Ticket>& tickets) {
    int count = 0;
    for (int i = 0; i < tickets.size(); ++i) {
        if (tickets[i].getTrangThai() != "CANCELLED") {
            count++;
        }
    }
    return count;
}

int StatisticManager::countTicketsUsed(const DynamicArray<Ticket>& tickets) {
    int count = 0;
    for (int i = 0; i < tickets.size(); ++i) {
        if (tickets[i].getTrangThai() == "USED") {
            count++;
        }
    }
    return count;
}

double StatisticManager::getTotalRevenue(const DynamicArray<Ticket>& tickets) {
    double total = 0;
    for (int i = 0; i < tickets.size(); ++i) {
        if (tickets[i].getTrangThai() != "CANCELLED") {
            total += tickets[i].getGiaVe();
        }
    }
    return total;
}
