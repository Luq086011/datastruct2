#include <iostream>
#include "livestream.hpp"
#include "seatassignment.hpp"
#include "overflow.hpp"
using namespace std;

int main() {
    int choice;
    char name[50];
    int priority;
    initSeatLabels();
    loadSeat(); 
    loadOverflow();
    loadSpectators("spectators.csv");

    while (true) {
        cout << "\n=== Spectator System ===\n";
        cout << "1. Register Spectator\n";
        cout << "2. View Priority Queue\n";
        cout << "3. Release Seat\n";
        cout << "4. View Seats\n";
        cout << "5. View Overflow Queue\n";
        cout << "0. Exit\n";
        cout << "Select option: ";
        cin >> choice;
        cin.ignore();
        if (cin.fail()) {
            cin.clear(); // clear the error flag
            cin.ignore(1000, '\n'); // discard invalid input
            cerr << "[ERROR] Invalid input. Please enter a number.\n";
            continue; // back to menu
        }
        switch (choice) {
        case 1:
            cout << "Enter name: ";
            cin.getline(name, 50);
            cout << "Enter priority Level(2 = VIP, 1 = Influencer, 0 = Normal): ";
            cin >> priority;
            cin.ignore();

            addSpectator(name, priority);
            break;
        
        case 2:
            if (pqSize == 0) {
                //Check if there's any spectator in the queue
                cout << "No spectators in priority queue.\n";
                break;
            }
            displayPriorityQueue();
            cout <<"Do you want to continue assigining the seats?"<<endl;
            cout << "Enter Y to continue or N to exit: ";
            char continueAssign;
            cin >> continueAssign;
            cin.ignore();
            if (continueAssign == 'N' || continueAssign == 'n') {
                cout << "Exiting seat assignment.\n";
                break;
            }
            while (pqSize > 0) {
            Spectator next = getNextSpectator();

            if (!isFull()) {
                assignSeat(next); // This will save to spectators.csv
            } else {
                addOverflow(next);
            }
            removeNextSpectator();
            }

            cout << "[INFO] All possible spectators have been assigned seats.\n";
            break;
        case 3:
            releaseSeat();
            pullOverflow();
            break;
        case 4:
            displaySeats();
            break;
        case 5:
            displayOverflow();
            break;
        case 0:
            return 0;
        default:
            cout << "Invalid option.\n";
        }
    }
}
