import glob, random, string, sys
import subprocess, json, os
import hashlib, zlib, crccheck
from config import methods, generators

def generate_file(output_path, size, include_polish = False):
    templ = string.ascii_letters + string.digits

    if include_polish:
        templ += 'ĄĆĘŁŃÓŚŹŻąćęłńóśźż'

    f = open(output_path, 'w')
    content = ''.join([random.choice(templ) for i in range(size)])
    f.write(content)
    f.close()

def generate_test_files():
    generate_file('tests/in_small.txt', 60)
    generate_file('tests/in_medium.txt', 1024*100)
    generate_file('tests/in_big.txt', 1024*200)
    generate_file('tests/in_polish.txt', 1024*10, True)

    outputs = {}
    test_inputs = glob.glob('tests/*.txt')
    for test_file in test_inputs:
        outputs[test_file.split('/')[-1]] = {}
        content = open(test_file, 'r').read().encode()

        for method in methods:
            digest = ''
            if method == 'adler32':
                digest = hex(zlib.adler32(content))[2:]
            if method == 'blake2b':
                digest = hashlib.blake2b(content).hexdigest()
            if method == 'blake2s':
                digest = hashlib.blake2s(content).hexdigest()
            if method == 'crc16':
                c = crccheck.crc.Crc16X25()
                c.process(content)
                digest = c.finalhex()
            if method == 'crc32':
                digest = hex(zlib.crc32(content) & 0xffffffff)[2:].zfill(8)
            if method == 'md4':
                digest = hashlib.new('md4', content).hexdigest()
            if method == 'md5':
                digest = hashlib.md5(content).hexdigest()
            if method == 'sha1':
                digest = hashlib.sha1(content).hexdigest()
            if method == 'sha256':
                digest = hashlib.sha256(content).hexdigest()
            if method == 'sha512':
                digest = hashlib.sha512(content).hexdigest()

            outputs[test_file.split('/')[-1]][method] = digest
    out = open('tests/out.json', 'w')
    out.write(json.dumps(outputs))
    out.close()


if '--generate' in sys.argv or len(glob.glob('tests/*.txt')) == 0:
    print('Generating test files...')
    generate_test_files()

input_files = glob.glob('tests/*.txt')
input_files.sort()

outputs = json.loads(open('tests/out.json', 'r').read())

for method in methods:
    for generator in generators:
        print('\n[*] Testing', method, 'using', generator)

        for input_file in input_files:
            result = subprocess.check_output(generators[generator].split(' ') + ['--method', method, '--file', input_file])
            out = json.loads(result)
            size = os.path.getsize(input_file)
            expected_hash = outputs[input_file.split('/')[-1]][method]

            if out['hash'] == expected_hash:
                print(method, '(', input_file, ') = OK')
            else:
                print(method, '(', input_file, ') = ERROR')
                print('Got:     ', out['hash'])
                print('Expected:', expected_hash)