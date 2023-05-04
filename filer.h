#ifndef FILER_H
#define FILER_H

#include <fstream>
#include <sstream>
#include <bitset>
#include <vector>
#include <array>
#include <iostream>

#define longest 43
#define biggest 748

struct Gene {
    static std::array<Gene*, biggest> heads;
    static std::array<std::bitset<biggest>, biggest> masks;

    std::bitset<biggest> sequence;
    std::vector<Gene*>   children;
    std::vector<Gene*>   guardian;
    bool  done;
    Gene* next = nullptr;

    Gene(std::bitset<biggest> _sequence) {sequence = _sequence;}

    Gene* addLL(Gene* curr);

    bool findEnd(Gene* parent, int i);
    void insert();
    void traverse(std::ofstream &outFile);
};

std::bitset<biggest> read(std::array<Gene*, longest> &sequences, std::string input);
void record(std::bitset<biggest> s, std::bitset<biggest> t, std::ofstream &outFile);

#endif //FILER_H
