#ifndef IDGENERATOR_H
#define IDGENERATOR_H

#include <string>
#include "core/DynamicArray.h"

class IDGenerator {
public:
    static std::string generateID(const std::string& prefix, const DynamicArray<std::string>& existingIDs);
    static int extractNumber(const std::string& id, const std::string& prefix);
};

#endif // IDGENERATOR_H
