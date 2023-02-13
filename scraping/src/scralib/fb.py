# std
import os
import datetime
from pytz import UTC

# pip
import pandas as pd
import facebook_scraper as fbs

# custom
from .fetcher import Fetcher
from .configuration import Configuraton

class FacebookFetcher(Fetcher):
    """."""
    
    def __init__(self, source_type: str, source_name: str, time_limit = None) -> None:
        super().__init__('facebook', source_type, source_name, time_limit)
        source = {source_type: source_name}
        self.generator = fbs.get_posts(**source, page_limit=None, cookies=Configuraton().get('PATHS', 'cookies'))

    def sanitize(self, post):
        cleanser = lambda x: x.replace('\n', '\\n') if type(x) is str else x
        return { key: cleanser(value) for key, value in post.items()}

    def get_time(self, post: fbs.Post) -> datetime.datetime:
        return datetime.datetime.fromtimestamp(post['timestamp'], UTC)

    def get_generator(self):
        return self.generator
