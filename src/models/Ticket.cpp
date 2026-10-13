#include "../../include/models/Ticket.h"
#include <sstream>

Ticket::Ticket() : hang(0), cot(0), giaVe(0.0), trangThai("VALID") {}

Ticket::Ticket(const std::string& maVe, const std::string& maSuatChieu, const std::string& maKhachHang, const std::string& maNhanVien, int hang, int cot, double giaVe, const std::string& maCombo, const std::string& ngayMua, const std::string& trangThai)
    : maVe(maVe), maSuatChieu(maSuatChieu), maKhachHang(maKhachHang), maNhanVien(maNhanVien), hang(hang), cot(cot), giaVe(giaVe), maCombo(maCombo), ngayMua(ngayMua), trangThai(trangThai) {}

std::string Ticket::getMaVe() const { return maVe; }
void Ticket::setMaVe(const std::string& maVe) { this->maVe = maVe; }

std::string Ticket::getMaSuatChieu() const { return maSuatChieu; }
void Ticket::setMaSuatChieu(const std::string& maSuatChieu) { this->maSuatChieu = maSuatChieu; }

std::string Ticket::getMaKhachHang() const { return maKhachHang; }
void Ticket::setMaKhachHang(const std::string& maKhachHang) { this->maKhachHang = maKhachHang; }

std::string Ticket::getMaNhanVien() const { return maNhanVien; }
void Ticket::setMaNhanVien(const std::string& maNhanVien) { this->maNhanVien = maNhanVien; }

int Ticket::getHang() const { return hang; }
void Ticket::setHang(int hang) { this->hang = hang; }

int Ticket::getCot() const { return cot; }
void Ticket::setCot(int cot) { this->cot = cot; }

double Ticket::getGiaVe() const { return giaVe; }
void Ticket::setGiaVe(double giaVe) { this->giaVe = giaVe; }

std::string Ticket::getMaCombo() const { return maCombo; }
void Ticket::setMaCombo(const std::string& maCombo) { this->maCombo = maCombo; }

std::string Ticket::getNgayMua() const { return ngayMua; }
void Ticket::setNgayMua(const std::string& ngayMua) { this->ngayMua = ngayMua; }

std::string Ticket::getTrangThai() const { return trangThai; }
void Ticket::setTrangThai(const std::string& trangThai) { this->trangThai = trangThai; }

void Ticket::markUsed() {
    trangThai = "USED";
}

void Ticket::cancel() {
    trangThai = "CANCELLED";
}

bool Ticket::isValid() const {
    return trangThai == "VALID";
}

std::string Ticket::toString() const {
    std::ostringstream oss;
    oss << maVe << "|" << maSuatChieu << "|" << maKhachHang << "|" << maNhanVien << "|" << hang << "|" << cot << "|" << giaVe << "|" << maCombo << "|" << ngayMua << "|" << trangThai;
    return oss.str();
}

Ticket Ticket::fromString(const std::string& line) {
    std::istringstream iss(line);
    std::string maVe, maSuatChieu, maKhachHang, maNhanVien, hangStr, cotStr, giaVeStr, maCombo, ngayMua, trangThai;
    int hang = 0, cot = 0;
    double giaVe = 0.0;

    std::getline(iss, maVe, '|');
    std::getline(iss, maSuatChieu, '|');
    std::getline(iss, maKhachHang, '|');
    std::getline(iss, maNhanVien, '|');
    if (std::getline(iss, hangStr, '|')) hang = std::stoi(hangStr);
    if (std::getline(iss, cotStr, '|')) cot = std::stoi(cotStr);
    if (std::getline(iss, giaVeStr, '|')) giaVe = std::stod(giaVeStr);
    std::getline(iss, maCombo, '|');
    std::getline(iss, ngayMua, '|');
    std::getline(iss, trangThai, '|');

    return Ticket(maVe, maSuatChieu, maKhachHang, maNhanVien, hang, cot, giaVe, maCombo, ngayMua, trangThai);
}
