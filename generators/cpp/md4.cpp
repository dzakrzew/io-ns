#include "md4.h"
using namespace std;

ulong MD4::F(ulong x, ulong y, ulong z) {
	return (x & y) | (~x & z);
}

ulong MD4::G(ulong x, ulong y, ulong z) {
    return (x & y) | (x & z) | (y & z);
}

ulong MD4::H(ulong x, ulong y, ulong z) {
    return x ^ y ^ z;
}

ulong MD4::leftRotate(unsigned long long val, int n) {
    ulong lbits = (val << n) & mask;
    ulong rbits = val >> (width - n);
    return lbits | rbits;
}


vector<ulong> MD4::resizeBlock(vector<ulong> in) {
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

MD4::MD4(string input) {
    vector<ulong> message;

    for (unsigned i = 0; i < input.size(); ++i) {
        bitset<8> b(input.c_str()[i]);
        message.push_back(b.to_ulong());
    }

    //uint32_t length = message.size() * 8;
    unsigned long long ll = static_cast<unsigned __int64>(message.size() * 8);

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

vector<unsigned char> MD4::getHex(ulong in) {
    string temp;
    vector<unsigned char> b = { 0, 0, 0, 0};
    b[3] = (int)((words[in] >> 24) & 0xFF);
    b[2] = (int)((words[in] >> 16) & 0xFF);
    b[1] = (int)((words[in] >> 8) & 0XFF);
    b[0] = (int)((words[in] & 0XFF));

    return b;
}

string MD4::hexdigest() {
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

void MD4::process(vector<vector<ulong>> chunks) {
    for (vector<ulong> chunk : chunks) {
        vector<ulong> X;
        vector<ulong> h = words;


       for (int i = 0; i < chunk.size(); i += 4) {
            unsigned char bytes[4]{ chunk[i], chunk[i + 1], chunk[i + 2], chunk[i + 3] };
            unsigned int value;
            std::memcpy(&value, bytes, sizeof(int));
            X.push_back(value);
        }

       vector<ulong> Xi = { 3,7,11,19 };
       // round 1
       for (int n = 0; n < 16; n++) {
            //modulo w C++ działa inaczej na negatywnych intach niż python dlatego potrzebne są takie komplikacje
            int a = (4 + ((-n + 0) % 4)) % 4;
            int b = (4 + ((-n + 1) % 4)) % 4;;
            int c = (4 + ((-n + 2) % 4)) % 4;;
            int d = (4 + ((-n + 3) % 4)) % 4;;

            int K = n;
            int S = Xi[n % 4];

            //wartości tej zmiennej wykraczają poza 32 bity, dlatego trzeba je rzutować na ullong
            ull to_rotate = h[a];
            to_rotate +=  X[K];
            to_rotate += F(h[b], h[c], h[d]);

            h[a] = leftRotate((to_rotate & mask), S);
       }

       //round 2
       Xi = { 3, 5, 9, 13 };
       for (int n = 0; n < 16; n++) {
           int a = (4 + ((-n + 0) % 4)) % 4;
           int b = (4 + ((-n + 1) % 4)) % 4;;
           int c = (4 + ((-n + 2) % 4)) % 4;;
           int d = (4 + ((-n + 3) % 4)) % 4;;

           int K = floor(n % 4 * 4 + n / 4);
           int S = Xi[n % 4];

           ull to_rotate = h[a];
           to_rotate += X[K];
           to_rotate += G(h[b], h[c], h[d]);
           to_rotate += 0x5A827999;

           h[a] = leftRotate((to_rotate & mask), S);
       }

       //round 3
       Xi = { 3, 9, 11, 15 };
       vector<ulong> Ki = { 0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15 };
       for (int n = 0; n < 16; n++) {
           int a = (4 + ((-n + 0) % 4)) % 4;
           int b = (4 + ((-n + 1) % 4)) % 4;;
           int c = (4 + ((-n + 2) % 4)) % 4;;
           int d = (4 + ((-n + 3) % 4)) % 4;;

           int K = Ki[n];
           int S = Xi[n % 4];

           ull to_rotate = h[a];
           to_rotate += X[K];
           to_rotate += H(h[b], h[c], h[d]);
           to_rotate += 0x6ED9EBA1;

           h[a] = leftRotate((to_rotate & mask), S);
       }

       words[0] = (words[0] + h[0]) & mask;
       words[1] = (words[1] + h[1]) & mask;
       words[2] = (words[2] + h[2]) & mask;
       words[3] = (words[3] + h[3]) & mask;
    }
}
/*
int main() {
    MD4 md4("hello");
    string s = md4.hexdigest();
    cout << s << "\n";

    return 0;
}
*/