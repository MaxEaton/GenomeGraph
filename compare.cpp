#include <iostream>
#include <fstream>

int main() {
    std::string input1 = "dir4.txt";
    std::string input2 = "trash.txt";

    std::ifstream inFile1(input1);
    std::string line1;
    while(getline(inFile1, line1)) {
        std::ifstream inFile2(input2);
        std::string line2;
        while(getline(inFile2, line2)) {
            if (line1 == line2) goto skip;
        }
        inFile2.close();
        std::cout << line1 << std::endl;
        skip:;
    }
    inFile1.close();

    return 0;
}