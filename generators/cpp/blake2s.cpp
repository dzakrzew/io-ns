#include "blake2s.h"
using namespace std;

ul Blake2s::Rotr32(ul x, ul y) {
    return (((x) >> (y)) ^ ((x) << (32 - (y))));
}

ul Blake2s::Get32(uc b0, uc b1, uc b2, uc b3) {
    ul temp = 0;
    temp = (ul)b0;
    temp ^= (ul)b1 << 8;
    temp ^= (ul)b2 << 16;
    temp ^= (ul)b3 << 24;
    return temp;
}

void Blake2s::G(ul a, ul b, ul c, ul d, ul x, ul y) {
    v[a] = v[a] + v[b] + x;
    v[d] = Rotr32(v[d] ^ v[a], 16);
    v[c] = v[c] + v[d];
    v[b] = Rotr32(v[b] ^ v[c], 12);
    v[a] = v[a] + v[b] + y;
    v[d] = Rotr32(v[d] ^ v[a], 8);
    v[c] = v[c] + v[d];
    v[b] = Rotr32(v[b] ^ v[c], 7);
}

void Blake2s::compress(int last)
{
    uc sigma[10][16] = {
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
        { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 },
        { 11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4 },
        { 7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8 },
        { 9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13 },
        { 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 },
        { 12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11 },
        { 13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 },
        { 6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5 },
        { 10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0 }
    };
    ul m[16];

    for (int i = 0; i < 8; i++) {           // init work variables
        v[i] = h[i];
        v[i + 8] = blake2s_iv[i];
    }

    v[12] ^= t[0];                 // low 32 bits of offset
    v[13] ^= t[1];                 // high 32 bits
    if (last)                           // last block flag set ?
        v[14] = ~v[14];
    // get little-endian words

    for (int i = 0; i < 16; i++) {
        m[i] = Get32(b[i * 4 + 0], b[i * 4 + 1], b[i * 4 + 2], b[i * 4 + 3]);
    }

    for (int i = 0; i < 10; i++) {
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

void Blake2s::update(string chunks)
{
    for (int i = 0; i < chunks.length(); i++) {
        if (c == 64) {             // buffer full ?
            t[0] += c;        // add counters
            if (t[0] < c)     // carry overflow ?
                t[1]++;            // high word
            compress(0);   // compress (not last)
            c = 0;                 // counter to zero
        }
        b[c++] = chunks[i];
    }
}

int Blake2s::init(string key)     // (keyLen=0: no key)
{
    int keylen = key.length();
    if (outlen == 0 || outlen > 32 || keylen > 32)
        return -1;                      // illegal parameters

    for (int i = 0; i < 8; i++)             // state, "param block"
        h[i] = blake2s_iv[i];
    h[0] ^= 0x01010000 ^ (keylen << 8) ^ outlen;

    t[0] = 0;                      // input count low word
    t[1] = 0;                      // input count high word
    c = 0;                         // pointer within buffer

    for (int i = keylen; i < 64; i++)       // zero input block
        b[i] = 0;
    if (keylen > 0) {
        update(key);
        c = 64;                    // at the end
    }

    return 0;
}

void Blake2s::finalHash()
{
    t[0] += c;                // mark last block offset
    if (t[0] < c)             // carry overflow
        t[1]++;                    // high word

    while (c < 64)                 // fill up with zeros
        b[c++] = 0;
    compress(1);           // final block flag = 1

    // little endian convert and store
    for (int i = 0; i < outlen; i++) {
        output.push_back((h[i >> 2] >> (8 * (i & 3))) & 0xFF);
    }
}

Blake2s::Blake2s(string message)
{
    string key = "";
    init(key);
    update(message);
    finalHash();
}

string Blake2s::digest() {
    stringstream ss;
    ss << std::hex;
    string s;

    for (int i = 0; i < 32; i++) {
        ss  << std::setw(2) << std::setfill('0') << (int)output[i] << "";
    }
    ss >> s;

    return s;
}