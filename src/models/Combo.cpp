#include "../../include/models/Combo.h"
#include <sstream>

Combo::Combo() : gia(0.0), trangThai(false) {}

Combo::Combo(const std::string& maCombo, const std::string& tenCombo, const std::string& moTa, double gia, bool trangThai)
    : maCombo(maCombo), tenCombo(tenCombo), moTa(moTa), gia(gia), trangThai(trangThai) {}

std::string Combo::getMaCombo() const { return maCombo; }
void Combo::setMaCombo(const std::string& maCombo) { this->maCombo = maCombo; }

std::string Combo::getTenCombo() const { return tenCombo; }
void Combo::setTenCombo(const std::string& tenCombo) { this->tenCombo = tenCombo; }

std::string Combo::getMoTa() const { return moTa; }
void Combo::setMoTa(const std::string& moTa) { this->moTa = moTa; }

double Combo::getGia() const { return gia; }
void Combo::setGia(double gia) { this->gia = gia; }

bool Combo::getTrangThai() const { return trangThai; }
void Combo::setTrangThai(bool trangThai) { this->trangThai = trangThai; }

std::string Combo::toString() const {
    std::ostringstream oss;
    oss << maCombo << "|" << tenCombo << "|" << moTa << "|" << gia << "|" << (trangThai ? "1" : "0");
    return oss.str();
}

Combo Combo::fromString(const std::string& line) {
    std::istringstream iss(line);
    std::string maCombo, tenCombo, moTa, giaStr, trangThaiStr;
    double gia = 0.0;
    bool trangThai = false;

    std::getline(iss, maCombo, '|');
    std::getline(iss, tenCombo, '|');
    std::getline(iss, moTa, '|');
    if (std::getline(iss, giaStr, '|')) gia = std::stod(giaStr);
    if (std::getline(iss, trangThaiStr, '|')) trangThai = (trangThaiStr == "1");

    return Combo(maCombo, tenCombo, moTa, gia, trangThai);
}
