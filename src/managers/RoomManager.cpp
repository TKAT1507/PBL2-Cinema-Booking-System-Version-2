#include "managers/RoomManager.h"
#include "utils/FileManager.h"
#include "utils/IDGenerator.h"

void RoomManager::load() {
    rooms.clear();
    DynamicArray<std::string> lines = FileManager::readLines(FileManager::getDataPath("rooms.txt"));
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].empty()) continue;
        rooms.push_back(Room::fromString(lines[i]));
    }
}

void RoomManager::save() {
    DynamicArray<std::string> lines;
    for (int i = 0; i < rooms.size(); ++i) {
        lines.push_back(rooms[i].toString());
    }
    FileManager::writeLines(FileManager::getDataPath("rooms.txt"), lines);
}

bool RoomManager::addRoom(const Room& room) {
    DynamicArray<std::string> existingIDs;
    for (int i = 0; i < rooms.size(); ++i) {
        existingIDs.push_back(rooms[i].getMaPhong());
    }
    Room r = room;
    r.setMaPhong(IDGenerator::generateID("RM", existingIDs));
    rooms.push_back(r);
    save();
    return true;
}

bool RoomManager::updateRoom(const std::string& maPhong, const Room& updated) {
    for (int i = 0; i < rooms.size(); ++i) {
        if (rooms[i].getMaPhong() == maPhong) {
            rooms[i] = updated;
            rooms[i].setMaPhong(maPhong);
            save();
            return true;
        }
    }
    return false;
}

bool RoomManager::deactivateRoom(const std::string& maPhong) {
    for (int i = 0; i < rooms.size(); ++i) {
        if (rooms[i].getMaPhong() == maPhong) {
            rooms[i].setTrangThai(false);
            save();
            return true;
        }
    }
    return false;
}

Room* RoomManager::findById(const std::string& maPhong) {
    for (int i = 0; i < rooms.size(); ++i) {
        if (rooms[i].getMaPhong() == maPhong) {
            return &rooms[i];
        }
    }
    return nullptr;
}

DynamicArray<Room> RoomManager::getActiveRooms() const {
    DynamicArray<Room> res;
    for (int i = 0; i < rooms.size(); ++i) {
        if (rooms[i].getTrangThai()) {
            res.push_back(rooms[i]);
        }
    }
    return res;
}

DynamicArray<Room>& RoomManager::getAllRooms() {
    return rooms;
}
