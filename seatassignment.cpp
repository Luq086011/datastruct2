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

Seat seatList[MAX_SEATS];

void initSeatLabels() {
    int index = 0;
    for (char row = 'A'; row <= 'F'; row++) {
        for (int num = 1; num <= 10; num++) {
            snprintf(seatList[index].label, sizeof(seatList[index].label), "%c%d", row, num);
            seatList[index].occupied = false;  // just in case
            index++;
        }
    }
}
void loadSeat() {
    ifstream fin("spectators.csv");
    if (!fin.is_open()) {
        cerr << "[INFO] No previous spectators.csv found. Skipping load.\n";
        return;
    }

    string line;
    getline(fin, line); // skip header

    while (getline(fin, line)) {
        if (line.empty() || line.find_first_not_of(" \t\r\n") == string::npos) {
            continue;
        }

        stringstream ss(line);
        string seatLabel, name, priorityStr;

        getline(ss, seatLabel, ',');
        getline(ss, name, ',');
        getline(ss, priorityStr, ',');

        if (seatLabel.empty() || name.empty() || priorityStr.empty()) {
            cerr << "[WARN] Malformed line: " << line << endl;
            continue;
        }

        try {
            int priority = stoi(priorityStr);

            // Find seat by label
            bool found = false;
            for (int i = 0; i < MAX_SEATS; i++) {
                if (seatLabel == seatList[i].label) {
                    seatList[i].occupied = true;
                    strcpy(seatList[i].occupant.name, name.c_str());
                    seatList[i].occupant.priority = priority;
                    found = true;
                    break;
                }
            }

            if (!found) {
                cerr << "[ERROR] Seat label not recognized: " << seatLabel << endl;
            }

        } catch (const exception& e) {
            cerr << "[ERROR] Failed to load row: " << line << " | Reason: " << e.what() << endl;
        }
    }

    fin.close();
    cout << "[INFO] Loaded seat assignments from spectators.csv\n";
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
            fout << seatList[i].label << "," << seatList[i].occupant.name << "," << seatList[i].occupant.priority << "\n";
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

