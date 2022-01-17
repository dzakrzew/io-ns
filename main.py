import subprocess
import json
import glob
import os
import argparse
import string
import random
from report_generator import ReportGenerator
from config import methods, generators

report_generator = ReportGenerator()

def generate_file(output_path, size):
    f = open(output_path, 'w')
    content = ''.join([random.choice(string.ascii_letters + string.digits + 'ĄĆĘŁŃÓŚŹŻąćęłńóśźż') for i in range(size)])
    f.write(content)
    f.close()

parser = argparse.ArgumentParser(description='IO-NS Hashing algorithms comparison')
parser.add_argument('--random-sizes', metavar='sizes', help='Generate random files with given instead of reading existing ones (example: 100,1000,10000)')
args = parser.parse_args()

input_files = None

if args.random_sizes == None:
    input_files = glob.glob('inputs/*.txt')

    if len(input_files) == 0:
        print('There are no input files')
        exit(1)
else:
    sizes = args.random_sizes.split(',')

    try:
        sizes = [int(s) for s in sizes]
    except Exception as e:
        print('Invalid --random-sizes parameter contents')
        exit(1)

    input_files = glob.glob('inputs/*.txt')
    for f in input_files:
        os.remove(f)

    i = 0
    for s in sizes:
        print('Generating ' + str(s) + ' bytes size file in' + str(i) + '.txt')
        generate_file('inputs/in' + str(i) + '.txt', s)
        i += 1

input_files = glob.glob('inputs/*.txt')
input_files.sort()

print('IO-NS')
print('Found', len(input_files), 'input files')

for method in methods:
    for generator in generators:
        print('\n[*] Calculating', method, 'hashes using', generator)

        for input_file in input_files:
            result = subprocess.check_output(generators[generator].split(' ') + ['--method', method, '--file', input_file])
            out = json.loads(result)
            size = os.path.getsize(input_file)

            report_generator.add_calculation_result(method, generator, input_file, size, out['hash'], out['execution_time'])
            print('[', method, '] Result for', input_file, '[', size, 'bytes ]:', out['hash'], 'in', out['execution_time'], 'seconds')

print('\nGenerating report...')
report_generator.generate_report()