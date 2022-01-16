#include "crc16.h"

using namespace std;

string CRC16::getHex(unsigned long in) {
    bitset<16> bs(in);
    unsigned n = bs.to_ulong();

    stringstream sstream;
    sstream << std::hex << std::setw(4) << std::setfill('0') << n;
    string temp;
    sstream >> temp;

    return temp;
}

string CRC16::digest(string input)
{
    unsigned short crc = 0xFFFF;

    for (int i = 0; i < input.size(); i++) {
        unsigned char ch = input[i];

        for (int j = 0; j < 8; j++) {
            unsigned char b = (ch ^ crc) & 1;
            crc >>= 1;
            if (b) {
                crc = crc ^ 0x8408;
            }
            ch >>= 1;
        }
    }

    return getHex(~crc);
}