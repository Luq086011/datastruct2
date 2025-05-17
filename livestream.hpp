#ifndef LIVESTREAM_HPP
#define LIVESTREAM_HPP

struct Spectator {
    char name[50];
    int priority; // 2 = VIP, 1 = Influencer, 0 = Regular
};

// Priority queue functions
void addSpectator(const char* name, int priority);
Spectator getNextSpectator();
void removeNextSpectator();
void displayPriorityQueue();
void saveSpectators(const char* filename);
void loadSpectators(const char* filename);
void saveToBufferFile(const char* name, int priority);

extern int pqSize;

#endif
