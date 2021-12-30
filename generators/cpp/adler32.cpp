#include "adler32.h"

using namespace std;

string Adler32::getHex(unsigned long in) {
    bitset<32> bs(in);
    unsigned n = bs.to_ulong();

    stringstream sstream;
    sstream << std::hex << std::setw(8) << std::setfill('0') << n;
    string temp;
    sstream >> temp;

    return temp;
}

string Adler32::digest(string input)
{
    vector<ulong> message;

    for (unsigned i = 0; i < input.size(); ++i) {
        bitset<8> b(input.c_str()[i]);
        message.push_back(b.to_ulong());
    }

    ulong length = message.size();

    ulong a = 1;
    ulong b = 0;

    for (int i = 0; i < length; i++) {
        a = (a + message[i]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }

    return getHex(((b << 16) | a));
}