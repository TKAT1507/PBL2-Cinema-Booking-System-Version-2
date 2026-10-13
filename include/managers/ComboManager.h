#ifndef COMBOMANAGER_H
#define COMBOMANAGER_H

#include "core/HashTable.h"
#include "core/DynamicArray.h"
#include "models/Combo.h"
#include <string>

class ComboManager {
private:
    HashTable<std::string, Combo> combos;
public:
    void load();
    void save();
    bool addCombo(const Combo& combo);
    bool updateCombo(const std::string& maCombo, const Combo& updated);
    bool deactivateCombo(const std::string& maCombo);
    Combo* findById(const std::string& maCombo);
    DynamicArray<Combo> getActiveCombos() const;
    DynamicArray<Combo> getAllCombos() const;
};

#endif // COMBOMANAGER_H

