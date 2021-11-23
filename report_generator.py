from datetime import datetime
import platform
import json
import os

class ReportGenerator:
    def __init__(self):
        self.results = {}
    
    def add_calculation_result(self, method, generator, input_file, size, hash, execution_time):
        if method not in self.results:
            self.results[method] = []

        result = {
            'generator': generator,
            'input_file': input_file,
            'size': size,
            'hash': hash,
            'execution_time': execution_time
        }

        self.results[method].append(result)
    
    def generate_report(self, filename = 'report.html'):
        
        template = open(os.path.join('reports', 'static', '_layout.html.template'), 'r').read()

        machine = {
            'platform': platform.platform(),
            'processor': platform.processor()
        }

        html = template.format(results_json = json.dumps(self.results), machine_json = json.dumps(machine))

        f = open(os.path.join('reports', filename), 'w')
        f.write(html)
        f.close()