#ifndef TICKET_H
#define TICKET_H

#include <string>

class Ticket {
private:
    std::string maVe;
    std::string maSuatChieu;
    std::string maKhachHang;
    std::string maNhanVien;
    int hang;
    int cot;
    double giaVe;
    std::string maCombo;
    std::string ngayMua; // dd/MM/yyyy
    std::string trangThai; // PENDING, VALID, USED, CANCELLED
    std::string maCumVe; // Batch/Order ID (e.g. ORD001)

public:
    Ticket();
    Ticket(const std::string& maVe, const std::string& maSuatChieu, const std::string& maKhachHang, const std::string& maNhanVien, int hang, int cot, double giaVe, const std::string& maCombo, const std::string& ngayMua, const std::string& trangThai, const std::string& maCumVe = "");

    std::string getMaVe() const;
    void setMaVe(const std::string& maVe);

    std::string getMaSuatChieu() const;
    void setMaSuatChieu(const std::string& maSuatChieu);

    std::string getMaKhachHang() const;
    void setMaKhachHang(const std::string& maKhachHang);

    std::string getMaNhanVien() const;
    void setMaNhanVien(const std::string& maNhanVien);

    int getHang() const;
    void setHang(int hang);

    int getCot() const;
    void setCot(int cot);

    double getGiaVe() const;
    void setGiaVe(double giaVe);

    std::string getMaCombo() const;
    void setMaCombo(const std::string& maCombo);

    std::string getNgayMua() const;
    void setNgayMua(const std::string& ngayMua);

    std::string getTrangThai() const;
    void setTrangThai(const std::string& trangThai);

    std::string getMaCumVe() const;
    void setMaCumVe(const std::string& maCumVe);

    void markUsed();
    void cancel();
    bool isValid() const;
    bool isPending() const;

    std::string toString() const;
    static Ticket fromString(const std::string& line);
};

#endif // TICKET_H
