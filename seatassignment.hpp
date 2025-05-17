#ifndef SEATASSIGNMENT_HPP
#define SEATASSIGNMENT_HPP

#include "livestream.hpp"

struct Seat {
    bool occupied;
    Spectator occupant;
};

void loadSeat();
void assignSeat(Spectator s);
void releaseSeat();
void displaySeats();
void saveSeat();
bool isFull();

#endif
