#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <bitset>

#define ulong unsigned long
#define SHA1_MOD(x) ((x) & 0xFFFFFFFF)

class SHA1 {
    private:
        ulong h0 = 0x67452301;
        ulong h1 = 0xEFCDAB89;
        ulong h2 = 0x98BADCFE;
        ulong h3 = 0x10325476;
        ulong h4 = 0xC3D2E1F0;

        ulong f(ulong t, ulong B, ulong C, ulong D);
        ulong S(ulong n, ulong X);
        ulong K(ulong t);
        std::string getHex(ulong in);
        std::vector<ulong> resizeBlock(std::vector<ulong> in);
    public:
        std::string digest(std::string input);
};