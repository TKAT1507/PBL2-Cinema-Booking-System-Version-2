#include "../../include/models/Cashier.h"
#include <sstream>

Cashier::Cashier() : User() {}

Cashier::Cashier(const std::string& taiKhoan, const std::string& matKhau, const std::string& soDienThoai, int tuoi, const std::string& vaiTro, const std::string& maNhanVien, const std::string& tenNhanVien)
    : User(taiKhoan, matKhau, soDienThoai, tuoi, vaiTro), maNhanVien(maNhanVien), tenNhanVien(tenNhanVien) {}

std::string Cashier::getMaNhanVien() const { return maNhanVien; }
void Cashier::setMaNhanVien(const std::string& maNhanVien) { this->maNhanVien = maNhanVien; }

std::string Cashier::getTenNhanVien() const { return tenNhanVien; }
void Cashier::setTenNhanVien(const std::string& tenNhanVien) { this->tenNhanVien = tenNhanVien; }

std::string Cashier::getRoleName() const { return "Thu ngan"; }

std::string Cashier::toString() const {
    return User::toString() + "|" + maNhanVien + "|" + tenNhanVien;
}

Cashier Cashier::fromString(const std::string& line) {
    std::istringstream iss(line);
    std::string taiKhoan, matKhau, soDienThoai, vaiTro, maNhanVien, tenNhanVien, tuoiStr;
    int tuoi = 0;

    std::getline(iss, taiKhoan, '|');
    std::getline(iss, matKhau, '|');
    std::getline(iss, soDienThoai, '|');
    if (std::getline(iss, tuoiStr, '|')) tuoi = std::stoi(tuoiStr);
    std::getline(iss, vaiTro, '|');
    std::getline(iss, maNhanVien, '|');
    std::getline(iss, tenNhanVien, '|');

    return Cashier(taiKhoan, matKhau, soDienThoai, tuoi, vaiTro, maNhanVien, tenNhanVien);
}
