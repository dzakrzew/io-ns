import subprocess
import json
import glob
import os

calculation_results = {}

def add_calculation_result(method, input_file, size, hash, execution_time):
    if method not in calculation_results:
        calculation_results[method] = []
    
    calculation_results[method].append([input_file, size, hash, execution_time])

def generate_report():
    html = '<!DOCTYPE html><html><head><title>IO-NS</title><meta charset="utf-8"></head><style>table, table * { border: 1px solid black; }</style><body>'

    for method in calculation_results:
        html += '<h1>' + method + '</h1>'
        html += '<table><tr><th>Input file</th><th>Size</th><th>Hash</th><th>Execution time</th></tr>'

        for results in calculation_results[method]:
            html += '<tr><td>' + results[0] + '</td><td>' + str(results[1]) + ' B</td><td>' + results[2] + '</td><td>' + results[3] + 's</td></tr>'
        
        html += '</table>'

    html += '</body></html>'
    f = open('reports/report.html', 'w')
    f.write(html)
    f.close()

# list of available methods
methods = ['md5', 'sha256']

# detecting input files
input_files = glob.glob('inputs/in*.txt')
input_files.sort()

print('IO-NS')
print('Found', len(input_files), 'input files')

for method in methods:
    print('Calculating', method, 'hashes')

    for input_file in input_files:
        result = subprocess.check_output(['python3.6', 'generators/hash.py', '--method', method, '--file', input_file])
        out = json.loads(result)
        size = os.path.getsize(input_file)

        add_calculation_result(method, input_file, size, out['hash'], out['execution_time'])
        print('[', method, '] Result for', input_file, '[', size, 'bytes ]:', out['hash'], 'in', out['execution_time'], 'seconds')

print('Generating report...')
generate_report()