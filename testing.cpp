#include <vector>
#include <bitset>
#include <iostream>

int main() {
    std::bitset<3> a;
    a.set(0);
    a.set(1);

    std::bitset<3> b;
    b.set(0);

    std::cout << (a > b) << std::endl;

    return 0;
}