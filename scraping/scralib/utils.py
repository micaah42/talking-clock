import os
from pathlib import Path


def get_data_folder():
    return os.path.join(Path.home(), 'socialdata')


def get_next(directory: str, prefix: str, suffix: str, shift: int = 1, number_only: bool = False):
    numbers = [-1]
    files = os.listdir(directory)
    for file in files:
        try:
            numbers.append(int(file.replace(prefix, '').replace(suffix, '')))
        except ValueError as e:
            print(f'could not parse number from: {directory}/{file}, {e}')
    return max(numbers) + shift if number_only else f'{prefix}{max(numbers) + shift}{suffix}'


def get_user_agents():
    agent_file = os.path.join(get_data_folder(), 'resources', 'chrome-agents.txt')
    with open(agent_file, 'r') as f:
        return f.readlines()
