#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

class Validator {
public:
    static bool isValidUsername(const std::string& username);
    static bool isValidPassword(const std::string& password);
    static bool isValidPhone(const std::string& phone);
    static bool isValidAge(int age);
    static bool isValidDate(const std::string& date);
    static bool isValidTime(const std::string& time);
    static bool isValidPrice(double price);
    static bool isNotEmpty(const std::string& s);
    static std::string trim(const std::string& s);
};

#endif // VALIDATOR_H
