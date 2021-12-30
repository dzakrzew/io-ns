#include "sha1.h"

using namespace std;

ulong SHA1::f(ulong t, ulong B, ulong C, ulong D)
{
    if (0 <= t && t <= 19) {
        return SHA1_MOD((B & C) | ((~B) & D));
    }
    else if (20 <= t && t <= 39) {
        return SHA1_MOD(B ^ C ^ D);
    }
    else if (40 <= t && t <= 59) {
        return SHA1_MOD((B & C) | (B & D) | (C & D));
    }
    else if (60 <= t && t <= 79) {
        return SHA1_MOD(B ^ C ^ D);
    }
}

ulong SHA1::S(ulong n, ulong X) {
    return SHA1_MOD((X << n) | (X >> 32 - n));
}

ulong SHA1::K(ulong t) {
    if (0 <= t && t <= 19) {
        return 0x5A827999;
    }
    else if (20 <= t && t <= 39) {
        return 0x6ED9EBA1;
    }
    else if (40 <= t && t <= 59) {
        return 0x8F1BBCDC;
    }
    else if (60 <= t && t <= 79) {
        return 0xCA62C1D6;
    }
}

vector<ulong> SHA1::resizeBlock(vector<ulong> in) {
    vector<ulong> output(16);

    for (int i = 0; i < 64; i = i + 4) {
        bitset<32> temp(0);

        temp = (ulong) in[i] << 24;
		temp |= (ulong) in[i + 1] << 16;
		temp |= (ulong) in[i + 2] << 8;
		temp |= (ulong) in[i + 3];

        output[i / 4] = temp.to_ulong();
    }

    return output;
}

string SHA1::getHex(unsigned long in) {
    bitset<32> bs(in);
    unsigned n = bs.to_ulong();

    stringstream sstream;
    sstream << std::hex << std::setw(8) << std::setfill('0') << n;
    string temp;
    sstream >> temp;

    return temp;
}

string SHA1::digest(string input)
{
    vector<ulong> message;

    for (unsigned i = 0; i < input.size(); ++i) {
        bitset<8> b(input.c_str()[i]);
        message.push_back(b.to_ulong());
    }

    ulong length = message.size() * 8;

    message.push_back(0x80);

    while ((message.size() * 8 + 64) % 512 != 0) {
        message.push_back(0x00);
    }

    message.push_back(length >> 56);
    message.push_back(length >> 48);
    message.push_back(length >> 40);
    message.push_back(length >> 32);
    message.push_back(length >> 24);
    message.push_back(length >> 16);
    message.push_back(length >> 8);
    message.push_back(length);

    vector<vector<ulong>> blocks;

    for (unsigned i = 0; i < message.size() * 8 / 512; i++) {
        vector<ulong> vtemp;

        for (int j = 0; j < 64; j++) {
            vtemp.push_back(message[(i * 64) + j]);
        }

        blocks.push_back(resizeBlock(vtemp));
    }

    for (unsigned bi = 0; bi < blocks.size(); bi++) {
        ulong W[80];

        for (int t = 0; t <= 15; t++) {
            W[t] = SHA1_MOD(blocks[bi][t]);
        }

        for (int t = 16; t < 80; t++) {
            W[t] = S(1, SHA1_MOD(W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]));
        }

        ulong a = h0;
        ulong b = h1;
        ulong c = h2;
        ulong d = h3;
        ulong e = h4;
        ulong temp;

        for (int t = 0; t < 80; t++) {
            temp = SHA1_MOD(S(5, a) + f(t, b, c, d) + e + W[t] + K(t));
            e = d;
            d = c;
            c = S(30, b);
            b = a;
            a = temp;
        }

        h0 = SHA1_MOD(h0 + a);
        h1 = SHA1_MOD(h1 + b);
        h2 = SHA1_MOD(h2 + c);
        h3 = SHA1_MOD(h3 + d);
        h4 = SHA1_MOD(h4 + e);
    }

    stringstream result;
    string res;

    result << getHex(h0) << getHex(h1) << getHex(h2) << getHex(h3)
         << getHex(h4);
    result >> res;
    
    return res;


}