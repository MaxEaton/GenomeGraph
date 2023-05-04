#include <fstream>
#include <sstream>
#include <bitset>
#include <vector>
#include <array>

#include <iostream>

#define longest 43
#define biggest 748

struct Node {
    std::bitset<biggest> sequence;
    std::vector<Node*> children;
};

struct Graph {
    std::array<std::vector<Node*>, longest> nodes;
};

Node* findNode(Graph &g, std::string str) {
    std::stringstream stream(str);
    std::string num;
    std::bitset<biggest> sequence;
    while (getline(stream, num, ',')) {
        sequence.flip(std::stoi(num)-1);
    }
    Node* node;
    for (Node* n : g.nodes[sequence.count()-1]) {
        if (n->sequence == sequence) {
            node = n;
            goto skip;
        }
    }
    node = new Node;
    node->sequence = sequence;
    g.nodes[sequence.count()-1].push_back(node);
    skip:;
    return node;
}

void read(Graph &g, std::string input) {
    std::ifstream inFile(input);
    std::string line;
    while(getline(inFile, line)) {
        findNode(g, line.substr(0, line.find("->")))->children.push_back(findNode(g, line.substr(line.find("->")+2)));
    }
    inFile.close();
}

int main(int argc, char *argv[]) {
    std::string input  =  argv[1];

    Graph g;
    read(g, input);

    for (int i=0; i<longest; i++) {
        std::cout << g.nodes[i].size() << std::endl;
    }

    return 0;
}
