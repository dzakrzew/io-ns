#include <iostream>
#include <chrono>
#include <fstream>
#include <streambuf>
#include "sha256.h"
#include "sha512.h"
#include "sha1.h"
#include "adler32.h"
#include "crc32.h"
#include "crc16.h"
#include "md4.h"
#include "md5.h"
#include "blake2b.h"
#include "blake2s.h"

enum Method {
    METHOD_SHA1, METHOD_SHA256, METHOD_SHA512, 
    METHOD_Adler32, METHOD_CRC32, METHOD_CRC16,
    METHOD_MD4, METHOD_MD5,
    METHOD_Blake2b, METHOD_Blake2s
};

using namespace std;

int main(int argc, char *argv[]) 
{
    Method selectedMethod;
    auto t1 = std::chrono::high_resolution_clock::now();
    auto t2 = std::chrono::high_resolution_clock::now();
    string digest;
    string file;
    string input;

    for (int i = 0; i < argc - 1; i++) {
        if (strcmp(argv[i], "--method") == 0) {
            if (strcmp(argv[i + 1], "sha1") == 0) {
                selectedMethod = METHOD_SHA1;
            }
            else if (strcmp(argv[i + 1], "sha256") == 0) {
                selectedMethod = METHOD_SHA256;
            }
            else if (strcmp(argv[i + 1], "sha512") == 0) {
                selectedMethod = METHOD_SHA512;
            }
            else if (strcmp(argv[i + 1], "adler32") == 0) {
                selectedMethod = METHOD_Adler32;
            }
            else if (strcmp(argv[i + 1], "crc32") == 0) {
                selectedMethod = METHOD_CRC32;
            }
            else if (strcmp(argv[i + 1], "crc16") == 0) {
                selectedMethod = METHOD_CRC16;
            }
            else if (strcmp(argv[i + 1], "md4") == 0) {
                selectedMethod = METHOD_MD4;
            }
            else if (strcmp(argv[i + 1], "md5") == 0) {
                selectedMethod = METHOD_MD5;
            }
            else if (strcmp(argv[i + 1], "blake2b") == 0) {
                selectedMethod = METHOD_Blake2b;
            }
            else if (strcmp(argv[i + 1], "blake2s") == 0) {
                selectedMethod = METHOD_Blake2s;
            }
            else {
                cout << "Undefined method" << endl;
                return 1;
            }
        }

        if (strcmp(argv[i], "--file") == 0) {
            ifstream f(argv[i + 1]);
            stringstream buffer;
            buffer << f.rdbuf();
            buffer >> input;
        }
    }

    if (selectedMethod == METHOD_SHA1) {
        SHA1 sha1;
        t1 = std::chrono::high_resolution_clock::now();
        digest = sha1.digest(input);
        t2 = std::chrono::high_resolution_clock::now();
    }

    if (selectedMethod == METHOD_SHA256) {
        SHA256 sha256;
        t1 = std::chrono::high_resolution_clock::now();
        digest = sha256.digest(input);
        t2 = std::chrono::high_resolution_clock::now();
    }

    if (selectedMethod == METHOD_SHA512) {
        SHA512 sha512;
        t1 = std::chrono::high_resolution_clock::now();
        digest = sha512.digest(input);
        t2 = std::chrono::high_resolution_clock::now();
    }

    if (selectedMethod == METHOD_Adler32) {
        Adler32 adler32;
        t1 = std::chrono::high_resolution_clock::now();
        digest = adler32.digest(input);
        t2 = std::chrono::high_resolution_clock::now();
    }

    if (selectedMethod == METHOD_CRC32) {
        CRC32 crc32;
        t1 = std::chrono::high_resolution_clock::now();
        digest = crc32.digest(input);
        t2 = std::chrono::high_resolution_clock::now();
    }

    if (selectedMethod == METHOD_CRC16) {
        CRC16 crc16;
        t1 = std::chrono::high_resolution_clock::now();
        digest = crc16.digest(input);
        t2 = std::chrono::high_resolution_clock::now();
    }

    if (selectedMethod == METHOD_MD4) {
        t1 = std::chrono::high_resolution_clock::now();
        MD4 md4(input);
        digest = md4.hexdigest();
        t2 = std::chrono::high_resolution_clock::now();
    }

    if (selectedMethod == METHOD_MD5) {
        t1 = std::chrono::high_resolution_clock::now();
        MD5 md5(input);
        digest = md5.hexdigest();
        t2 = std::chrono::high_resolution_clock::now();
    }

    if (selectedMethod == METHOD_Blake2b) {
        t1 = std::chrono::high_resolution_clock::now();
        Blake2b blake2b(input);
        digest = blake2b.digest();
        t2 = std::chrono::high_resolution_clock::now();
    }

    if (selectedMethod == METHOD_Blake2s) {
        t1 = std::chrono::high_resolution_clock::now();
        Blake2s blake2s(input);
        digest = blake2s.digest();
        t2 = std::chrono::high_resolution_clock::now();
    }
    
    
    cout << "{\"hash\": \"" << digest << "\", \"execution_time\": \"" 
        << fixed
        << setprecision(10)
        << (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()) / (10e9)
        << "\"}" << endl;
}