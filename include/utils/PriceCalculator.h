#ifndef PRICECALCULATOR_H
#define PRICECALCULATOR_H

#include <string>
#include "core/DynamicArray.h"

class PriceCalculator {
public:
    static const double BASE_PRICE;
    static const double VIP_SURCHARGE;
    static const double WEEKEND_SURCHARGE;
    static const double EARLY_BIRD_DISCOUNT;
    static const double LATE_NIGHT_SURCHARGE;

    static double calculateTicketPrice(int seatType, const std::string& ngayChieu, const std::string& gioChieu);
    static double calculateTotal(const DynamicArray<double>& ticketPrices, const DynamicArray<double>& comboPrices);
    static bool isWeekend(const std::string& date);
    static int getHour(const std::string& time);
};

#endif // PRICECALCULATOR_H
