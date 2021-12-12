import subprocess
import json
import glob
import os
from report_generator import ReportGenerator

report_generator = ReportGenerator()

# list of available methods
methods = ['md5', 'sha256', 'sha512']

# generators
generators = {
    'Python': 'python3.6 generators/hash.py'
}

# detecting input files
input_files = glob.glob('inputs/in*.txt')
input_files.sort()

print('IO-NS')
print('Found', len(input_files), 'input files')

for method in methods:
    for generator in generators:
        print('[*] Calculating', method, 'hashes using', generator)

        for input_file in input_files:
            result = subprocess.check_output(generators[generator].split(' ') + ['--method', method, '--file', input_file])
            out = json.loads(result)
            size = os.path.getsize(input_file)

            report_generator.add_calculation_result(method, generator, input_file, size, out['hash'], out['execution_time'])
            print('[', method, '] Result for', input_file, '[', size, 'bytes ]:', out['hash'], 'in', out['execution_time'], 'seconds')

print('Generating report...')
report_generator.generate_report()