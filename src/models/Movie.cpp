#include "../../include/models/Movie.h"
#include <sstream>

Movie::Movie() : thoiLuong(0), doTuoi(0), trangThai(false) {}

Movie::Movie(const std::string& maPhim, const std::string& tenPhim, const std::string& theLoai, int thoiLuong, int doTuoi, const std::string& moTa, const std::string& noiDung, const std::string& poster, bool trangThai)
    : maPhim(maPhim), tenPhim(tenPhim), theLoai(theLoai), thoiLuong(thoiLuong), doTuoi(doTuoi), moTa(moTa), noiDung(noiDung), poster(poster), trangThai(trangThai) {}

std::string Movie::getMaPhim() const { return maPhim; }
void Movie::setMaPhim(const std::string& maPhim) { this->maPhim = maPhim; }

std::string Movie::getTenPhim() const { return tenPhim; }
void Movie::setTenPhim(const std::string& tenPhim) { this->tenPhim = tenPhim; }

std::string Movie::getTheLoai() const { return theLoai; }
void Movie::setTheLoai(const std::string& theLoai) { this->theLoai = theLoai; }

int Movie::getThoiLuong() const { return thoiLuong; }
void Movie::setThoiLuong(int thoiLuong) { this->thoiLuong = thoiLuong; }

int Movie::getDoTuoi() const { return doTuoi; }
void Movie::setDoTuoi(int doTuoi) { this->doTuoi = doTuoi; }

std::string Movie::getMoTa() const { return moTa; }
void Movie::setMoTa(const std::string& moTa) { this->moTa = moTa; }

std::string Movie::getNoiDung() const { return noiDung; }
void Movie::setNoiDung(const std::string& noiDung) { this->noiDung = noiDung; }

std::string Movie::getPoster() const { return poster; }
void Movie::setPoster(const std::string& poster) { this->poster = poster; }

bool Movie::getTrangThai() const { return trangThai; }
void Movie::setTrangThai(bool trangThai) { this->trangThai = trangThai; }

std::string Movie::toString() const {
    std::ostringstream oss;
    oss << maPhim << "|" << tenPhim << "|" << theLoai << "|" << thoiLuong << "|" << doTuoi << "|" << moTa << "|" << noiDung << "|" << poster << "|" << (trangThai ? "1" : "0");
    return oss.str();
}

Movie Movie::fromString(const std::string& line) {
    std::istringstream iss(line);
    std::string maPhim, tenPhim, theLoai, thoiLuongStr, doTuoiStr, moTa, noiDung, poster, trangThaiStr;
    int thoiLuong = 0, doTuoi = 0;
    bool trangThai = false;

    std::getline(iss, maPhim, '|');
    std::getline(iss, tenPhim, '|');
    std::getline(iss, theLoai, '|');
    if (std::getline(iss, thoiLuongStr, '|')) thoiLuong = std::stoi(thoiLuongStr);
    if (std::getline(iss, doTuoiStr, '|')) doTuoi = std::stoi(doTuoiStr);
    std::getline(iss, moTa, '|');
    std::getline(iss, noiDung, '|');
    std::getline(iss, poster, '|');
    if (std::getline(iss, trangThaiStr, '|')) trangThai = (trangThaiStr == "1");

    return Movie(maPhim, tenPhim, theLoai, thoiLuong, doTuoi, moTa, noiDung, poster, trangThai);
}
