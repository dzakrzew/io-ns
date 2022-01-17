import hashlib
import argparse
import sys
import time
import json
from hash_functions.sha256 import SHA256
from hash_functions.sha512 import SHA512
from hash_functions.sha1 import SHA1
from hash_functions.md4 import MD4
from hash_functions.md5 import MD5
from hash_functions.crc16 import CRC16
from hash_functions.crc32 import CRC32
from hash_functions.adler32 import Adler32
from hash_functions.blake2b import Blake2b
from hash_functions.blake2s import Blake2s

available_methods = ['sha256', 'sha512', 'sha1', 'md4', 'md5', 'crc16', 'crc32', 'adler32', 'blake2b', 'blake2s']

def hash_sha256(file_content):
    start_time = time.time()
    file_hash = SHA256.digest(file_content)
    execution_time = time.time() - start_time
    return (file_hash, execution_time)

def hash_sha512(file_content):
    start_time = time.time()
    file_hash = SHA512.digest(file_content)
    execution_time = time.time() - start_time
    return (file_hash, execution_time)

def hash_sha1(file_content):
    start_time = time.time()
    file_hash = SHA1.digest(file_content)
    execution_time = time.time() - start_time
    return (file_hash, execution_time)

def hash_md4(file_content):
    start_time = time.time()
    file_hash = MD4.digest(file_content)
    execution_time = time.time() - start_time
    return (file_hash, execution_time)

def hash_md5(file_content):
    start_time = time.time()
    file_hash = MD5.digest(file_content)
    execution_time = time.time() - start_time
    return (file_hash, execution_time)

def hash_crc16(file_content):
    start_time = time.time()
    file_hash = CRC16.digest(file_content)
    execution_time = time.time() - start_time
    return (file_hash, execution_time)

def hash_crc32(file_content):
    start_time = time.time()
    file_hash = CRC32.digest(file_content)
    execution_time = time.time() - start_time
    return (file_hash, execution_time)

def hash_adler32(file_content):
    start_time = time.time()
    file_hash = Adler32.digest(file_content)
    execution_time = time.time() - start_time
    return (file_hash, execution_time)

def hash_blake2b(file_content):
    start_time = time.time()
    blake2b = Blake2b('', file_content)
    file_hash = blake2b.digest()
    execution_time = time.time() - start_time
    return (file_hash, execution_time)

def hash_blake2s(file_content):
    start_time = time.time()
    blake2s = Blake2s('', file_content)
    file_hash = blake2s.digest()
    execution_time = time.time() - start_time
    return (file_hash, execution_time)

def print_response(hash, execution_time):
    output = json.dumps({'hash': hash, 'execution_time': "{:.10f}".format(execution_time)})
    print(output, file = sys.stdout)
    sys.exit(0)

parser = argparse.ArgumentParser(description='Generates hash for given file using selected method and measures time of execution')
parser.add_argument('--method', metavar='method', help='method (default: md5)')
parser.add_argument('--file', metavar='file', help='input file to generate hash from')
args = parser.parse_args()

selected_method = args.method
file_path = args.file

if selected_method not in available_methods:
    print(selected_method, 'is not an available hashing method. Available methods:', available_methods)
    sys.exit(1)

file_content = None

try:
    with open(file_path, 'rb') as input_file:
        file_content = input_file.read()
except Exception as e:
    print('Error while opening the input file:', e)
    sys.exit(1)

if selected_method in available_methods:
    file_hash, execution_time = globals()['hash_' + selected_method](file_content)

    # return response as json on stdout
    print_response(file_hash, execution_time)
    sys.exit(0)

else:
    print('Method not implemented')
    sys.exit(1)