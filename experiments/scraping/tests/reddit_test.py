import sys

from pytz import utc
# sys.path.append('../src/scralib')

import unittest
from datetime import datetime, timedelta

from scralib.reddit import RedditFetcher


class TestFetchers(unittest.TestCase):
    def test_reddit(self):
        fetcher = RedditFetcher('Austria', datetime.now(utc) - timedelta(days=1))
        fetcher.download()

        fetcher = RedditFetcher('Austria')
        fetcher.download()
    

if __name__ == '__main__':
    unittest.main()
