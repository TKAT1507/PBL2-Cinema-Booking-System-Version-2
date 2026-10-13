#ifndef ROOM_H
#define ROOM_H

#include <string>

class Room {
private:
    std::string maPhong;
    std::string tenPhong;
    int soHang;
    int soCot;
    bool trangThai;

public:
    Room();
    Room(const std::string& maPhong, const std::string& tenPhong, int soHang, int soCot, bool trangThai);

    std::string getMaPhong() const;
    void setMaPhong(const std::string& maPhong);

    std::string getTenPhong() const;
    void setTenPhong(const std::string& tenPhong);

    int getSoHang() const;
    void setSoHang(int soHang);

    int getSoCot() const;
    void setSoCot(int soCot);

    bool getTrangThai() const;
    void setTrangThai(bool trangThai);

    std::string toString() const;
    static Room fromString(const std::string& line);
};

#endif // ROOM_H
