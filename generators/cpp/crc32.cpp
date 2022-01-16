#include "crc32.h"

using namespace std;

string CRC32::getHex(unsigned long in) {
    bitset<32> bs(in);
    unsigned n = bs.to_ulong();

    stringstream sstream;
    sstream << std::hex << std::setw(8) << std::setfill('0') << n;
    string temp;
    sstream >> temp;

    return temp;
}

string CRC32::digest(string input)
{
    unsigned int crc = 0xFFFFFFFF;

    for (int i = 0; i < input.size(); i++) {
        char ch = input[i];

        for (int j = 0; j < 8; j++) {
            int b = (ch ^ crc) & 1;
            
            crc >>= 1;
            if (b) {
                crc = crc ^ 0xEDB88320;
            }
            ch >>= 1;
        }
    }

    return getHex(~crc);
}