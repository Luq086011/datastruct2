#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include "seatassignment.hpp"
using namespace std;

extern Spectator getNextSpectator();
extern void removeNextSpectator();

const int MAX_SEATS = 5;
Seat seatList[MAX_SEATS];

void loadSeatAssignmentsFromFile() {
    ifstream fin("spectators.csv");
    if (!fin.is_open()) {
        cerr << "[INFO] No previous spectators.csv found. Skipping load.\n";
        return;
    }

    string line;
    getline(fin, line); // Skip header

    while (getline(fin, line)) {
        stringstream ss(line);
        string seatStr, name, priorityStr;

        getline(ss, seatStr, ',');
        getline(ss, name, ',');
        getline(ss, priorityStr, ',');

        // Skip malformed or empty fields
        if (seatStr.empty() || name.empty() || priorityStr.empty()) {
            cerr << "[WARN] Malformed line in CSV: " << line << endl;
            continue;
        }

        int seatIndex, priority;

        try {
            seatIndex = stoi(seatStr) - 1;
            priority = stoi(priorityStr);
        } catch (const invalid_argument& e) {
            cerr << "[ERROR] Non-numeric value in CSV: " << line << endl;
            continue;
        } catch (const out_of_range& e) {
            cerr << "[ERROR] Number out of range in CSV: " << line << endl;
            continue;
        }

        // Validate seat index range
        if (seatIndex < 0 || seatIndex >= MAX_SEATS) {
            cerr << "[ERROR] Seat index out of range in CSV: " << seatStr << endl;
            continue;
        }

        // Assign to seat list
        seatList[seatIndex].occupied = true;
        strcpy(seatList[seatIndex].occupant.name, name.c_str());
        seatList[seatIndex].occupant.priority = priority;
    }

    fin.close();
    cout << "[INFO] Loaded seat data from spectators.csv\n";
}

bool isFull() {
    for (int i = 0; i < MAX_SEATS; i++) {
        if (!seatList[i].occupied)
            return false;
    }
    return true;
}

bool isEmpty() {
    for (int i = 0; i < MAX_SEATS; i++) {
        if (seatList[i].occupied)
            return false;
    }
    return true;
}

void saveSeat() {
    ofstream fout("spectators.csv");
    if (!fout.is_open()) {
        cerr << "[ERROR] Could not write spectators.csv\n";
        return;
    }

    fout << "Seat,Name,Priority\n";
    for (int i = 0; i < MAX_SEATS; i++) {
        if (seatList[i].occupied) {
            fout << (i + 1) << "," << seatList[i].occupant.name << "," << seatList[i].occupant.priority << "\n";
        }
    }
    fout.close();
}

void assignSeat(Spectator s) {
    for (int i = 0; i < MAX_SEATS; i++) {
        if (!seatList[i].occupied) {
            seatList[i].occupied = true;
            seatList[i].occupant = s;
            cout << "[INFO] Assigned " << s.name << " (Priority " << s.priority << ") to Seat " << (i + 1) << endl;
            saveSeat();
            return;
        }
    }
    cout << "[WARN] All seats are full, cannot assign.\n";
}

void releaseSeat() {
    for (int i = 0; i < MAX_SEATS; i++) {
        if (seatList[i].occupied) {
            cout << "[INFO] Releasing " << seatList[i].occupant.name << " from Seat " << (i + 1) << endl;
            seatList[i].occupied = false;
            saveSeat(); // update CSV
            return;
        }
    }

    cout << "[WARN] No occupied seats.\n";
}


void displaySeats() {
    ifstream fin("spectators.csv");
    if (!fin.is_open()) {
        cerr << "[ERROR] Could not open spectators.csv\n";
        return;
    }

    string line;
    getline(fin, line); // Skip header

    cout << "\n[Seats from CSV]\n";
    while (getline(fin, line)) {
        cout << line << "\n";
    }

    fin.close();
}

