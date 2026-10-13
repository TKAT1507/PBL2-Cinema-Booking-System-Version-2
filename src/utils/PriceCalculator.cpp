#include "utils/PriceCalculator.h"
#include <sstream>

const double PriceCalculator::BASE_PRICE = 65000;
const double PriceCalculator::VIP_SURCHARGE = 30000;
const double PriceCalculator::WEEKEND_SURCHARGE = 15000;
const double PriceCalculator::EARLY_BIRD_DISCOUNT = 10000;
const double PriceCalculator::LATE_NIGHT_SURCHARGE = 10000;

double PriceCalculator::calculateTicketPrice(int seatType, const std::string& ngayChieu, const std::string& gioChieu) {
    double price = BASE_PRICE;
    
    // seatType: 0=EMPTY, 1=BOOKED, 2=VIP_EMPTY, 3=VIP_BOOKED
    if (seatType == 2 || seatType == 3) {
        price += VIP_SURCHARGE;
    }
    
    if (isWeekend(ngayChieu)) {
        price += WEEKEND_SURCHARGE;
    }
    
    int hour = getHour(gioChieu);
    if (hour < 12) {
        price -= EARLY_BIRD_DISCOUNT;
    } else if (hour >= 22) {
        price += LATE_NIGHT_SURCHARGE;
    }
    
    return price;
}

double PriceCalculator::calculateTotal(const DynamicArray<double>& ticketPrices, const DynamicArray<double>& comboPrices) {
    double total = 0;
    for (int i = 0; i < ticketPrices.size(); ++i) {
        total += ticketPrices[i];
    }
    for (int i = 0; i < comboPrices.size(); ++i) {
        total += comboPrices[i];
    }
    return total;
}

bool PriceCalculator::isWeekend(const std::string& date) {
    if (date.length() < 10) return false;
    int day, month, year;
    char sep1, sep2;
    std::stringstream ss(date);
    ss >> day >> sep1 >> month >> sep2 >> year;
    
    if (month < 3) {
        month += 12;
        year -= 1;
    }
    int k = year % 100;
    int j = year / 100;
    int h = (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
    
    return h == 0 || h == 1; // 0=Saturday, 1=Sunday
}

int PriceCalculator::getHour(const std::string& time) {
    if (time.length() < 2) return 0;
    int hour = 0;
    std::stringstream ss(time.substr(0, 2));
    ss >> hour;
    return hour;
}
