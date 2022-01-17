#include "blake2b.h"
using namespace std;

ull Blake2b::Rotr64(ull x, ull y) {
    return (((x) >> (y)) ^ ((x) << (64 - (y))));
}

ull Blake2b::Get64(uc b0, uc b1, uc b2, uc b3, uc b4, uc b5, uc b6, uc b7) {
    ull temp = 0;
    temp = (ull)b0;
    temp ^= (ull)b1 << 8;
    temp ^= (ull)b2 << 16;
    temp ^= (ull)b3 << 24;
    temp ^= (ull)b4 << 32;
    temp ^= (ull)b5 << 40;
    temp ^= (ull)b6 << 48;
    temp ^= (ull)b7 << 56;

    return temp;
}

void Blake2b::G(ull a, ull b, ull c, ull d, ull x, ull y) {
    v[a] = v[a] + v[b] + x;
    v[d] = Rotr64(v[d] ^ v[a], 32);
    v[c] = v[c] + v[d];
    v[b] = Rotr64(v[b] ^ v[c], 24);
    v[a] = v[a] + v[b] + y;
    v[d] = Rotr64(v[d] ^ v[a], 16);
    v[c] = v[c] + v[d];
    v[b] = Rotr64(v[b] ^ v[c], 63);
}

void Blake2b::compress(int last)
{
    uc sigma[12][16] = {
            { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
           { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 },
           { 11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4 },
           { 7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8 },
           { 9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13 },
           { 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 },
           { 12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11 },
           { 13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 },
           { 6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5 },
           { 10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0 },
           { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
           { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 }
    };
    ull m[16];

    for (int i = 0; i < 8; i++) {
        v[i] = h[i];
        v[i + 8] = blake2b_iv[i];
    }

    v[12] ^= t[0];
    v[13] ^= t[1];
    if (last)
        v[14] = ~v[14];


    for (int i = 0; i < 16; i++) {
        m[i] = Get64(b[i * 8 + 0], b[i * 8 + 1], b[i * 8 + 2], b[i * 8 + 3],
                    b[i * 8 + 4], b[i * 8 + 5], b[i * 8 + 6], b[i * 8 + 7]);
    }

    for (int i = 0; i < 12; i++) {
        G(0, 4, 8, 12, m[sigma[i][0]], m[sigma[i][1]]);
        G(1, 5, 9, 13, m[sigma[i][2]], m[sigma[i][3]]);
        G(2, 6, 10, 14, m[sigma[i][4]], m[sigma[i][5]]);
        G(3, 7, 11, 15, m[sigma[i][6]], m[sigma[i][7]]);
        G(0, 5, 10, 15, m[sigma[i][8]], m[sigma[i][9]]);
        G(1, 6, 11, 12, m[sigma[i][10]], m[sigma[i][11]]);
        G(2, 7, 8, 13, m[sigma[i][12]], m[sigma[i][13]]);
        G(3, 4, 9, 14, m[sigma[i][14]], m[sigma[i][15]]);
    }

    for (int i = 0; i < 8; ++i)
        h[i] ^= v[i] ^ v[i + 8];
}

void Blake2b::update(string chunks)
{
    for (int i = 0; i < chunks.length(); i++) {
        if (c == 128) {
            t[0] += c;
            if (t[0] < c)
                t[1]++;
            compress(0);
            c = 0;
        }
        b[c++] = chunks[i];
    }
}

int Blake2b::init(string key)
{
    int keylen = key.length();
    if (outlen == 0 || outlen > 64 || keylen > 64)
        return -1;

    for (int i = 0; i < 8; i++)
        h[i] = blake2b_iv[i];
    h[0] ^= 0x01010000 ^ (keylen << 8) ^ outlen;

    t[0] = 0;
    t[1] = 0;
    c = 0;

    for (int i = keylen; i < 128; i++)
        b[i] = 0;
    if (keylen > 0) {
        update(key);
        c = 128;
    }

    return 0;
}

void Blake2b::finalHash()
{
    t[0] += c;
    if (t[0] < c)
        t[1]++;

    while (c < 128)
        b[c++] = 0;
    compress(1);

    // little endian convert and store
    for (int i = 0; i < outlen; i++) {
        output.push_back((h[i >> 3] >> (8 * (i & 7))) & 0xFF);
    }
}

Blake2b::Blake2b(string key, string message)
{
    init(key);
    update(message);
    finalHash();
}

string Blake2b::digest() {
    stringstream ss;
    ss << std::hex;
    string s;

    for (int i = 0; i < 64; i++) {
        ss  << std::setw(2) << std::setfill('0') << (int)output[i] << "";
    }
    ss >> s;

    return s;
}
