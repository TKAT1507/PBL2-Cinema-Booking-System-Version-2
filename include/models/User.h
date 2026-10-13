#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
    std::string taiKhoan;
    std::string matKhau;
    std::string soDienThoai;
    int tuoi;
    std::string vaiTro;

public:
    User();
    User(const std::string& taiKhoan, const std::string& matKhau, const std::string& soDienThoai, int tuoi, const std::string& vaiTro);
    virtual ~User() = default;

    std::string getTaiKhoan() const;
    void setTaiKhoan(const std::string& taiKhoan);

    std::string getMatKhau() const;
    void setMatKhau(const std::string& matKhau);

    std::string getSoDienThoai() const;
    void setSoDienThoai(const std::string& soDienThoai);

    int getTuoi() const;
    void setTuoi(int tuoi);

    std::string getVaiTro() const;
    void setVaiTro(const std::string& vaiTro);

    virtual std::string getRoleName() const = 0;
    std::string toString() const;
};

#endif // USER_H
