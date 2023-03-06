#include <fstream>
#include <sstream>
#include <bitset>
#include <vector>
#include <array>
#include <iostream>

#define longest 43
#define biggest 748

std::bitset<biggest> read(std::array<std::vector<std::bitset<biggest>>, longest> &sequences, std::string input) {
    std::bitset<biggest> sources;
    std::ifstream inFile(input);
    std::string line;
    while(getline(inFile, line)) {
        std::stringstream lineStream(line);
        std::string num;
        std::bitset<biggest> sequence;
        while (lineStream >> num) {
            sequence.flip(std::stoi(num)-1);
        }
        if (sequence.count()-1) sequences[sequence.count()-1].push_back(sequence); // wasting 0th index
        else sources |= sequence;
    }
    inFile.close();
    return sources;
}

void record(std::bitset<biggest> s, std::bitset<biggest> t, std::ofstream &outFile) {
    bool comma = false;
    for (int x=0; x<biggest; x++) {
        if (s[x]) {
            if (comma) outFile << ",";
            outFile << x+1;
            comma = true;
        }
    }
    outFile << "->";
    comma = false;
    for (int x=0; x<biggest; x++) {
        if (t[x]) {
            if (comma) outFile << ",";
            outFile << x+1;
            comma = true;
        }
    }
    outFile << std::endl;
}

void print(std::bitset<biggest> sequence) {
    for (int i=0; i<biggest; i++) {
        if (sequence[i]) std::cout << i+1 << ",";
    }
    std::cout << std::endl;
}

struct Gene {
    static std::array<Gene*, biggest> heads;

    std::bitset<biggest> sequence;
    std::bitset<biggest> visiteds;
    std::vector<Gene*>   children;
    std::vector<Gene*>   guardian;

    Gene(std::bitset<biggest> _sequence) {
        sequence = _sequence;
    }

    bool findEnd(Gene* parent) {
        if (this == parent) return false;
        bool end = true;
        for (Gene* child : parent->children) {
            if ((sequence & child->sequence) == child->sequence) {
                for (Gene* guard : guardian) {
                    if ((guard->sequence & child->sequence) == child->sequence) goto skip;
                }
//                visiteds |= child->sequence;
                if (findEnd(child)) {
                    child->children.push_back(this);
                    this->guardian.push_back(child);
                }
                end = false;
                skip:;
            }
        }

        return end;
    }

    void insert() {
        for (int i=0; i<biggest; i++) {
            if (sequence[i] && !visiteds[i]) {
                visiteds.set(i);
                if (findEnd(heads[i])) {
                    heads[i]->children.push_back(this);
                    this->guardian.push_back(heads[i]);
                }
            }
        }
    }

    void traverse(std::ofstream &outFile) {
        visiteds.set(0);
        for (Gene* child : children) {
            record(sequence, child->sequence, outFile);
            if (!child->visiteds[0]) {
                child->traverse(outFile);
            }
        }
    }
};

std::array<Gene*, biggest> Gene::heads;

int main(int argc, char *argv[]) {
    std::string input  = argv[1];
    std::string output = argv[2];

    std::array<std::vector<std::bitset<biggest>>, longest> sequences;
    std::bitset<biggest> sources = read(sequences, input);

    Gene* gene;
    std::bitset<biggest> headSeq;
    for (int j=0; j<biggest; j++) {
        headSeq.reset();
        headSeq.set(j);
        gene = new Gene(headSeq);
        Gene::heads[j] = gene;
    }
    for (int j=0; j<sequences[1].size(); j++) {
        gene = new Gene(sequences[1][j]);
        for (int k=0; k<biggest; k++) {
            if (sequences[1][j][k]) Gene::heads[k]->children.push_back(gene);
        }
    }
    for (int i=2; i<longest; i++) {
        std::cout << i << ": " << sequences[i].size() << std::endl;
        for (int j=0; j<sequences[i].size(); j++) {
            gene = new Gene(sequences[i][j]);
            gene->insert();
            gene->visiteds.reset();
        }
    }

    std::ofstream outFile(output);
    for (Gene* head : Gene::heads) {
        if ((head->sequence & sources) == head->sequence) head->traverse(outFile);
        else {
            for (Gene* child : head->children) {
                if (!child->visiteds[0]) child->traverse(outFile);
            }
        }
    }
    outFile.close();

    return 0;
}
