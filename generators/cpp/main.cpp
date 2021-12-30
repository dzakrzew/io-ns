#include <iostream>
#include "sha256.h"
#include "sha512.h"
#include "sha1.h"

using namespace std;

int main() {
    SHA1 sha1;
    SHA256 sha256;
    SHA512 sha512;

    cout << sha1.digest("hello") << endl;
    cout << sha256.digest("hello") << endl;
    cout << sha512.digest("hello") << endl;
}