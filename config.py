import platform

# list of available methods
methods = ['sha256', 'sha512', 'sha1', 'adler32', 'crc16', 'crc32', 'blake2s', 'blake2b', 'md4', 'md5']

# generators
generators = {
    'Python': 'python generators/python/hash.py',
    'C++': './generators/cpp/hash.exe',
    'C#': './generators/cs/hash.exe'
}

# configuration for linux
if platform.system() == 'Linux':
    generators = {
        'Python': 'python3 generators/python/hash.py',
        'C++': './generators/cpp/hash',
        'C#': 'mono generators/cs/hash.exe'
    }