import os
import json
import logging

from .fb import FacebookFetcher
from .reddit import RedditFetcher

def main():
    # choose scrapelist
    file = './scrapelist.json'
    if not os.path.isfile(file):
        file = '/etc/micaah42/scrapelist.json'
    
    # load
    with open(file, 'r') as f:
        fetchlist = json.load(f)
    print(fetchlist)

    # run fetches
    for fetch in fetchlist:
        if fetch['page'] == 'reddit':
            fetcher = RedditFetcher(fetch['source_name'])
        elif fetch['page'] == 'facebook':
            fetcher = FacebookFetcher(fetch['source_type'], fetch['souce_name'])
        else:
            logging.critical(f'invalid page: {fetch["page"]}')
            continue
        fetcher.download()
    
    
if __name__ == '__main__':
    main()