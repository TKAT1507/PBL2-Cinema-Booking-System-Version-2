#ifndef SHOWTIME_H
#define SHOWTIME_H

#include <string>

class Showtime {
private:
    std::string maSuatChieu;
    std::string maPhong;
    std::string maPhim;
    std::string ngayChieu; // dd/MM/yyyy
    std::string gioChieu;  // HH:mm
    bool trangThai;

public:
    Showtime();
    Showtime(const std::string& maSuatChieu, const std::string& maPhong, const std::string& maPhim, const std::string& ngayChieu, const std::string& gioChieu, bool trangThai);

    std::string getMaSuatChieu() const;
    void setMaSuatChieu(const std::string& maSuatChieu);

    std::string getMaPhong() const;
    void setMaPhong(const std::string& maPhong);

    std::string getMaPhim() const;
    void setMaPhim(const std::string& maPhim);

    std::string getNgayChieu() const;
    void setNgayChieu(const std::string& ngayChieu);

    std::string getGioChieu() const;
    void setGioChieu(const std::string& gioChieu);

    bool getTrangThai() const;
    void setTrangThai(bool trangThai);

    std::string toString() const;
    static Showtime fromString(const std::string& line);
};

#endif // SHOWTIME_H
