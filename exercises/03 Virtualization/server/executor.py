#!/usr/bin/env python
import subprocess

# Mapping language to command
languages = {
    'python': ['python', '-c'],
    'sh': ['/bin/sh', '-c'],
    'js': ['node', '-e']
}

def run(code, language):
    """Runs a piece of code in the specified language"""
    if (language not in languages):
        return f'Invalid language, supporting {list(languages.keys())}'
    
    try:
        # Run code in a docker container
        # Spreading the command to an array and using shell=False
        # to prevent shell injection which could affect the host
        return subprocess.check_output(
            ['docker', 'container', 'run', '--rm', 'executor', *languages[language], code],
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
