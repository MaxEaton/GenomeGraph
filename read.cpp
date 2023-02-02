#include <fstream>
#include <sstream>
#include <bitset>
#include <vector>
#include <array>

#define input  "seq4.txt"
#define output "dir4.txt"
#define longest 43
#define biggest 748

void read(std::array<std::vector<std::bitset<biggest>>, longest> &sequences) {
    std::ifstream inFile(input);
    std::string line;
    while(getline(inFile, line)) {
        std::stringstream lineStream(line);
        std::string num;
        std::bitset<biggest> sequence;
        int size = 0;
        while (lineStream >> num) {
            sequence.flip(std::stoi(num)-1);
            size += 1;
        }
        sequences[size-1].push_back(sequence);
    }
    inFile.close();
}

void record(std::array<std::vector<std::bitset<biggest>>, longest> &sequences, std::ofstream &outFile, int i, int j, int m, int n) {
    bool comma = false;
    for (int x=0; x<biggest; x++) {
        if (sequences[i][j][x]) {
            if (comma) outFile << ",";
            outFile << x+1;
            comma = true;
        }
    }
    outFile << "->";
    comma = false;
    for (int x=0; x<biggest; x++) {
        if (sequences[m][n][x]) {
            if (comma) outFile << ",";
            outFile << x+1;
            comma = true;
        }
    }
    outFile << std::endl;
}

int main() {
    std::array<std::vector<std::bitset<biggest>>, longest> sequences;
    read(sequences);

    std::ofstream outFile(output);
    for (int i=0; i<longest-1; i++) {
        for (int j=0; j<sequences[i].size(); j++) {
            std::vector<std::bitset<biggest>> visited;
            for (int m=i+1; m<longest-2; m++) {
                for (int n=0; n<sequences[m].size(); n++) {
                    if ((sequences[i][j] ^ sequences[m][n]).count() == m-i) {
                        for (std::bitset<biggest> v : visited) {
                            if ((v & sequences[m][n]) == v) goto skip;
                        }
                        visited.push_back(sequences[m][n]);
                        record(sequences, outFile, i, j, m, n);
                        skip:;
                    }
                }
            }
        }
    }
    outFile.close();

    return 0;
}
