#include <iostream>
#include "sha256.h"
#include "sha512.h"

using namespace std;

int main() {
    SHA256 sha256;
    SHA512 sha512;

    cout << sha256.digest("hello") << endl;
    cout << sha512.digest("hello") << endl;
}