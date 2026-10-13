#ifndef MOVIE_H
#define MOVIE_H

#include <string>

class Movie {
private:
    std::string maPhim;
    std::string tenPhim;
    std::string theLoai;
    int thoiLuong;
    int doTuoi;
    std::string moTa;
    std::string noiDung;
    std::string poster;
    bool trangThai;

public:
    Movie();
    Movie(const std::string& maPhim, const std::string& tenPhim, const std::string& theLoai, int thoiLuong, int doTuoi, const std::string& moTa, const std::string& noiDung, const std::string& poster, bool trangThai);

    std::string getMaPhim() const;
    void setMaPhim(const std::string& maPhim);

    std::string getTenPhim() const;
    void setTenPhim(const std::string& tenPhim);

    std::string getTheLoai() const;
    void setTheLoai(const std::string& theLoai);

    int getThoiLuong() const;
    void setThoiLuong(int thoiLuong);

    int getDoTuoi() const;
    void setDoTuoi(int doTuoi);

    std::string getMoTa() const;
    void setMoTa(const std::string& moTa);

    std::string getNoiDung() const;
    void setNoiDung(const std::string& noiDung);

    std::string getPoster() const;
    void setPoster(const std::string& poster);

    bool getTrangThai() const;
    void setTrangThai(bool trangThai);

    std::string toString() const;
    static Movie fromString(const std::string& line);
};

#endif // MOVIE_H
