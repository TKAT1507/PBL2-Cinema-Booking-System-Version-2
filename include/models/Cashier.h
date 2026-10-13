#ifndef CASHIER_H
#define CASHIER_H

#include "User.h"
#include <string>

class Cashier : public User {
private:
    std::string maNhanVien;
    std::string tenNhanVien;

public:
    Cashier();
    Cashier(const std::string& taiKhoan, const std::string& matKhau, const std::string& soDienThoai, int tuoi, const std::string& vaiTro, const std::string& maNhanVien, const std::string& tenNhanVien);

    std::string getMaNhanVien() const;
    void setMaNhanVien(const std::string& maNhanVien);

    std::string getTenNhanVien() const;
    void setTenNhanVien(const std::string& tenNhanVien);

    std::string getRoleName() const override;
    std::string toString() const;
    static Cashier fromString(const std::string& line);
};

#endif // CASHIER_H
