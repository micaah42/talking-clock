import os
import abc
import json
import logging
import datetime
import configparser
from pytz import UTC
from typing import Any, Iterable

import numpy as np
import pandas as pd

from scralib.configuration import Configuraton


CONFIG_FILE = os.path.join('resources' if os.getenv('HOME') else '/data', 'scrape.conf')

class Fetcher(abc.ABC):
    def __init__(self, page: str, source_type: str, source_name: str, time_limit = None) -> None:
        self.folder = os.path.join('resources', page, source_type, source_name)
        self.file = get_next(self.folder, 'fetch', '.csv')
        self.time_limit = time_limit if time_limit is not None else self.read_timestamp()
        self.chunk_size = 5

    @abc.abstractmethod
    def get_generator(self) -> Iterable[Any]:
        pass

    @abc.abstractmethod
    def get_time(self, post: Any) -> datetime.datetime:
        pass

    @abc.abstractmethod
    def sanitize(self, post: Any) -> Any:
        pass

    def read_timestamp(self):
        timestamp_file = os.path.join(self.folder, 'timestamp')
        if not os.path.isfile(timestamp_file):
            return datetime.datetime.now(UTC) - datetime.timedelta(days=1)
        with open(timestamp_file, 'r') as f:
            time_limit = datetime.datetime.fromtimestamp(float(f.read()))
            time_limit = time_limit.astimezone()
            logging.info(f'{time_limit=}')
            return time_limit

    def save(self, items: Any):
        # nothing todo if nothing to save
        if len(items) == 0:
            logging.warning('nothing to save!')
            return
        
        # prepare
        data_frame = pd.DataFrame(items)
        os.makedirs(self.folder, exist_ok=True)

        # save data
        file = os.path.join(self.folder, self.file)
        file_exists = os.path.isfile(file)
        data_frame.to_csv(file, mode=('a' if file_exists else 'w'), header=(not file_exists))
        logging.warning(f'saved {len(items)}: {self.file}, {data_frame["timestamp"].min()}')

    def download(self):
        logging.warning(f'downloading {self.folder}, {self.file}')
        
        # update last timestamp (next time we fetch until we reached this timestamp to avoid duplicates)
        os.makedirs(self.folder, exist_ok=True)
        file = os.path.join(self.folder, 'timestamp')
        last_timestamp = str(datetime.datetime.now().timestamp())
        with open(file, 'w') as f:
            f.write(last_timestamp)
            logging.warning(f'updated timestamp: {last_timestamp}')

        items = []
        for i, x in enumerate(self.get_generator()):
            item = self.sanitize(x)
            items.append(item)
            logging.info(item)

            if 0 == (i + 1) % self.chunk_size:
                self.save(items)
                items.clear()

            if self.get_time(x) < self.time_limit:
                print(f'Reached time limit: {self.time_limit.isoformat()}')
                self.save(items)
                return            
        

def get_folder(page: str, source_type: str, source_name: str):
    """returns the folder in which all downloads will be placed"""
    config = Configuraton()
    root = config.get('path', 'root')
    return os.path.join(root, page, f'{source_name}.{source_type}')


def get_next(directory: str, prefix: str, suffix: str, shift: int = 1):
    """get (last + shift)th item in numbered list of files in directory, like file0.txt, ..., fileN.txt"""
    numbers = [-1]
    files = os.listdir(directory) if os.path.isdir(directory) else []
    files = filter(lambda x: x.startswith(prefix) and x.endswith(suffix), files)
    for file in files:
        try:
            numbers.append(int(file.replace(prefix, '').replace(suffix, '')))
        except ValueError as error:
            print(f'could not parse number from: {directory}/{file}, {error}')
    return f'{prefix}{max(numbers) + shift}{suffix}'
