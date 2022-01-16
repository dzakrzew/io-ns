#include <iostream>
#include <vector>
#include <bitset>
#include <iomanip>

#define ulong unsigned long

class CRC16 {
    private:
        std::string getHex(ulong in);
    public:
        std::string digest(std::string input);
};