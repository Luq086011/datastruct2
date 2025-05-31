#include "overflow.hpp"
#include "seatassignment.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
using namespace std;

const int OVERFLOW_LIMIT = 10;
Spectator overflowQueue[OVERFLOW_LIMIT];
int overflowSize = 0;

void loadOverflow() {
    ifstream fin("overflow.csv");
    if (!fin.is_open()) {
        cerr << "[INFO] No existing overflow.csv found. Skipping load.\n";
        return;
    }

    string line;
    getline(fin, line); // Skip header

    overflowSize = 0; // reset to avoid duplication

    while (getline(fin, line)) {
        if (overflowSize >= OVERFLOW_LIMIT) break;

        stringstream ss(line);
        string name, prioStr;
        getline(ss, name, ',');
        getline(ss, prioStr, ',');

        if (name.empty() || prioStr.empty()) {
            cerr << "[WARN] Skipping invalid line in overflow.csv: " << line << endl;
            continue;
        }

        try {
            int priority = stoi(prioStr);

            Spectator s;
            strcpy(s.name, name.c_str());
            s.priority = priority;

            overflowQueue[overflowSize++] = s;
        } catch (const invalid_argument& e) {
            cerr << "[ERROR] Invalid priority in line: " << line << "\n";
            continue;
        } catch (const out_of_range& e) {
            cerr << "[ERROR] Priority out of range in line: " << line << "\n";
            continue;
        }
    }

    fin.close();
    cout << "[INFO] Loaded overflow queue from file. Size: " << overflowSize << "\n";
}


void addOverflow(Spectator s) {
    if (overflowSize >= OVERFLOW_LIMIT) {
        cout << "Overflow queue is full!\n";
        return;
    }
    overflowQueue[overflowSize++] = s;
    cout << s.name << " added to overflow queue.\n";

    saveOverflow();
}

void saveOverflow() {
    ofstream fout("overflow.csv");
    if (!fout.is_open()) {
        cerr << "[ERROR] Could not write to overflow.csv\n";
        return;
    }

    fout << "Name,Priority\n";
    for (int i = 0; i < overflowSize; i++) {
        fout << overflowQueue[i].name << "," << overflowQueue[i].priority << "\n";
    }

    fout.close();
}
void pullOverflow() {
    if (overflowSize == 0) {
        cout << "[INFO] No spectators in overflow queue.\n";
        return;
    }

    if (!isFull()) {
        Spectator next = overflowQueue[0];

        // Shift overflow queue
        for (int i = 1; i < overflowSize; i++) {
            overflowQueue[i - 1] = overflowQueue[i];
        }
        overflowSize--;

        assignSeat(next); // will update spectators.csv
        saveOverflow(); // update overflow.csv
        cout << "[INFO] Pulled " << next.name << " from overflow and assigned to seat.\n";
    } else {
        cout << "[WARN] Still no available seats.\n";
    }
}
void displayOverflow() {
    ifstream fin("overflow.csv");
    if (!fin.is_open()) {
        cerr << "[ERROR] Could not open overflow.csv\n";
        return;
    }

    string line;
    getline(fin, line); // skip header

    cout << "\n[Overflow Queue]\n";
    while (getline(fin, line)) {
        cout << line << "\n";
    }

    fin.close();
}
