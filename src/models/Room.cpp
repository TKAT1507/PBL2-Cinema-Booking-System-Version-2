#include "../../include/models/Room.h"
#include <sstream>

Room::Room() : soHang(0), soCot(0), trangThai(false) {}

Room::Room(const std::string& maPhong, const std::string& tenPhong, int soHang, int soCot, bool trangThai)
    : maPhong(maPhong), tenPhong(tenPhong), soHang(soHang), soCot(soCot), trangThai(trangThai) {}

std::string Room::getMaPhong() const { return maPhong; }
void Room::setMaPhong(const std::string& maPhong) { this->maPhong = maPhong; }

std::string Room::getTenPhong() const { return tenPhong; }
void Room::setTenPhong(const std::string& tenPhong) { this->tenPhong = tenPhong; }

int Room::getSoHang() const { return soHang; }
void Room::setSoHang(int soHang) { this->soHang = soHang; }

int Room::getSoCot() const { return soCot; }
void Room::setSoCot(int soCot) { this->soCot = soCot; }

bool Room::getTrangThai() const { return trangThai; }
void Room::setTrangThai(bool trangThai) { this->trangThai = trangThai; }

std::string Room::toString() const {
    std::ostringstream oss;
    oss << maPhong << "|" << tenPhong << "|" << soHang << "|" << soCot << "|" << (trangThai ? "1" : "0");
    return oss.str();
}

Room Room::fromString(const std::string& line) {
    std::istringstream iss(line);
    std::string maPhong, tenPhong, soHangStr, soCotStr, trangThaiStr;
    int soHang = 0, soCot = 0;
    bool trangThai = false;

    std::getline(iss, maPhong, '|');
    std::getline(iss, tenPhong, '|');
    if (std::getline(iss, soHangStr, '|')) soHang = std::stoi(soHangStr);
    if (std::getline(iss, soCotStr, '|')) soCot = std::stoi(soCotStr);
    if (std::getline(iss, trangThaiStr, '|')) trangThai = (trangThaiStr == "1");

    return Room(maPhong, tenPhong, soHang, soCot, trangThai);
}
