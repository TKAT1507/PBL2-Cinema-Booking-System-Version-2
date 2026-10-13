#include "../../include/models/User.h"
#include <sstream>

User::User() : tuoi(0) {}

User::User(const std::string& taiKhoan, const std::string& matKhau, const std::string& soDienThoai, int tuoi, const std::string& vaiTro)
    : taiKhoan(taiKhoan), matKhau(matKhau), soDienThoai(soDienThoai), tuoi(tuoi), vaiTro(vaiTro) {}

std::string User::getTaiKhoan() const { return taiKhoan; }
void User::setTaiKhoan(const std::string& taiKhoan) { this->taiKhoan = taiKhoan; }

std::string User::getMatKhau() const { return matKhau; }
void User::setMatKhau(const std::string& matKhau) { this->matKhau = matKhau; }

std::string User::getSoDienThoai() const { return soDienThoai; }
void User::setSoDienThoai(const std::string& soDienThoai) { this->soDienThoai = soDienThoai; }

int User::getTuoi() const { return tuoi; }
void User::setTuoi(int tuoi) { this->tuoi = tuoi; }

std::string User::getVaiTro() const { return vaiTro; }
void User::setVaiTro(const std::string& vaiTro) { this->vaiTro = vaiTro; }

std::string User::toString() const {
    std::ostringstream oss;
    oss << taiKhoan << "|" << matKhau << "|" << soDienThoai << "|" << tuoi << "|" << vaiTro;
    return oss.str();
}
