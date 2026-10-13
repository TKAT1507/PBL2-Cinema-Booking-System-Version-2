#include "../../include/models/Showtime.h"
#include <sstream>

Showtime::Showtime() : trangThai(false) {}

Showtime::Showtime(const std::string& maSuatChieu, const std::string& maPhong, const std::string& maPhim, const std::string& ngayChieu, const std::string& gioChieu, bool trangThai)
    : maSuatChieu(maSuatChieu), maPhong(maPhong), maPhim(maPhim), ngayChieu(ngayChieu), gioChieu(gioChieu), trangThai(trangThai) {}

std::string Showtime::getMaSuatChieu() const { return maSuatChieu; }
void Showtime::setMaSuatChieu(const std::string& maSuatChieu) { this->maSuatChieu = maSuatChieu; }

std::string Showtime::getMaPhong() const { return maPhong; }
void Showtime::setMaPhong(const std::string& maPhong) { this->maPhong = maPhong; }

std::string Showtime::getMaPhim() const { return maPhim; }
void Showtime::setMaPhim(const std::string& maPhim) { this->maPhim = maPhim; }

std::string Showtime::getNgayChieu() const { return ngayChieu; }
void Showtime::setNgayChieu(const std::string& ngayChieu) { this->ngayChieu = ngayChieu; }

std::string Showtime::getGioChieu() const { return gioChieu; }
void Showtime::setGioChieu(const std::string& gioChieu) { this->gioChieu = gioChieu; }

bool Showtime::getTrangThai() const { return trangThai; }
void Showtime::setTrangThai(bool trangThai) { this->trangThai = trangThai; }

std::string Showtime::toString() const {
    std::ostringstream oss;
    oss << maSuatChieu << "|" << maPhong << "|" << maPhim << "|" << ngayChieu << "|" << gioChieu << "|" << (trangThai ? "1" : "0");
    return oss.str();
}

Showtime Showtime::fromString(const std::string& line) {
    std::istringstream iss(line);
    std::string maSuatChieu, maPhong, maPhim, ngayChieu, gioChieu, trangThaiStr;
    bool trangThai = false;

    std::getline(iss, maSuatChieu, '|');
    std::getline(iss, maPhong, '|');
    std::getline(iss, maPhim, '|');
    std::getline(iss, ngayChieu, '|');
    std::getline(iss, gioChieu, '|');
    if (std::getline(iss, trangThaiStr, '|')) trangThai = (trangThaiStr == "1");

    return Showtime(maSuatChieu, maPhong, maPhim, ngayChieu, gioChieu, trangThai);
}
