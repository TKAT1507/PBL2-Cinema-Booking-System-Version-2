#ifndef SYSTEMDATE_H
#define SYSTEMDATE_H

#include <QDate>
#include <QString>
#include <QList>

class SystemDate {
public:
    // Trả về ngày hiện tại của hệ thống (mặc định là ngày cố định tượng trưng: 28/10/2026 - Thứ Tư)
    static QDate getCurrentDate() {
        if (s_useRealDate) {
            return QDate::currentDate();
        }
        return s_fixedDate;
    }

    // Thay đổi ngày tượng trưng nếu cần thử nghiệm
    static void setFixedDate(const QDate& date) {
        s_fixedDate = date;
    }

    // Bật/tắt chế độ sử dụng ngày thực tế của máy tính
    static void setUseRealDate(bool useReal) {
        s_useRealDate = useReal;
    }

    static bool isUsingRealDate() {
        return s_useRealDate;
    }

    // Tên thứ bằng tiếng Việt
    static QString getDayOfWeekVN(const QDate& date) {
        switch (date.dayOfWeek()) {
            case 1: return "Thứ Hai";
            case 2: return "Thứ Ba";
            case 3: return "Thứ Tư";
            case 4: return "Thứ Năm";
            case 5: return "Thứ Sáu";
            case 6: return "Thứ Bảy";
            case 7: return "Chủ Nhật";
            default: return "";
        }
    }

    // Danh sách 7 ngày liên tiếp tính từ ngày hiện tại
    static QList<QDate> getUpcoming7Days() {
        QList<QDate> list;
        QDate today = getCurrentDate();
        for (int i = 0; i < 7; ++i) {
            list.append(today.addDays(i));
        }
        return list;
    }

private:
    // Ngày cố định mặc định: 28/10/2026 (Thứ Tư)
    inline static QDate s_fixedDate = QDate(2026, 10, 28);
    inline static bool s_useRealDate = false;
};

#endif // SYSTEMDATE_H
