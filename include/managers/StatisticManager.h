#ifndef STATISTICMANAGER_H
#define STATISTICMANAGER_H

#include "core/DynamicArray.h"
#include "models/Ticket.h"
#include "models/Showtime.h"
#include <string>

class StatisticManager {
public:
    static double getRevenueByDate(const DynamicArray<Ticket>& tickets, const std::string& date);
    static double getRevenueByMovie(const DynamicArray<Ticket>& tickets, const DynamicArray<Showtime>& showtimes, const std::string& maPhim);
    static int countTicketsSold(const DynamicArray<Ticket>& tickets);
    static int countTicketsUsed(const DynamicArray<Ticket>& tickets);
    static double getTotalRevenue(const DynamicArray<Ticket>& tickets);
};

#endif // STATISTICMANAGER_H
