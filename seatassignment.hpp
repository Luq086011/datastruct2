#ifndef SEATASSIGNMENT_HPP
#define SEATASSIGNMENT_HPP

#include "livestream.hpp"

struct Seat {
    bool occupied;
    Spectator occupant;
    char label[4];
};

const int ROWS = 6;
const int COLS = 10;
const int MAX_SEATS = ROWS * COLS;

extern Seat seatList[MAX_SEATS];
void loadSeat();
void assignSeat(Spectator s);
void releaseSeat();
void displaySeats();
void saveSeat();
bool isFull();
void initSeatLabels();

#endif
