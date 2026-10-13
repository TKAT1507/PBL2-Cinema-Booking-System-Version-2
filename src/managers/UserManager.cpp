#include "managers/UserManager.h"
#include "utils/FileManager.h"
#include "utils/IDGenerator.h"

void UserManager::loadAll() {
    customers.clear();
    DynamicArray<std::string> lines = FileManager::readLines(FileManager::getDataPath("customers.txt"));
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].empty()) continue;
        customers.push_back(Customer::fromString(lines[i]));
    }

    cashiers.clear();
    lines = FileManager::readLines(FileManager::getDataPath("cashiers.txt"));
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].empty()) continue;
        cashiers.push_back(Cashier::fromString(lines[i]));
    }

    admins.clear();
    lines = FileManager::readLines(FileManager::getDataPath("admins.txt"));
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].empty()) continue;
        admins.push_back(Admin::fromString(lines[i]));
    }
}

void UserManager::saveAll() {
    DynamicArray<std::string> lines;
    for (int i = 0; i < customers.size(); ++i) lines.push_back(customers[i].toString());
    FileManager::writeLines(FileManager::getDataPath("customers.txt"), lines);

    lines.clear();
    for (int i = 0; i < cashiers.size(); ++i) lines.push_back(cashiers[i].toString());
    FileManager::writeLines(FileManager::getDataPath("cashiers.txt"), lines);

    lines.clear();
    for (int i = 0; i < admins.size(); ++i) lines.push_back(admins[i].toString());
    FileManager::writeLines(FileManager::getDataPath("admins.txt"), lines);
}

bool UserManager::taiKhoanExists(const std::string& taiKhoan) {
    for (int i = 0; i < customers.size(); ++i) {
        if (customers[i].getTaiKhoan() == taiKhoan) return true;
    }
    for (int i = 0; i < cashiers.size(); ++i) {
        if (cashiers[i].getTaiKhoan() == taiKhoan) return true;
    }
    for (int i = 0; i < admins.size(); ++i) {
        if (admins[i].getTaiKhoan() == taiKhoan) return true;
    }
    return false;
}

bool UserManager::dangKy(const std::string& taiKhoan, const std::string& matKhau, const std::string& soDienThoai, int tuoi) {
    if (taiKhoanExists(taiKhoan)) return false;

    DynamicArray<std::string> existingIDs;
    for (int i = 0; i < customers.size(); ++i) existingIDs.push_back(customers[i].getMaKhachHang());

    Customer newCust;
    newCust.setTaiKhoan(taiKhoan);
    newCust.setMatKhau(matKhau);
    newCust.setSoDienThoai(soDienThoai);
    newCust.setTuoi(tuoi);
    newCust.setVaiTro("CUSTOMER");
    newCust.setMaKhachHang(IDGenerator::generateID("KH", existingIDs));

    customers.push_back(newCust);
    saveAll();
    return true;
}

int UserManager::dangNhap(const std::string& taiKhoan, const std::string& matKhau, std::string& role, std::string& idOut) {
    for (int i = 0; i < customers.size(); ++i) {
        if (customers[i].getTaiKhoan() == taiKhoan && customers[i].getMatKhau() == matKhau) {
            role = "CUSTOMER";
            idOut = customers[i].getMaKhachHang();
            return i;
        }
    }
    for (int i = 0; i < cashiers.size(); ++i) {
        if (cashiers[i].getTaiKhoan() == taiKhoan && cashiers[i].getMatKhau() == matKhau) {
            role = "CASHIER";
            idOut = cashiers[i].getMaNhanVien();
            return i;
        }
    }
    for (int i = 0; i < admins.size(); ++i) {
        if (admins[i].getTaiKhoan() == taiKhoan && admins[i].getMatKhau() == matKhau) {
            role = "ADMIN";
            idOut = admins[i].getMaQuanLy();
            return i;
        }
    }
    return -1;
}

bool UserManager::doiMatKhau(const std::string& taiKhoan, const std::string& matKhauCu, const std::string& matKhauMoi) {
    for (int i = 0; i < customers.size(); ++i) {
        if (customers[i].getTaiKhoan() == taiKhoan && customers[i].getMatKhau() == matKhauCu) {
            customers[i].setMatKhau(matKhauMoi);
            saveAll();
            return true;
        }
    }
    for (int i = 0; i < cashiers.size(); ++i) {
        if (cashiers[i].getTaiKhoan() == taiKhoan && cashiers[i].getMatKhau() == matKhauCu) {
            cashiers[i].setMatKhau(matKhauMoi);
            saveAll();
            return true;
        }
    }
    for (int i = 0; i < admins.size(); ++i) {
        if (admins[i].getTaiKhoan() == taiKhoan && admins[i].getMatKhau() == matKhauCu) {
            admins[i].setMatKhau(matKhauMoi);
            saveAll();
            return true;
        }
    }
    return false;
}

Customer* UserManager::findCustomerById(const std::string& maKH) {
    for (int i = 0; i < customers.size(); ++i) {
        if (customers[i].getMaKhachHang() == maKH) return &customers[i];
    }
    return nullptr;
}

Cashier* UserManager::findCashierById(const std::string& maNV) {
    for (int i = 0; i < cashiers.size(); ++i) {
        if (cashiers[i].getMaNhanVien() == maNV) return &cashiers[i];
    }
    return nullptr;
}

Admin* UserManager::findAdminById(const std::string& maQL) {
    for (int i = 0; i < admins.size(); ++i) {
        if (admins[i].getMaQuanLy() == maQL) return &admins[i];
    }
    return nullptr;
}

DynamicArray<Customer>& UserManager::getCustomers() { return customers; }
DynamicArray<Cashier>& UserManager::getCashiers() { return cashiers; }
DynamicArray<Admin>& UserManager::getAdmins() { return admins; }
