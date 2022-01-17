#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <sstream>
#include <iomanip>

#define ul unsigned long
#define ull unsigned long long
#define uc unsigned char

class Blake2b {
private:
    std::vector<uc> output;
    uc b[128];
    ull h[8];
    ull t[2];
    ull c;
    ul outlen = 64;
    ull v[16];
    ull blake2b_iv[8] =
    {
       0x6A09E667F3BCC908, 0xBB67AE8584CAA73B,
       0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
       0x510E527FADE682D1, 0x9B05688C2B3E6C1F,
       0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179
    };
    ull Rotr64(ull x, ull y);
    ull Get64(uc b0, uc b1, uc b2, uc b3, uc b4, uc b5, uc b6, uc b7);
    void G(ull a, ull b, ull c, ull d, ull x, ull y);

    int init(std::string key);
    void update(std::string chunks);
    void finalHash();
    void compress(int last);

public:
    Blake2b(std::string message);
    std::string digest();
};