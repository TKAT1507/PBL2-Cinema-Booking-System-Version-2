#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include <string>

class Admin : public User {
private:
    std::string maQuanLy;
    std::string tenQuanLy;

public:
    Admin();
    Admin(const std::string& taiKhoan, const std::string& matKhau, const std::string& soDienThoai, int tuoi, const std::string& vaiTro, const std::string& maQuanLy, const std::string& tenQuanLy);

    std::string getMaQuanLy() const;
    void setMaQuanLy(const std::string& maQuanLy);

    std::string getTenQuanLy() const;
    void setTenQuanLy(const std::string& tenQuanLy);

    std::string getRoleName() const override;
    std::string toString() const;
    static Admin fromString(const std::string& line);
};

#endif // ADMIN_H
