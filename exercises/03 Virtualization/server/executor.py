#!/usr/bin/env python
import subprocess
from os import getcwd
from datetime import datetime

# Mapping language to command
interpreted_languages = {
    'python': ['python', '-c'],
    'sh': ['/bin/sh', '-c'],
    'js': ['node', '-e']
}

compiled_languages = {
    'cpp': ['g++', './a.out'],
    'c': ['gcc', './a.out']
}

def run(code, language):
    """Runs a piece of code in the specified language"""
    if (language not in interpreted_languages) and (language not in compiled_languages):
        return f'Invalid language, supporting {list(interpreted_languages.keys())}'
    
    try:
        # Run code in a docker container
        # Spreading the command to an array and using shell=False
        # to prevent shell injection which could affect the host
        if language in interpreted_languages:
            return subprocess.check_output(
                ['docker', 'container', 'run', '--rm', 'executor', *interpreted_languages[language], code],
                text=True,
                stderr=subprocess.STDOUT
            )
        else:
            cwd = getcwd()
            # Creating a sufficiently unique filename
            stamp = datetime.timestamp(datetime.now())
            filename = f'{stamp}.{language}'
            # Write the file that shall be compiled & run
            # VSCode: Set "cwd": "${fileDirname}" in launch.json for debugging
            with open(f'{cwd}/src/{filename}', 'w') as file:
                file.write(code)
            [compiler, run_cmd] = compiled_languages[language][0:2]
            #run_cmd = compiled_languages[language][1]
            return subprocess.check_output(
                ['docker', 'container', 'run', '--rm', '-v', f'{cwd}/src:/src', 'executor', '/bin/sh', '-c', f'{compiler} /src/{filename} && {run_cmd}'],
                text=True,
                stderr=subprocess.STDOUT
            )
    except subprocess.CalledProcessError as error:
        # Return stdout on error
        return f'{error.output}\n\nExited with status code {error.returncode}'

if __name__ == '__main__':
    # Demonstrating injection if shell=True
    stuff = 'injection\ninnocence" | grep "t'
    print(subprocess.check_output(f'echo "{stuff}"', shell=True))
    print (run('hi', 'lolcode'))
    print(run('print("hi")', 'python'))
