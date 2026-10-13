#include "../../include/models/Admin.h"
#include <sstream>

Admin::Admin() : User() {}

Admin::Admin(const std::string& taiKhoan, const std::string& matKhau, const std::string& soDienThoai, int tuoi, const std::string& vaiTro, const std::string& maQuanLy, const std::string& tenQuanLy)
    : User(taiKhoan, matKhau, soDienThoai, tuoi, vaiTro), maQuanLy(maQuanLy), tenQuanLy(tenQuanLy) {}

std::string Admin::getMaQuanLy() const { return maQuanLy; }
void Admin::setMaQuanLy(const std::string& maQuanLy) { this->maQuanLy = maQuanLy; }

std::string Admin::getTenQuanLy() const { return tenQuanLy; }
void Admin::setTenQuanLy(const std::string& tenQuanLy) { this->tenQuanLy = tenQuanLy; }

std::string Admin::getRoleName() const { return "Quan ly"; }

std::string Admin::toString() const {
    return User::toString() + "|" + maQuanLy + "|" + tenQuanLy;
}

Admin Admin::fromString(const std::string& line) {
    std::istringstream iss(line);
    std::string taiKhoan, matKhau, soDienThoai, vaiTro, maQuanLy, tenQuanLy, tuoiStr;
    int tuoi = 0;

    std::getline(iss, taiKhoan, '|');
    std::getline(iss, matKhau, '|');
    std::getline(iss, soDienThoai, '|');
    if (std::getline(iss, tuoiStr, '|')) tuoi = std::stoi(tuoiStr);
    std::getline(iss, vaiTro, '|');
    std::getline(iss, maQuanLy, '|');
    std::getline(iss, tenQuanLy, '|');

    return Admin(taiKhoan, matKhau, soDienThoai, tuoi, vaiTro, maQuanLy, tenQuanLy);
}
