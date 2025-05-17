#include "livestream.hpp"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std;

const int MAX_SPECTATORS = 100;
Spectator priorityQueue[MAX_SPECTATORS];
int pqSize = 0;

void loadSpectators(const char* filename) {
    ifstream fin(filename);
    if (!fin) return;

    string line;
    getline(fin, line); // skip header

    while (getline(fin, line)) {
        stringstream ss(line);
        string nameStr, prioStr;
        getline(ss, nameStr, ',');
        getline(ss, prioStr, ',');

        Spectator s;
        strncpy(s.name, nameStr.c_str(), sizeof(s.name));
        s.priority = stoi(prioStr);

        addSpectator(s.name, s.priority);
    }

    fin.close();
    cout << "Loaded spectators from file.\n";
}

void addSpectator(const char* name, int priority) {
    if (pqSize >= MAX_SPECTATORS) {
        cout << "Queue full.\n";
        return;
    }

    Spectator s;
    strcpy(s.name, name);
    s.priority = priority;

    // Insert sorted by priority
    int i = pqSize - 1;
    while (i >= 0 && priorityQueue[i].priority < s.priority) {
        priorityQueue[i + 1] = priorityQueue[i];
        i--;
    }
    priorityQueue[i + 1] = s;
    pqSize++;

    saveToBufferFile(name, priority);  // ✅ Save to buffer file
}
Spectator getNextSpectator() {
    return priorityQueue[0];
}

void removeNextSpectator() {
    for (int i = 1; i < pqSize; i++) {
        priorityQueue[i - 1] = priorityQueue[i];
    }
    pqSize--;
}

void displayPriorityQueue() {
    cout << "\n[Priority Queue]:\n";
    for (int i = 0; i < pqSize; i++) {
        cout << i + 1 << ". " << priorityQueue[i].name << " (Priority: " << priorityQueue[i].priority << ")\n";
    }
}
void saveSpectators(const char* filename) {
    ofstream fout(filename);
    if (!fout) {
        cout << "Error opening file for writing.\n";
        return;
    }

    fout << "Name,Priority\n";
    for (int i = 0; i < pqSize; i++) {
        fout << priorityQueue[i].name << "," << priorityQueue[i].priority << "\n";
    }

    fout.close();
    cout << "Spectators saved to file: " << filename << "\n";
}
void saveToBufferFile(const char* name, int priority) {
    ofstream fout("buffer.csv", ios::app); // append mode
    if (!fout.is_open()) {
        cerr << "[ERROR] Could not open buffer.csv for writing.\n";
        return;
    }
    fout << name << "," << priority << "\n";
    fout.close();
}


