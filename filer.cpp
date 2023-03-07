#include "filer.h"


// TODO read() to order 1,2 -> 1,3 -> 2,3 -> 1,4
//                  not 1,2 -> 1,3 -> 1,4 -> 2,3


void print(std::bitset<biggest> sequence) {
    for (int i=0; i<biggest; i++) {
        if (sequence[i]) std::cout << i+1 << " ";
    }
    std::cout << std::endl;
}

Gene* Gene::addLL(Gene* curr) {
    if (!curr) return this;
    for (int i=biggest-1; i>=0; i--) {
        if (sequence[i] && !curr->sequence[i]) {
            curr->next = addLL(curr->next);
            return curr;
        } else if (!sequence[i] && curr->sequence[i]) {
            next = curr;
            return this;
        }
    }
}

bool Gene::findEnd(Gene* parent, int i) {
    if (this == parent) return false;
    print(parent->sequence);
    bool end = true;
    for (Gene* child : parent->children) {
        if ((sequence & child->sequence) == child->sequence) {
            end = false;
            std::cout << "    ";
            print(child->sequence);
            // visiteds |= masks[i] & child->sequence;
            // in lieu of guardian
            for (Gene* guard : guardian) {
                if ((guard->sequence & child->sequence) == child->sequence) goto skip;
            }
            if (findEnd(child, i)) {
                child->children.push_back(this);
                this->guardian.push_back(child);
            }
            skip:;
        }
    }
    return end;
}

void Gene::insert() {
    std::cout << "new" << std::endl;
    print(sequence);
    for (int i=0; i<biggest; i++) {
        if (sequence[i]) {
            if (findEnd(heads[i], i)) {
                heads[i]->children.push_back(this);
                this->guardian.push_back(heads[i]);
            }
        }
    }
}

void Gene::traverse(std::ofstream &outFile) {
    done = true;
    for (Gene* child : children) {
        record(sequence, child->sequence, outFile);
        if (!child->done) {
            child->traverse(outFile);
        }
    }
}

std::array<Gene*, biggest> Gene::heads;
std::array<std::bitset<biggest>, biggest> Gene::masks;

std::bitset<biggest> read(std::array<Gene*, longest> &sequences, std::string input) {
    std::bitset<biggest> sources;
    Gene* gene;
    std::ifstream inFile(input);
    std::string line;
    while(getline(inFile, line)) {
        std::stringstream lineStream(line);
        std::string num;
        std::bitset<biggest> sequence;
        while (lineStream >> num) {
            sequence.flip(std::stoi(num)-1);
        }
        gene = new Gene(sequence);
//        print(sequence);
        if (sequence.count()-1) sequences[sequence.count()-1] = gene->addLL(sequences[sequence.count()-1]); // TODO wasting 0th index
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

int main(int argc, char *argv[]) {
    std::string input  = argv[1];
    std::string output = argv[2];

    std::array<Gene*, longest> sequences;
    for (Gene*& sequence : sequences) {sequence = nullptr;}
    std::bitset<biggest> sources = read(sequences, input);

    std::bitset<biggest> mask;
    for (int j=0; j<biggest; j++) {
        mask.set(j);
        Gene::masks[j] = mask;
    } // TODO make constexpr

    Gene* gene;
    std::bitset<biggest> headSeq;
    for (int j=0; j<biggest; j++) {
        headSeq.reset();
        headSeq.set(j);
        gene = new Gene(headSeq);
        Gene::heads[j] = gene;
    } // TODO make constexpr
    gene = sequences[1];
    while (gene) {
        for (int k=0; k<biggest; k++) {
            if (gene->sequence[k]) Gene::heads[k]->children.push_back(gene);
        }
        gene = gene->next;
    }
    std::bitset<biggest> guy;
    guy.set(0);
    guy.set(1);
    guy.set(4);
    guy.set(467);
    for (int i=2; i<4; i++) {
        std::cout << i << std::endl;
        gene = sequences[i];
        while (gene) {
            gene->insert();
            if (gene->sequence == guy) break;
            gene = gene->next;
        }
    }
//    for (int i=2; i<longest; i++) {
//        std::cout << i << std::endl;
//        gene = sequences[i];
//        while (gene) {
//            gene->insert();
//            gene = gene->next;
//        }
//    }

    std::ofstream outFile(output);
    for (Gene* head : Gene::heads) {
        if ((head->sequence & sources) == head->sequence) head->traverse(outFile);
        else for (Gene* child : head->children) if (!child->done) child->traverse(outFile);
    }
    outFile.close();

    return 0;
}
