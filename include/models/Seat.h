#ifndef SEAT_H
#define SEAT_H

struct Seat {
    int hang;
    int cot;
    int loaiGhe; // 0 = NORMAL, 1 = VIP

    Seat() : hang(0), cot(0), loaiGhe(0) {}
    Seat(int hang, int cot, int loaiGhe) : hang(hang), cot(cot), loaiGhe(loaiGhe) {}
};

#endif // SEAT_H
