"""."""

import time
import datetime
import argparse
from dateutil.parser import parse

from fb import FacebookFetcher


def main(): 
    # source selection
    parser = argparse.ArgumentParser()
    parser.add_argument('--page', choices=['facebook', 'reddit'], required=True)
    parser.add_argument('--source_type', choices=['account', 'group'], required=True)
    parser.add_argument('--source_name', type=str, required=True)
    parser.add_argument('--time_limit', type=str)
    args = parser.parse_args()

    # apply additional parsing
    if type(args.time_limit) is str:
        args.time_limit = parse(args.time_limit).astimezone()
    print(f'{args=}')

    # set up fetcher
    if args.page == 'facebook':
        fetcher = FacebookFetcher(args.source_type, args.source_name, args.time_limit)
    else:
        raise Exception('Page not supported!')
        
    fetcher.download()

if __name__ == '__main__':
    main()