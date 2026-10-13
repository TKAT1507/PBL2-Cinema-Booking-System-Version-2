#include "utils/IDGenerator.h"
#include <sstream>
#include <iomanip>

std::string IDGenerator::generateID(const std::string& prefix, const DynamicArray<std::string>& existingIDs) {
    int maxNum = 0;
    for (int i = 0; i < existingIDs.size(); ++i) {
        int num = extractNumber(existingIDs[i], prefix);
        if (num > maxNum) {
            maxNum = num;
        }
    }
    maxNum++;
    std::stringstream ss;
    ss << prefix << std::setw(3) << std::setfill('0') << maxNum;
    return ss.str();
}

int IDGenerator::extractNumber(const std::string& id, const std::string& prefix) {
    if (id.length() > prefix.length() && id.substr(0, prefix.length()) == prefix) {
        std::string numStr = id.substr(prefix.length());
        int num = 0;
        std::stringstream ss(numStr);
        ss >> num;
        return num;
    }
    return 0;
}
