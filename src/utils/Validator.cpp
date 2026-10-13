#include "utils/Validator.h"
#include <cctype>
#include <algorithm>
#include <sstream>

std::string Validator::trim(const std::string& s) {
    std::string result = s;
    result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), result.end());
    return result;
}

bool Validator::isNotEmpty(const std::string& s) {
    return !trim(s).empty();
}

bool Validator::isValidUsername(const std::string& username) {
    if (username.length() < 4) return false;
    for (char c : username) {
        if (!std::isalnum(c)) return false;
    }
    return true;
}

bool Validator::isValidPassword(const std::string& password) {
    return password.length() >= 6;
}

bool Validator::isValidPhone(const std::string& phone) {
    if (phone.length() < 10 || phone.length() > 11) return false;
    if (phone[0] != '0') return false;
    for (char c : phone) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

bool Validator::isValidAge(int age) {
    return age >= 1 && age <= 120;
}

bool Validator::isValidDate(const std::string& date) {
    if (date.length() != 10 || date[2] != '/' || date[5] != '/') return false;
    int day, month, year;
    char sep1, sep2;
    std::stringstream ss(date);
    ss >> day >> sep1 >> month >> sep2 >> year;
    if (ss.fail() || sep1 != '/' || sep2 != '/') return false;
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        daysInMonth[1] = 29;
    }
    return day >= 1 && day <= daysInMonth[month - 1];
}

bool Validator::isValidTime(const std::string& time) {
    if (time.length() != 5 || time[2] != ':') return false;
    int hour, minute;
    char sep;
    std::stringstream ss(time);
    ss >> hour >> sep >> minute;
    if (ss.fail() || sep != ':') return false;
    return hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59;
}

bool Validator::isValidPrice(double price) {
    return price >= 0;
}
