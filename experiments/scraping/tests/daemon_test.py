import json

from pytz import utc
# sys.path.append('../src/scralib')

import unittest
from datetime import datetime, timedelta

from scralib.reddit import RedditFetcher
from scralib.daemon import main

class TestFetchers(unittest.TestCase):
    def test_reddit(self):
        with open('./scrapelist.json', 'w') as f:
            json.dump([{'page': 'reddit', 'source_name': 'okoidawappler'}, {'page': 'reddit', 'source_name': 'aeiou'}], f)
        
        main()
    

if __name__ == '__main__':
    unittest.main()
