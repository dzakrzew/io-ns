import hashlib
import argparse
import sys
import time
import json

available_methods = ['md5', 'sha256']

def hash_md5(input):
    # start measuring time
    start_time = time.time()

    file_hash = hashlib.md5(file_content)
    execution_time = time.time() - start_time

    return (file_hash.hexdigest(), execution_time)

def hash_sha256(input):
    # start measuring time
    start_time = time.time()

    file_hash = hashlib.sha256(file_content)
    execution_time = time.time() - start_time

    return (file_hash.hexdigest(), execution_time)

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

if selected_method == 'md5':
    file_hash, execution_time = hash_md5(file_content)

    # return response as json on stdout
    print_response(file_hash, execution_time)
    sys.exit(0)
elif selected_method == 'sha256':
    file_hash, execution_time = hash_sha256(file_content)

    # return response as json on stdout
    print_response(file_hash, execution_time)
    sys.exit(0)
else:
    print('Method not implemented')
    sys.exit(1)