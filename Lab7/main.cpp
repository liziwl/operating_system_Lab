#include <iostream>
#include <sstream>
#include <cstdlib>
#include <stdio.h>

#define CACHE_SIZE 64

std::string STREND("END");
std::string buffer;
bool valid[CACHE_SIZE];
unsigned long index2tag[CACHE_SIZE];
int i;
int total;
int hit;

bool isHex(std::string const &s) {
    return s.find_first_not_of("0123456789abcdefABCDEF") == std::string::npos;
}

int main() {
    total = 0;
    hit = 0;
    for (i = 0; i < CACHE_SIZE; i++) {
        valid[i] = false;
        index2tag[i] = 0;
    }
    while (true) {
        std::cin >> buffer;
        if (STREND.compare(buffer) == 0) {
            printf("Hit ratio = %.2f%%\n", (hit * 100.0 / total));
            break;
        }
        if (!isHex(buffer)) {
//            puts("Not Hex");
            continue;
        }
//        std::cout << "read: " << buffer << std::endl;
        unsigned long value = strtoul(buffer.c_str(), NULL, 16);
        unsigned long offset = value & 0xF;
        unsigned long index = (value >> 4) & 0x3F;
        unsigned long tag = value >> 10;
//        std::cout << tag << " " << index << " " << offset << std::endl;
        if (valid[index]) {
            if (index2tag[index] == tag) {
                std::cout << "Hit" << std::endl;
                hit++;
            } else {
                index2tag[index] = tag;
                std::cout << "Miss" << std::endl;
            }
        } else {
            valid[index] = true;
            index2tag[index] = tag;
            std::cout << "Miss" << std::endl;
        }
        total++;
    }
    return 0;
}