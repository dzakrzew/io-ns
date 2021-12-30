#include <iostream>
#include <vector>
#include <bitset>
#include <iomanip>

#define ulong unsigned long

class Adler32 {
    private:
        const ulong MOD_ADLER = 65521;

        std::string getHex(ulong in);
    public:
        std::string digest(std::string input);
};