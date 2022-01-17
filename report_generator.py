from datetime import datetime
import os, platform, subprocess
import webbrowser
import json

class ReportGenerator:
    def __init__(self):
        self.results = {}
    
    def get_processor_name(self):
        if platform.system() == 'Windows':
            return platform.processor()
        elif platform.system() == 'Darwin':
            os.environ['PATH'] = os.environ['PATH'] + os.pathsep + '/usr/sbin'
            command = 'sysctl -n machdep.cpu.brand_string'
            return subprocess.check_output(command).strip()
        elif platform.system() == 'Linux':
            command = 'cat /proc/cpuinfo'
            all_info = str(subprocess.check_output(command, shell=True).strip())
            return all_info.split(r'model name\t: ')[1].split(r'\n')[0]
        
        return ''

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
            'processor': self.get_processor_name()
        }

        html = template.replace('{results_json}', json.dumps(self.results))
        html = html.replace('{machine_json}', json.dumps(machine))
        html = html.replace('{date}', str(datetime.now()))

        f = open(os.path.join('reports', filename), 'w')
        f.write(html)
        f.close()

        webbrowser.open(os.path.abspath(os.path.join('reports', filename)))