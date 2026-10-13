#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "core/DynamicArray.h"
#include "models/Customer.h"
#include "models/Cashier.h"
#include "models/Admin.h"
#include <string>

class UserManager {
private:
    DynamicArray<Customer> customers;
    DynamicArray<Cashier> cashiers;
    DynamicArray<Admin> admins;
public:
    void loadAll();
    void saveAll();
    bool dangKy(const std::string& taiKhoan, const std::string& matKhau, const std::string& soDienThoai, int tuoi);
    int dangNhap(const std::string& taiKhoan, const std::string& matKhau, std::string& role, std::string& odId);
    bool doiMatKhau(const std::string& taiKhoan, const std::string& matKhauCu, const std::string& matKhauMoi);
    Customer* findCustomerById(const std::string& maKH);
    Cashier* findCashierById(const std::string& maNV);
    Admin* findAdminById(const std::string& maQL);
    bool taiKhoanExists(const std::string& taiKhoan);

    DynamicArray<Customer>& getCustomers();
    DynamicArray<Cashier>& getCashiers();
    DynamicArray<Admin>& getAdmins();
};

#endif // USERMANAGER_H
