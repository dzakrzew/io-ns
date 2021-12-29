#include "sha256.h"

using namespace std;

string SHA256::getHex(unsigned long in) {
    bitset<32> bs(in);
    unsigned n = bs.to_ulong();

    stringstream sstream;
    sstream << std::hex << std::setw(8) << std::setfill('0') << n;
    string temp;
    sstream >> temp;

    return temp;
}

vector<unsigned long> SHA256::resizeBlock(vector<unsigned long> in) {
    vector<unsigned long> output(16);

    for (int i = 0; i < 64; i = i + 4) {
        bitset<32> temp(0);

        temp = (unsigned long) in[i] << 24;
		temp |= (unsigned long) in[i + 1] << 16;
		temp |= (unsigned long) in[i + 2] << 8;
		temp |= (unsigned long) in[i + 3];

        output[i / 4] = temp.to_ulong();
    }

    return output;
}

string SHA256::digest(string input)
{
    vector<unsigned long> message;

    for (unsigned i = 0; i < input.size(); ++i) {
        bitset<8> b(input.c_str()[i]);
        message.push_back(b.to_ulong());
    }

    unsigned long length = message.size() * 8;

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

    vector<vector<unsigned long>> blocks;

    for (unsigned i = 0; i < message.size() * 8 / 512; i++) {
        vector<unsigned long> vtemp;

        for (int j = 0; j < 64; j++) {
            vtemp.push_back(message[(i * 64) + j]);
        }

        blocks.push_back(resizeBlock(vtemp));
    }

    for (unsigned bi = 0; bi < blocks.size(); bi++) {
        unsigned long W[64];

        for (int t = 0; t <= 15; t++) {
            W[t] = SHA256_MOD(blocks[bi][t]);

        }

        for (int t = 16; t < 64; t++) {
            W[t] = SHA256_MOD(SHA256_SSIG1(W[t-2]) + W[t-7] + SHA256_SSIG0(W[t-15]) + W[t-16]);
        }

        unsigned long a = h0;
        unsigned long b = h1;
        unsigned long c = h2;
        unsigned long d = h3;
        unsigned long e = h4;
        unsigned long f = h5;
        unsigned long g = h6;
        unsigned long h = h7;
        unsigned long temp1;
        unsigned long temp2;

        for (int t = 0; t < 64; t++) {
            temp1 = SHA256_MOD(h + SHA256_BSIG1(e) + SHA256_CH(e,f,g) + K[t] + W[t]);
            temp2 = SHA256_MOD(SHA256_BSIG0(a) + SHA256_MAJ(a,b,c));
            h = g;
            g = f;
            f = e;
            e = SHA256_MOD(d + temp1);
            d = c;
            c = b;
            b = a;
            a = SHA256_MOD(temp1 + temp2);
        }

        h0 = SHA256_MOD(h0 + a);
        h1 = SHA256_MOD(h1 + b);
        h2 = SHA256_MOD(h2 + c);
        h3 = SHA256_MOD(h3 + d);
        h4 = SHA256_MOD(h4 + e);
        h5 = SHA256_MOD(h5 + f);
        h6 = SHA256_MOD(h6 + g);
        h7 = SHA256_MOD(h7 + h);
    }

    stringstream result;
    string res;

    result << getHex(h0) << getHex(h1) << getHex(h2) << getHex(h3)
         << getHex(h4) << getHex(h5) << getHex(h6) << getHex(h7);
    result >> res;
    
    return res;
}