#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H

#include "core/DynamicArray.h"
#include "models/Room.h"
#include <string>

class RoomManager {
private:
    DynamicArray<Room> rooms;
public:
    void load();
    void save();
    bool addRoom(const Room& room);
    bool updateRoom(const std::string& maPhong, const Room& updated);
    bool deactivateRoom(const std::string& maPhong);
    Room* findById(const std::string& maPhong);
    DynamicArray<Room> getActiveRooms() const;
    DynamicArray<Room>& getAllRooms();
};

#endif // ROOMMANAGER_H
