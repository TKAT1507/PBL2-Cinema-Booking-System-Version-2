#ifndef SEAT_MAP_H
#define SEAT_MAP_H

#include <string>

class SeatMap {
public:
    enum SeatStatus { EMPTY = 0, BOOKED = 1, VIP_EMPTY = 2, VIP_BOOKED = 3 };

private:
    std::string maSuatChieu;
    int soHang;
    int soCot;
    int** trangThai;

    void allocateMemory();
    void freeMemory();
    void copyFrom(const SeatMap& other);

public:
    SeatMap();
    ~SeatMap();
    SeatMap(const SeatMap& other);
    SeatMap& operator=(const SeatMap& other);
    SeatMap(SeatMap&& other) noexcept;
    SeatMap& operator=(SeatMap&& other) noexcept;

    void initFromRoom(const std::string& maSuatChieu, int soHang, int soCot, int vipRows = 2);

    int getSeatStatus(int row, int col) const;
    void bookSeat(int row, int col);
    void cancelSeat(int row, int col);
    bool isSeatAvailable(int row, int col) const;
    int countAvailable() const;
    int countBooked() const;

    std::string getMaSuatChieu() const;
    int getSoHang() const;
    int getSoCot() const;

    std::string toString() const;
    static SeatMap fromString(const std::string& line);
};

#endif // SEAT_MAP_H
