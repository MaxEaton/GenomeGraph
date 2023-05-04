#include <fstream>
#include <sstream>
#include <bitset>
#include <vector>
#include <array>

#define longest 43
#define biggest 748

#include <iostream>
#include <chrono>
#include <time.h>

int read(std::array<std::vector<std::bitset<biggest>>, longest> &sequences, std::string input, int randPercent) {
    std::ifstream inFile(input);
    std::string line;
    srand(time(0));
    int count = 0;
    while(getline(inFile, line)) {
        if (rand() % 100000 < randPercent*1000) {
            count += 1;
            std::stringstream lineStream(line);
            std::string num;
            std::bitset<biggest> sequence;
            while (lineStream >> num) {
                sequence.flip(std::stoi(num)-1);
            }
            sequences[sequence.count()-1].push_back(sequence);
        }
    }
    inFile.close();
    return count;
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

int main(int argc, char *argv[]) {
    std::string input  = argv[1];
    std::string output = argv[2];
    int randInput = std::stoi(argv[3]);

    std::array<std::vector<std::bitset<biggest>>, longest> sequences;
    int count = read(sequences, input, randInput);

    auto start = std::chrono::high_resolution_clock::now();

    /* sequence2
    std::ofstream outFile(output);
    for (int i=0; i<longest-1; i++) {
        for (int j=0; j<sequences[i].size(); j++) {
            std::bitset<biggest> prune1;
            for (int n=0; n<sequences[i+1].size(); n++) {
                if ((sequences[i][j] ^ sequences[i+1][n]).count() == 1) {
                    prune1 |= (sequences[i][j] ^ sequences[i+1][n]);
                    record(sequences, outFile, i, j, i+1, n);
                }
            }
            std::vector<std::bitset<biggest>> visited;
            for (int m=i+2; m<longest; m++) {
                for (int n=0; n<sequences[m].size(); n++) {
                    if ((sequences[i][j] ^ sequences[m][n]).count() == m-i && !(prune1 & sequences[m][n]).count()) {
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
    */

    std::ofstream outFile(output);
    for (int i=0; i<longest-1; i++) {
        for (int j=0; j<sequences[i].size(); j++) {
            std::vector<std::bitset<biggest>> visited;
            for (int m=i+1; m<longest; m++) {
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

    std::cout << count << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;

    return 0;
}
