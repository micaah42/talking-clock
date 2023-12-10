import os
import datetime
import configparser
from typing import Any


class Configuration:
    def __init__(self):

        # read the file
        self.parser = configparser.ConfigParser()
        self.parser.read('scraping.ini', f'{os.getenv("HOME")}/.config/micaah42/scraping.ini', '/etc/scraping.ini')

        self.set('DEFAULT', 'last_access', datetime.datetime.now().isoformat())

    def get(self, section: str, option: str, default: Any = None) -> Any:
        try:
            value = self.parser.get(section, option, fallback=default)
            return value
        except configparser.NoSectionError:
            return default

    def set(self, section: str, option: str, value: Any) -> Any:
        try:
            self.parser.set(section, option, value)
            with open(self.file, 'w') as f:
                self.parser.write(f)
        except configparser.NoSectionError:
            self.parser.add_section(section)
            self.set(section, option, value)
