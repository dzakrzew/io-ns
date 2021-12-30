#include <iostream>
#include "sha256.h"
#include "sha512.h"
#include "sha1.h"
#include "adler32.h"

using namespace std;

int main() {
    SHA1 sha1;
    SHA256 sha256;
    SHA512 sha512;
    Adler32 adler32;

    cout << sha1.digest("hello") << endl;
    cout << sha256.digest("hello") << endl;
    cout << sha512.digest("hello") << endl;
    cout << adler32.digest("hello") << endl;
}