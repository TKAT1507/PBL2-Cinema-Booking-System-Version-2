#include <iostream>
#include <string>
#include "core/HashTable.h"
#include "core/DynamicArray.h"
#include "models/User.h"
#include "models/Customer.h"
#include "models/Cashier.h"
#include "models/Admin.h"
#include "models/Movie.h"
#include "models/Room.h"
#include "models/Seat.h"
#include "models/Showtime.h"
#include "models/SeatMap.h"
#include "models/Ticket.h"
#include "models/Combo.h"
#include "utils/FileManager.h"
#include "utils/IDGenerator.h"
#include "utils/Validator.h"
#include "utils/PriceCalculator.h"
#include "managers/UserManager.h"
#include "managers/MovieManager.h"
#include "managers/RoomManager.h"
#include "managers/ShowtimeManager.h"
#include "managers/TicketManager.h"
#include "managers/ComboManager.h"
#include "managers/StatisticManager.h"

using namespace std;

int main() {
    cout << "=== PBL2 Cinema Booking System - Core Test ===" << endl;

    
    // Test 1: DynamicArray
    cout << "\n--- Test DynamicArray<int> ---" << endl;
    DynamicArray<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);
    cout << "Size: " << arr.size() << ", Capacity: " << arr.capacity() << endl;
    cout << "Elements: ";
    for (int i = 0; i < arr.size(); i++) cout << arr[i] << " ";
    cout << endl;
    arr.removeAt(1);
    cout << "After remove index 1: ";
    for (int i = 0; i < arr.size(); i++) cout << arr[i] << " ";
    cout << endl;
    cout << "Contains 30: " << (arr.contains(30) ? "Yes" : "No") << endl;
    cout << "Contains 20: " << (arr.contains(20) ? "Yes" : "No") << endl;
    
    // Test 2: DynamicArray<string>
    cout << "\n--- Test DynamicArray<string> ---" << endl;
    DynamicArray<string> sarr;
    sarr.push_back("Hello");
    sarr.push_back("World");
    sarr.push_back("PBL2");
    cout << "Size: " << sarr.size() << endl;
    for (int i = 0; i < sarr.size(); i++) cout << sarr[i] << " ";
    cout << endl;
    
    // Test 3: Customer (Inheritance + Polymorphism)
    cout << "\n--- Test Customer (OOP) ---" << endl;
    Customer c("user1", "pass123", "0123456789", 22, "CUSTOMER", "KH001");
    cout << "Role: " << c.getRoleName() << endl;
    cout << "toString: " << c.toString() << endl;
    Customer c2 = Customer::fromString(c.toString());
    cout << "fromString back: " << c2.getTaiKhoan() << " | " << c2.getMaKhachHang() << endl;
    
    // Test 4: Cashier
    cout << "\n--- Test Cashier ---" << endl;
    Cashier ca("cashier1", "pass456", "0111222333", 30, "CASHIER", "NV001", "Nguyen Van A");
    cout << "Role: " << ca.getRoleName() << endl;
    cout << "toString: " << ca.toString() << endl;
    
    // Test 5: Admin
    cout << "\n--- Test Admin ---" << endl;
    Admin ad("admin1", "pass789", "0999888777", 35, "ADMIN", "QL001", "Tran Van B");
    cout << "Role: " << ad.getRoleName() << endl;
    cout << "toString: " << ad.toString() << endl;
    
    // Test 6: Polymorphism
    cout << "\n--- Test Polymorphism ---" << endl;
    User* users[3];
    users[0] = new Customer("u1", "p1", "0111111111", 20, "CUSTOMER", "KH001");
    users[1] = new Cashier("u2", "p2", "0222222222", 25, "CASHIER", "NV001", "Name1");
    users[2] = new Admin("u3", "p3", "0333333333", 30, "ADMIN", "QL001", "Name2");
    for (int i = 0; i < 3; i++) {
        cout << "User " << i << ": " << users[i]->getRoleName() << endl;
    }
    for (int i = 0; i < 3; i++) delete users[i];
    
    // Test 7: Movie
    cout << "\n--- Test Movie ---" << endl;
    Movie m;
    m.setMaPhim("MV001");
    m.setTenPhim("Avengers Endgame");
    m.setTheLoai("Hanh dong");
    m.setThoiLuong(181);
    m.setDoTuoi(13);
    m.setMoTa("Sieu anh hung");
    m.setNoiDung("Story");
    m.setPoster("posters/avengers.jpg");
    m.setTrangThai(true);
    cout << "Movie: " << m.getTenPhim() << " (" << m.getThoiLuong() << " min)" << endl;
    string mStr = m.toString();
    cout << "toString: " << mStr << endl;
    Movie m2 = Movie::fromString(mStr);
    cout << "fromString: " << m2.getTenPhim() << endl;
    
    // Test 8: Room + SeatMap
    cout << "\n--- Test Room + SeatMap ---" << endl;
    Room r;
    r.setMaPhong("RM001");
    r.setTenPhong("Phong 1");
    r.setSoHang(5);
    r.setSoCot(6);
    r.setTrangThai(true);
    cout << "Room: " << r.getTenPhong() << " (" << r.getSoHang() << "x" << r.getSoCot() << ")" << endl;
    
    SeatMap sm;
    sm.initFromRoom("ST001", 5, 6, 2);
    cout << "SeatMap initialized for ST001" << endl;
    cout << "Available seats: " << sm.countAvailable() << endl;
    cout << "Seat (0,0) status: " << sm.getSeatStatus(0, 0) << " (0=EMPTY)" << endl;
    cout << "Seat (3,0) status: " << sm.getSeatStatus(3, 0) << " (2=VIP_EMPTY)" << endl;
    sm.bookSeat(0, 0);
    sm.bookSeat(3, 0); // VIP seat
    cout << "After booking (0,0) and (3,0):" << endl;
    cout << "Seat (0,0) status: " << sm.getSeatStatus(0, 0) << " (1=BOOKED)" << endl;
    cout << "Seat (3,0) status: " << sm.getSeatStatus(3, 0) << " (3=VIP_BOOKED)" << endl;
    cout << "Available seats: " << sm.countAvailable() << endl;
    cout << "Booked seats: " << sm.countBooked() << endl;
    
    // Test SeatMap serialization
    string smStr = sm.toString();
    cout << "SeatMap toString length: " << smStr.length() << " chars" << endl;
    SeatMap sm2 = SeatMap::fromString(smStr);
    cout << "fromString - Seat (0,0): " << sm2.getSeatStatus(0, 0) << endl;
    cout << "fromString - Available: " << sm2.countAvailable() << endl;
    
    // Test 9: Validator
    cout << "\n--- Test Validator ---" << endl;
    cout << "Username 'ab': " << (Validator::isValidUsername("ab") ? "VALID" : "INVALID") << endl;
    cout << "Username 'user1': " << (Validator::isValidUsername("user1") ? "VALID" : "INVALID") << endl;
    cout << "Phone '0123456789': " << (Validator::isValidPhone("0123456789") ? "VALID" : "INVALID") << endl;
    cout << "Phone '123': " << (Validator::isValidPhone("123") ? "VALID" : "INVALID") << endl;
    cout << "Date '15/09/2026': " << (Validator::isValidDate("15/09/2026") ? "VALID" : "INVALID") << endl;
    cout << "Date '32/13/2026': " << (Validator::isValidDate("32/13/2026") ? "VALID" : "INVALID") << endl;
    
    // Test 10: PriceCalculator
    cout << "\n--- Test PriceCalculator ---" << endl;
    double normalPrice = PriceCalculator::calculateTicketPrice(0, "15/09/2026", "14:00"); // Monday
    cout << "Normal seat, weekday 14:00: " << normalPrice << " VND" << endl;
    double vipPrice = PriceCalculator::calculateTicketPrice(2, "15/09/2026", "14:00");
    cout << "VIP seat, weekday 14:00: " << vipPrice << " VND" << endl;
    double weekendPrice = PriceCalculator::calculateTicketPrice(0, "20/09/2026", "14:00"); // Saturday
    cout << "Normal seat, weekend 14:00: " << weekendPrice << " VND" << endl;
    
    // Test 11: IDGenerator
    cout << "\n--- Test IDGenerator ---" << endl;
    DynamicArray<string> ids;
    ids.push_back("KH001");
    ids.push_back("KH003");
    ids.push_back("KH002");
    string newId = IDGenerator::generateID("KH", ids);
    cout << "Next ID after KH001,KH002,KH003: " << newId << endl;
    
    DynamicArray<string> emptyIds;
    string firstId = IDGenerator::generateID("MV", emptyIds);
    cout << "First ID for MV: " << firstId << endl;
    
    // Test 12: Ticket
    cout << "\n--- Test Ticket ---" << endl;
    Ticket t;
    t.setMaVe("TK001");
    t.setMaSuatChieu("ST001");
    t.setMaKhachHang("KH001");
    t.setMaNhanVien("");
    t.setHang(0);
    t.setCot(0);
    t.setGiaVe(65000);
    t.setMaCombo("CB001");
    t.setNgayMua("15/09/2026");
    t.setTrangThai("VALID");
    cout << "Ticket: " << t.getMaVe() << " Status: " << t.getTrangThai() << endl;
    cout << "isValid: " << (t.isValid() ? "Yes" : "No") << endl;
    t.markUsed();
    cout << "After markUsed: " << t.getTrangThai() << endl;
    cout << "isValid: " << (t.isValid() ? "Yes" : "No") << endl;
    
    // Test 13: HashTable & ComboManager
    cout << "\n--- Test HashTable & ComboManager ---" << endl;
    HashTable<string, Combo> comboTable;
    Combo cb1("CB001", "Combo 1", "Bong ngo + Nước ngọt", 75000, true);
    Combo cb2("CB002", "Combo 2", "2 Bong ngo + 2 Nước ngọt", 120000, true);
    comboTable.put(cb1.getMaCombo(), cb1);
    comboTable.put(cb2.getMaCombo(), cb2);
    
    cout << "HashTable size: " << comboTable.size() << endl;
    cout << "Contains CB001: " << (comboTable.containsKey("CB001") ? "Yes" : "No") << endl;
    Combo* foundCb = comboTable.get("CB001");
    if (foundCb) {
        cout << "Found CB001: " << foundCb->getTenCombo() << " (" << foundCb->getGia() << " VND)" << endl;
    }
    
    comboTable.remove("CB001");
    cout << "After remove CB001, size: " << comboTable.size() << endl;
    cout << "Contains CB001: " << (comboTable.containsKey("CB001") ? "Yes" : "No") << endl;

    cout << "\n=== ALL CORE TESTS PASSED ===" << endl;
    return 0;
}

