#include "managers/ShowtimeManager.h"
#include "utils/FileManager.h"
#include "utils/IDGenerator.h"
#include <string>

static int parseTime(const std::string& timeStr) {
    if (timeStr.length() < 5) return 0;
    int hh = std::stoi(timeStr.substr(0, 2));
    int mm = std::stoi(timeStr.substr(3, 2));
    return hh * 60 + mm;
}

void ShowtimeManager::load() {
    showtimes.clear();
    DynamicArray<std::string> lines = FileManager::readLines(FileManager::getDataPath("showtimes.txt"));
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].empty()) continue;
        showtimes.push_back(Showtime::fromString(lines[i]));
    }
}

void ShowtimeManager::save() {
    DynamicArray<std::string> lines;
    for (int i = 0; i < showtimes.size(); ++i) {
        lines.push_back(showtimes[i].toString());
    }
    FileManager::writeLines(FileManager::getDataPath("showtimes.txt"), lines);
}

void ShowtimeManager::loadSeatMaps() {
    seatMaps.clear();
    DynamicArray<std::string> lines = FileManager::readLines(FileManager::getDataPath("seatmaps.txt"));
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].empty()) continue;
        seatMaps.push_back(SeatMap::fromString(lines[i]));
    }
}

void ShowtimeManager::saveSeatMaps() {
    DynamicArray<std::string> lines;
    for (int i = 0; i < seatMaps.size(); ++i) {
        lines.push_back(seatMaps[i].toString());
    }
    FileManager::writeLines(FileManager::getDataPath("seatmaps.txt"), lines);
}

bool ShowtimeManager::hasTimeConflict(const std::string& maPhong, const std::string& ngayChieu, const std::string& gioChieu, int thoiLuongPhut, const std::string& excludeId) const {
    int startNew = parseTime(gioChieu);
    int endNew = startNew + thoiLuongPhut + 15;

    for (int i = 0; i < showtimes.size(); ++i) {
        if (!showtimes[i].getTrangThai()) continue;
        if (showtimes[i].getMaPhong() == maPhong && showtimes[i].getNgayChieu() == ngayChieu) {
            if (showtimes[i].getMaSuatChieu() == excludeId) continue;
            
            // Assuming 150-minute block for the existing one for conflict check since we can't fetch thoiLuong easily here
            int startOld = parseTime(showtimes[i].getGioChieu());
            int endOld = startOld + 150; 

            if ((startNew >= startOld && startNew < endOld) || (endNew > startOld && endNew <= endOld) ||
                (startNew <= startOld && endNew >= endOld)) {
                return true;
            }
        }
    }
    return false;
}

bool ShowtimeManager::addShowtime(const Showtime& st, int soHang, int soCot, int vipRows) {
    DynamicArray<std::string> existingIDs;
    for (int i = 0; i < showtimes.size(); ++i) {
        existingIDs.push_back(showtimes[i].getMaSuatChieu());
    }
    Showtime newSt = st;
    std::string maSC = IDGenerator::generateID("ST", existingIDs);
    newSt.setMaSuatChieu(maSC);
    
    showtimes.push_back(newSt);
    save();

    SeatMap sm;
    sm.initFromRoom(maSC, soHang, soCot, vipRows);
    seatMaps.push_back(sm);
    saveSeatMaps();

    return true;
}

bool ShowtimeManager::updateShowtime(const std::string& maSC, const Showtime& updated) {
    for (int i = 0; i < showtimes.size(); ++i) {
        if (showtimes[i].getMaSuatChieu() == maSC) {
            showtimes[i] = updated;
            showtimes[i].setMaSuatChieu(maSC);
            save();
            return true;
        }
    }
    return false;
}

bool ShowtimeManager::cancelShowtime(const std::string& maSC) {
    for (int i = 0; i < showtimes.size(); ++i) {
        if (showtimes[i].getMaSuatChieu() == maSC) {
            showtimes[i].setTrangThai(false);
            save();
            return true;
        }
    }
    return false;
}

Showtime* ShowtimeManager::findById(const std::string& maSC) {
    for (int i = 0; i < showtimes.size(); ++i) {
        if (showtimes[i].getMaSuatChieu() == maSC) return &showtimes[i];
    }
    return nullptr;
}

SeatMap* ShowtimeManager::findSeatMap(const std::string& maSuatChieu) {
    for (int i = 0; i < seatMaps.size(); ++i) {
        if (seatMaps[i].getMaSuatChieu() == maSuatChieu) return &seatMaps[i];
    }
    return nullptr;
}

DynamicArray<Showtime> ShowtimeManager::getActiveShowtimes() const {
    DynamicArray<Showtime> res;
    for (int i = 0; i < showtimes.size(); ++i) {
        if (showtimes[i].getTrangThai()) res.push_back(showtimes[i]);
    }
    return res;
}

DynamicArray<Showtime> ShowtimeManager::getShowtimesByMovie(const std::string& maPhim) const {
    DynamicArray<Showtime> res;
    for (int i = 0; i < showtimes.size(); ++i) {
        if (showtimes[i].getTrangThai() && showtimes[i].getMaPhim() == maPhim) res.push_back(showtimes[i]);
    }
    return res;
}

DynamicArray<Showtime> ShowtimeManager::getShowtimesByDate(const std::string& ngayChieu) const {
    DynamicArray<Showtime> res;
    for (int i = 0; i < showtimes.size(); ++i) {
        if (showtimes[i].getTrangThai() && showtimes[i].getNgayChieu() == ngayChieu) res.push_back(showtimes[i]);
    }
    return res;
}

DynamicArray<Showtime>& ShowtimeManager::getAllShowtimes() { return showtimes; }
DynamicArray<SeatMap>& ShowtimeManager::getAllSeatMaps() { return seatMaps; }
