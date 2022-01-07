#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <bitset>
#include <stdint.h>
#include <string.h>
#include <cmath>
#include <sstream>

#define ulong unsigned long
#define ull unsigned long long

class MD4 {
private:
    int width = 32;
    ulong mask = 0xFFFFFFFF;
    std::vector<ulong> words = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476 };

    ulong F(ulong x, ulong y, ulong z);
    ulong G(ulong x, ulong y, ulong z);
    ulong H(ulong x, ulong y, ulong z);
    ulong leftRotate(unsigned long long val, int n);
    std::vector<ulong> resizeBlock(std::vector<ulong> in);
    void process(std::vector<std::vector<ulong>> chunks);
    std::vector<unsigned char> getHex(ulong in);

public:
    MD4(std::string input);
    std::string hexdigest();
};