# Na skróty
Software Engineering student project

## Description
The goal of this project is to implement and compare multiple hash functions in different programming languages.

## Implementations and languages*
* SHA-1 in Python
* SHA-1 in C#
* SHA-1 in C++
* SHA-256 in Python
* SHA-256 in C#
* SHA-256 in C++
* SHA-512 in Python
* SHA-512 in C#
* SHA-512 in C++
* ADLER-32 in Python
* ADLER-32 in C#
* ADLER-32 in C++
* MD4 in Python
* MD4 in C#
* MD4 in C++
* MD5 in Python
* MD5 in C#
* MD5 in C++
* CRC-16 in Python
* CRC-16 in C#
* CRC-16 in C++
* CRC-32 in Python
* CRC-32 in C#
* CRC-32 in C++
* BLAKE2s in Python
* BLAKE2s in C#
* BLAKE2s in C++
* BLAKE2b in Python
* BLAKE2b in C#
* BLAKE2b in C++

*strike line – not implemented yet

## How to run
The project supports Python >=3.6 and Linux operating system.
To execute comparison test and generate report for files inside `inputs/` directory, run the following command:
```
python main.py
```
If there are no input files, it is possible to generate random input data with specified size for each execution of script:
```
python main.py --random-sizes 10,100,1024,10240,102400,1048576
```
The example above will test every implementation using 6 files in the following sizes: `10B`, `100B`, `1KiB`, `10KiB`, `100KiB`, `1MiB`.

## Testing
To execute automated tests that will check every implementation and compare results to Python built-in hash libraries, run the following command:
```
python test.py --generate
```

## Dependencies
* To execute tests, it is necessary to install crccheck library: `pip install crccheck`
* To run C# implementations, the Mono runtime is required: [See tutorial for Ubuntu 20.04](https://linuxize.com/post/how-to-install-mono-on-ubuntu-20-04/)