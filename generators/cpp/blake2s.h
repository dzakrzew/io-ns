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

class Blake2s {
private:
    std::vector<uc> output;
    uc b[64];
    ul h[8];
    ul t[2];
    ul c;
    ul outlen = 32;
    ul v[16]; //dla uproszczenia umieszczam to tutaj, orignalnie w jest w G

    ul blake2s_iv[8] =
    {
        0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19
    };
    ul Get32(uc b0, uc b1, uc b2, uc b3);
    void G(ul a, ul b, ul c, ul d, ul x, ul y);
    ul Rotr32(ul x, ul y);

    int init(std::string key);
    void update(std::string chunks);
    void finalHash();
    void compress(int last);

public:
    Blake2s(std::string key, std::string message);
    std::string digest();
};