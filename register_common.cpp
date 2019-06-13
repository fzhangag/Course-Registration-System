#include "register_common.h"

#include <string>

using namespace std;

int ELFHash_c(const char *str, int m) {
    unsigned int hash = 0, g = 0;
    while(*str) {
        hash = (hash << 4) + *str++;
        if((g = hash & 0xF000000L))
            hash ^= g >> 24;
    }
    return ((int)(hash & 0x7FFFFFFFL)) % m;
}

int ELFHash(const string& str, int m) {
    return ELFHash_c(str.c_str(), m);
}