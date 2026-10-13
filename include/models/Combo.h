#ifndef COMBO_H
#define COMBO_H

#include <string>

class Combo {
private:
    std::string maCombo;
    std::string tenCombo;
    std::string moTa;
    double gia;
    bool trangThai;

public:
    Combo();
    Combo(const std::string& maCombo, const std::string& tenCombo, const std::string& moTa, double gia, bool trangThai);

    std::string getMaCombo() const;
    void setMaCombo(const std::string& maCombo);

    std::string getTenCombo() const;
    void setTenCombo(const std::string& tenCombo);

    std::string getMoTa() const;
    void setMoTa(const std::string& moTa);

    double getGia() const;
    void setGia(double gia);

    bool getTrangThai() const;
    void setTrangThai(bool trangThai);

    std::string toString() const;
    static Combo fromString(const std::string& line);
};

#endif // COMBO_H
