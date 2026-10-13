#include "../../include/models/SeatMap.h"
#include <sstream>

SeatMap::SeatMap() : soHang(0), soCot(0), trangThai(nullptr) {}

SeatMap::~SeatMap() {
    freeMemory();
}

void SeatMap::allocateMemory() {
    if (soHang > 0 && soCot > 0) {
        trangThai = new int*[soHang];
        for (int i = 0; i < soHang; ++i) {
            trangThai[i] = new int[soCot];
            for (int j = 0; j < soCot; ++j) {
                trangThai[i][j] = EMPTY;
            }
        }
    } else {
        trangThai = nullptr;
    }
}

void SeatMap::freeMemory() {
    if (trangThai) {
        for (int i = 0; i < soHang; ++i) {
            delete[] trangThai[i];
        }
        delete[] trangThai;
        trangThai = nullptr;
    }
}

void SeatMap::copyFrom(const SeatMap& other) {
    maSuatChieu = other.maSuatChieu;
    soHang = other.soHang;
    soCot = other.soCot;
    allocateMemory();
    if (trangThai && other.trangThai) {
        for (int i = 0; i < soHang; ++i) {
            for (int j = 0; j < soCot; ++j) {
                trangThai[i][j] = other.trangThai[i][j];
            }
        }
    }
}

SeatMap::SeatMap(const SeatMap& other) {
    copyFrom(other);
}

SeatMap& SeatMap::operator=(const SeatMap& other) {
    if (this != &other) {
        freeMemory();
        copyFrom(other);
    }
    return *this;
}

SeatMap::SeatMap(SeatMap&& other) noexcept : maSuatChieu(std::move(other.maSuatChieu)), soHang(other.soHang), soCot(other.soCot), trangThai(other.trangThai) {
    other.soHang = 0;
    other.soCot = 0;
    other.trangThai = nullptr;
}

SeatMap& SeatMap::operator=(SeatMap&& other) noexcept {
    if (this != &other) {
        freeMemory();
        maSuatChieu = std::move(other.maSuatChieu);
        soHang = other.soHang;
        soCot = other.soCot;
        trangThai = other.trangThai;
        other.soHang = 0;
        other.soCot = 0;
        other.trangThai = nullptr;
    }
    return *this;
}

void SeatMap::initFromRoom(const std::string& maSuatChieu, int soHang, int soCot, int vipRows) {
    freeMemory();
    this->maSuatChieu = maSuatChieu;
    this->soHang = soHang;
    this->soCot = soCot;
    allocateMemory();

    for (int i = 0; i < soHang; ++i) {
        for (int j = 0; j < soCot; ++j) {
            if (i >= soHang - vipRows) {
                trangThai[i][j] = VIP_EMPTY;
            } else {
                trangThai[i][j] = EMPTY;
            }
        }
    }
}

int SeatMap::getSeatStatus(int row, int col) const {
    if (row >= 0 && row < soHang && col >= 0 && col < soCot) {
        return trangThai[row][col];
    }
    return -1;
}

void SeatMap::bookSeat(int row, int col) {
    if (row >= 0 && row < soHang && col >= 0 && col < soCot) {
        if (trangThai[row][col] == EMPTY) trangThai[row][col] = BOOKED;
        else if (trangThai[row][col] == VIP_EMPTY) trangThai[row][col] = VIP_BOOKED;
    }
}

void SeatMap::cancelSeat(int row, int col) {
    if (row >= 0 && row < soHang && col >= 0 && col < soCot) {
        if (trangThai[row][col] == BOOKED) trangThai[row][col] = EMPTY;
        else if (trangThai[row][col] == VIP_BOOKED) trangThai[row][col] = VIP_EMPTY;
    }
}

bool SeatMap::isSeatAvailable(int row, int col) const {
    if (row >= 0 && row < soHang && col >= 0 && col < soCot) {
        return trangThai[row][col] == EMPTY || trangThai[row][col] == VIP_EMPTY;
    }
    return false;
}

int SeatMap::countAvailable() const {
    int count = 0;
    if (trangThai) {
        for (int i = 0; i < soHang; ++i) {
            for (int j = 0; j < soCot; ++j) {
                if (trangThai[i][j] == EMPTY || trangThai[i][j] == VIP_EMPTY) {
                    count++;
                }
            }
        }
    }
    return count;
}

int SeatMap::countBooked() const {
    int count = 0;
    if (trangThai) {
        for (int i = 0; i < soHang; ++i) {
            for (int j = 0; j < soCot; ++j) {
                if (trangThai[i][j] == BOOKED || trangThai[i][j] == VIP_BOOKED) {
                    count++;
                }
            }
        }
    }
    return count;
}

std::string SeatMap::getMaSuatChieu() const { return maSuatChieu; }
int SeatMap::getSoHang() const { return soHang; }
int SeatMap::getSoCot() const { return soCot; }

std::string SeatMap::toString() const {
    std::ostringstream oss;
    oss << maSuatChieu << "|" << soHang << "|" << soCot << "|";
    bool first = true;
    for (int i = 0; i < soHang; ++i) {
        for (int j = 0; j < soCot; ++j) {
            if (!first) oss << ",";
            oss << trangThai[i][j];
            first = false;
        }
    }
    return oss.str();
}

SeatMap SeatMap::fromString(const std::string& line) {
    std::istringstream iss(line);
    std::string maSuatChieu, soHangStr, soCotStr, flatData;
    SeatMap sm;

    if (std::getline(iss, maSuatChieu, '|') &&
        std::getline(iss, soHangStr, '|') &&
        std::getline(iss, soCotStr, '|')) {
        
        sm.maSuatChieu = maSuatChieu;
        sm.soHang = std::stoi(soHangStr);
        sm.soCot = std::stoi(soCotStr);
        sm.allocateMemory();

        if (std::getline(iss, flatData)) {
            std::istringstream dataIss(flatData);
            std::string valStr;
            for (int i = 0; i < sm.soHang; ++i) {
                for (int j = 0; j < sm.soCot; ++j) {
                    if (std::getline(dataIss, valStr, ',')) {
                        sm.trangThai[i][j] = std::stoi(valStr);
                    }
                }
            }
        }
    }
    return sm;
}
