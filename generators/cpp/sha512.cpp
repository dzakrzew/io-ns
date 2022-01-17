#include "sha512.h"

using namespace std;

string SHA512::getHex(uint64_t in) {
    stringstream sstream;
    sstream << std::hex << std::setw(16) << std::setfill('0') << in;
    string temp;
    sstream >> temp;

    return temp;
}

vector<uint64_t> SHA512::resizeBlock(vector<uint64_t> in) {
    vector<uint64_t> output(16);

    for (int i = 0; i < 128; i = i + 8) {
        bitset<64> temp(0);

        temp = (uint64_t) in[i] << 56;
		temp |= (uint64_t) in[i + 1] << 48;
		temp |= (uint64_t) in[i + 2] << 40;
		temp |= (uint64_t) in[i + 3] << 32;
		temp |= (uint64_t) in[i + 4] << 24;
		temp |= (uint64_t) in[i + 5] << 16;
		temp |= (uint64_t) in[i + 6] << 8;
		temp |= (uint64_t) in[i + 7];

        output[i / 8] = temp.to_ullong();
    }

    return output;
}

string SHA512::digest(string input)
{
    vector<uint64_t> message;

    for (unsigned i = 0; i < input.size(); ++i) {
        bitset<8> b(input.c_str()[i]);
        message.push_back(b.to_ullong());
    }

    uint64_t length = message.size() * 8;

    message.push_back(0x80);

    while ((message.size() * 8 + 128) % 1024 != 0) {
        message.push_back(0x00);
    }

    message.push_back(0x00);
    message.push_back(0x00);
    message.push_back(0x00);
    message.push_back(0x00);
    message.push_back(0x00);
    message.push_back(0x00);
    message.push_back(0x00);
    message.push_back(0x00);
    message.push_back(length >> 56);
    message.push_back(length >> 48);
    message.push_back(length >> 40);
    message.push_back(length >> 32);
    message.push_back(length >> 24);
    message.push_back(length >> 16);
    message.push_back(length >> 8);
    message.push_back(length);

    vector<vector<uint64_t>> blocks;

    for (unsigned i = 0; i < message.size() * 8 / 1024; i++) {
        vector<uint64_t> vtemp;

        for (int j = 0; j < 128; j++) {
            vtemp.push_back(message[(i * 128) + j]);
        }

        blocks.push_back(resizeBlock(vtemp));
    }

    for (unsigned bi = 0; bi < blocks.size(); bi++) {
        uint64_t W[80];

        for (int t = 0; t <= 15; t++) {
            W[t] = SHA512_MOD(blocks[bi][t]);

        }

        for (int t = 16; t < 80; t++) {
            W[t] = SHA512_MOD(SHA512_SSIG1(W[t-2]) + W[t-7] + SHA512_SSIG0(W[t-15]) + W[t-16]);
        }

        uint64_t a = h0;
        uint64_t b = h1;
        uint64_t c = h2;
        uint64_t d = h3;
        uint64_t e = h4;
        uint64_t f = h5;
        uint64_t g = h6;
        uint64_t h = h7;
        uint64_t temp1;
        uint64_t temp2;

        for (int t = 0; t < 80; t++) {
            temp1 = SHA512_MOD(h + SHA512_BSIG1(e) + SHA512_CH(e,f,g) + K[t] + W[t]);
            temp2 = SHA512_MOD(SHA512_BSIG0(a) + SHA512_MAJ(a,b,c));
            h = g;
            g = f;
            f = e;
            e = SHA512_MOD(d + temp1);
            d = c;
            c = b;
            b = a;
            a = SHA512_MOD(temp1 + temp2);
        }

        h0 = SHA512_MOD(h0 + a);
        h1 = SHA512_MOD(h1 + b);
        h2 = SHA512_MOD(h2 + c);
        h3 = SHA512_MOD(h3 + d);
        h4 = SHA512_MOD(h4 + e);
        h5 = SHA512_MOD(h5 + f);
        h6 = SHA512_MOD(h6 + g);
        h7 = SHA512_MOD(h7 + h);
    }

    stringstream result;
    string res;

    result << getHex(h0) << getHex(h1) << getHex(h2) << getHex(h3)
         << getHex(h4) << getHex(h5) << getHex(h6) << getHex(h7);
    result >> res;
    
    return res;
}