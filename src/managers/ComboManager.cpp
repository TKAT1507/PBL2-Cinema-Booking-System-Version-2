#include "managers/ComboManager.h"
#include "utils/FileManager.h"
#include "utils/IDGenerator.h"

void ComboManager::load() {
    combos.clear();
    DynamicArray<std::string> lines = FileManager::readLines(FileManager::getDataPath("combos.txt"));
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].empty()) continue;
        Combo c = Combo::fromString(lines[i]);
        combos.put(c.getMaCombo(), c);
    }
}

void ComboManager::save() {
    DynamicArray<std::string> lines;
    DynamicArray<Combo> all = combos.values();
    for (int i = 0; i < all.size(); ++i) {
        lines.push_back(all[i].toString());
    }
    FileManager::writeLines(FileManager::getDataPath("combos.txt"), lines);
}

bool ComboManager::addCombo(const Combo& combo) {
    DynamicArray<std::string> existingIDs = combos.keys();
    Combo c = combo;
    if (c.getMaCombo().empty() || combos.containsKey(c.getMaCombo())) {
        c.setMaCombo(IDGenerator::generateID("CB", existingIDs));
    }
    combos.put(c.getMaCombo(), c);
    save();
    return true;
}

bool ComboManager::updateCombo(const std::string& maCombo, const Combo& updated) {
    Combo* existing = combos.get(maCombo);
    if (existing != nullptr) {
        *existing = updated;
        existing->setMaCombo(maCombo);
        save();
        return true;
    }
    return false;
}

bool ComboManager::deactivateCombo(const std::string& maCombo) {
    Combo* existing = combos.get(maCombo);
    if (existing != nullptr) {
        existing->setTrangThai(false);
        save();
        return true;
    }
    return false;
}

Combo* ComboManager::findById(const std::string& maCombo) {
    return combos.get(maCombo);
}

DynamicArray<Combo> ComboManager::getActiveCombos() const {
    DynamicArray<Combo> res;
    DynamicArray<Combo> all = combos.values();
    for (int i = 0; i < all.size(); ++i) {
        if (all[i].getTrangThai()) res.push_back(all[i]);
    }
    return res;
}

DynamicArray<Combo> ComboManager::getAllCombos() const {
    return combos.values();
}
