#include "../../include/models/Customer.h"
#include <sstream>

Customer::Customer() : User() {}

Customer::Customer(const std::string& taiKhoan, const std::string& matKhau, const std::string& soDienThoai, int tuoi, const std::string& vaiTro, const std::string& maKhachHang)
    : User(taiKhoan, matKhau, soDienThoai, tuoi, vaiTro), maKhachHang(maKhachHang) {}

std::string Customer::getMaKhachHang() const { return maKhachHang; }
void Customer::setMaKhachHang(const std::string& maKhachHang) { this->maKhachHang = maKhachHang; }

std::string Customer::getRoleName() const { return "Khach hang"; }

std::string Customer::toString() const {
    return User::toString() + "|" + maKhachHang;
}

Customer Customer::fromString(const std::string& line) {
    std::istringstream iss(line);
    std::string taiKhoan, matKhau, soDienThoai, vaiTro, maKhachHang, tuoiStr;
    int tuoi = 0;

    std::getline(iss, taiKhoan, '|');
    std::getline(iss, matKhau, '|');
    std::getline(iss, soDienThoai, '|');
    if (std::getline(iss, tuoiStr, '|')) tuoi = std::stoi(tuoiStr);
    std::getline(iss, vaiTro, '|');
    std::getline(iss, maKhachHang, '|');

    return Customer(taiKhoan, matKhau, soDienThoai, tuoi, vaiTro, maKhachHang);
}
