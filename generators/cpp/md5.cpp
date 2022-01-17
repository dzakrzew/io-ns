#include "md5.h"
using namespace std;

ulong MD5::F(ulong x, ulong y, ulong z) {
    return (x & y) | (~x & z);
}

ulong MD5::G(ulong x, ulong y, ulong z) {
    return (z & x) | (~z & y);
}

ulong MD5::H(ulong x, ulong y, ulong z) {
    return x ^ y ^ z;
}

ulong MD5::I(ulong x, ulong y, ulong z) {
    return y ^ (x | ~z);
}

ulong MD5::leftRotate(unsigned long long val, int n) {
    ulong lbits = (val << n) & mask;
    ulong rbits = val >> (width - n);
    return lbits | rbits;
}


vector<ulong> MD5::resizeBlock(vector<ulong> in) {
    vector<ulong> output(16);

    for (int i = 0; i < 64; i = i + 4) {
        bitset<32> temp(0);

        temp = (ulong)in[i] << 24;
        temp |= (ulong)in[i + 1] << 16;
        temp |= (ulong)in[i + 2] << 8;
        temp |= (ulong)in[i + 3];

        output[i / 4] = temp.to_ulong();
    }

    return output;
}

MD5::MD5(string input) {
    vector<ulong> message;

    for (unsigned i = 0; i < input.size(); ++i) {
        bitset<8> b(input.c_str()[i]);
        message.push_back(b.to_ulong());
    }

    //uint32_t length = message.size() * 8;
    ull ll = static_cast<ull>(message.size() * 8);

    //dodanie bitu 128 na koniec wiadomości
    message.push_back(0x80);

    //uzupełnie wiadomości do 448 bitów
    while ((message.size() * 8 + 64) % 512 != 0) {
        message.push_back(0x00);
    }

    //dodanie padingu do 512 bitów
    for (int i = 0; i <= 56; i += 8) {
        message.push_back(ll >> i);
    }

    //podział wiadomości na bloki 512 bitów
    vector<vector<ulong>> message_chunks;
    for (unsigned i = 0; i < message.size() * 8 / 512; i++) {
        vector<ulong> vtemp;

        for (int j = 0; j < 64; j++) {
            vtemp.push_back(message[(i * 64) + j]);
        }

        message_chunks.push_back(vtemp);
    }

    process(message_chunks);
}

vector<unsigned char> MD5::getHex(ulong in) {
    string temp;
    vector<unsigned char> b = { 0, 0, 0, 0 };
    b[3] = (int)((words[in] >> 24) & 0xFF);
    b[2] = (int)((words[in] >> 16) & 0xFF);
    b[1] = (int)((words[in] >> 8) & 0XFF);
    b[0] = (int)((words[in] & 0XFF));

    return b;
}

string MD5::hexdigest() {
    string res = "";

    std::stringstream ss;
    ss << std::hex;
    for (int wi = 0; wi < 4; wi++) {
        for (int i(0); i < 4; i++)
            ss << std::setw(2) << std::setfill('0') << (int)getHex(wi)[i];
    }
    ss >> res;
    return res;
}

void MD5::process(vector<vector<ulong>> chunks) {
    for (vector<ulong> chunk : chunks) {
        vector<ulong> X;
        vector<ulong> h = words;

        //prepare constants
        vector<ulong> constants;
        for (int i = 0; i < 64; i++) {
            ull c = (abs( (double)sin(i + 1)) * 4294967296);
            c = c & mask;
            constants.push_back(c);
            //cout << constants[i] << "  ";
        }

        for (int i = 0; i < chunk.size(); i += 4) {
            unsigned char bytes[4]{ chunk[i], chunk[i + 1], chunk[i + 2], chunk[i + 3] };
            unsigned int value;
            std::memcpy(&value, bytes, sizeof(int));
            X.push_back(value);
        }

        vector<ulong> Xi = { 7, 12, 17, 22 };
        // round 1
        for (int n = 0; n < 16; n++) {
            //modulo w C++ działa inaczej na negatywnych intach niż python dlatego potrzebne są takie komplikacje
            int a = (4 + ((-n + 0) % 4)) % 4;
            int b = (4 + ((-n + 1) % 4)) % 4;;
            int c = (4 + ((-n + 2) % 4)) % 4;;
            int d = (4 + ((-n + 3) % 4)) % 4;;

            int K = n;
            int S = Xi[n % 4];
            int T = n;

            //wartości tej zmiennej wykraczają poza 32 bity, dlatego trzeba je rzutować na ullong
            ull to_rotate = h[a];
            to_rotate += X[K];
            to_rotate += F(h[b], h[c], h[d]);
            to_rotate += constants[T];

            h[a] = (h[b] + leftRotate((to_rotate & mask), S)) & mask;
        }

        //round 2
        Xi = { 5, 9, 14, 20 };
        for (int n = 0; n < 16; n++) {
            int a = (4 + ((-n + 0) % 4)) % 4;
            int b = (4 + ((-n + 1) % 4)) % 4;;
            int c = (4 + ((-n + 2) % 4)) % 4;;
            int d = (4 + ((-n + 3) % 4)) % 4;;

            int K = (5 * n + 1) % 16;
            int S = Xi[n % 4];
            int T = n + 16;

            ull to_rotate = h[a];
            to_rotate += X[K];
            to_rotate += G(h[b], h[c], h[d]);
            to_rotate += constants[T];

            h[a] = (h[b] + leftRotate((to_rotate & mask), S)) & mask;
        }

        //round 3
        Xi = { 4, 11, 16, 23 };
        for (int n = 0; n < 16; n++) {
            int a = (4 + ((-n + 0) % 4)) % 4;
            int b = (4 + ((-n + 1) % 4)) % 4;;
            int c = (4 + ((-n + 2) % 4)) % 4;;
            int d = (4 + ((-n + 3) % 4)) % 4;;

            int K = (3 * n + 5) % 16;
            int S = Xi[n % 4];
            int T = n + 32;

            ull to_rotate = h[a];
            to_rotate += X[K];
            to_rotate += H(h[b], h[c], h[d]);
            to_rotate += constants[T];

            h[a] = (h[b] + leftRotate((to_rotate & mask), S)) & mask;
        }

        //round 4
        Xi = { 6, 10, 15, 21 };
        for (int n = 0; n < 16; n++) {
            int a = (4 + ((-n + 0) % 4)) % 4;
            int b = (4 + ((-n + 1) % 4)) % 4;;
            int c = (4 + ((-n + 2) % 4)) % 4;;
            int d = (4 + ((-n + 3) % 4)) % 4;;

            int K = (7 * n) % 16;
            int S = Xi[n % 4];
            int T = n + 48;

            ull to_rotate = h[a];
            to_rotate += X[K];
            to_rotate += I(h[b], h[c], h[d]);
            to_rotate += constants[T];

            h[a] = (h[b] + leftRotate((to_rotate & mask), S)) & mask;
        }

        words[0] = (words[0] + h[0]) & mask;
        words[1] = (words[1] + h[1]) & mask;
        words[2] = (words[2] + h[2]) & mask;
        words[3] = (words[3] + h[3]) & mask;
    }
}

/*
int main() {
    MD5 md5("hello");
    string s = md5.hexdigest();
    cout << s << "\n";

    return 0;
}
*/