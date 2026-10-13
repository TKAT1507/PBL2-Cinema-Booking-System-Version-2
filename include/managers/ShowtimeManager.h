#ifndef SHOWTIMEMANAGER_H
#define SHOWTIMEMANAGER_H

#include "core/DynamicArray.h"
#include "models/Showtime.h"
#include "models/SeatMap.h"
#include <string>

class ShowtimeManager {
private:
    DynamicArray<Showtime> showtimes;
    DynamicArray<SeatMap> seatMaps;
public:
    void load();
    void save();
    void loadSeatMaps();
    void saveSeatMaps();

    bool addShowtime(const Showtime& st, int soHang, int soCot, int vipRows = 2);
    bool updateShowtime(const std::string& maSC, const Showtime& updated);
    bool cancelShowtime(const std::string& maSC);
    
    Showtime* findById(const std::string& maSC);
    SeatMap* findSeatMap(const std::string& maSuatChieu);
    
    DynamicArray<Showtime> getActiveShowtimes() const;
    DynamicArray<Showtime> getShowtimesByMovie(const std::string& maPhim) const;
    DynamicArray<Showtime> getShowtimesByDate(const std::string& ngayChieu) const;
    
    bool hasTimeConflict(const std::string& maPhong, const std::string& ngayChieu, const std::string& gioChieu, int thoiLuongPhut, const std::string& excludeId = "") const;
    
    DynamicArray<Showtime>& getAllShowtimes();
    DynamicArray<SeatMap>& getAllSeatMaps();
};

#endif // SHOWTIMEMANAGER_H
