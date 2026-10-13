#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include <string>

class Customer : public User {
private:
    std::string maKhachHang;

public:
    Customer();
    Customer(const std::string& taiKhoan, const std::string& matKhau, const std::string& soDienThoai, int tuoi, const std::string& vaiTro, const std::string& maKhachHang);

    std::string getMaKhachHang() const;
    void setMaKhachHang(const std::string& maKhachHang);

    std::string getRoleName() const override;
    std::string toString() const;
    static Customer fromString(const std::string& line);
};

#endif // CUSTOMER_H
