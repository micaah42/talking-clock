# std
import os
import enum
import datetime

# pip
import pandas as pd
import facebook_scraper as fbs

# custom
import scralib.utils


def download(source: str, source_type: str,
             time_limit: datetime.datetime, fetch_directory: str,
             reactions: bool = True, comments: int = 0,
             posts_per_chunk: int = 50
             ):
    chunk_posts = []
    posts = pd.DataFrame()

    # setup fetch stuff options
    # ua = 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.0.0 Safari/537.36'
    # fbs.set_user_agent(ua)

    options = {
        'allow_extra_requests': comments != 0 and reactions,
        'reactions': reactions,
        'comments': comments,
        'posts_per_page': 100
    }

    cookies = os.path.join(scralib.utils.get_data_folder(), 'resources', 'ursula-zimmermann-cookies.json')

    if source_type == 'fbacc':
        post_iterator = fbs.get_posts(account=source, options=options, page_limit=100000, cookies=cookies)
    elif source_type == 'fbgrp':
        post_iterator = fbs.get_posts(group=source, options=options, page_limit=100000, cookies=cookies)
    elif source_type == 'fbhsh':
        post_iterator = fbs.get_posts(hashtag=source, options=options, page_limit=100000, cookies=cookies)
    else:
        raise ValueError('Invalid fb source type!')

    for i, post in enumerate(post_iterator):
        try:
            chunk_posts.append(post)
        except TypeError as e:
            print(e, post)

        print(f'\rdownloaded {i + 1} posts, last: {post["time"]}', end='')

        if i % posts_per_chunk == posts_per_chunk - 1 or post['timestamp'] < time_limit.timestamp():
            # append to the posts dataframe
            chunk = pd.DataFrame(chunk_posts)
            posts = pd.concat((posts, chunk))

            # make a new save
            if not os.path.isdir(fetch_directory):
                os.makedirs(fetch_directory, exist_ok=True)

            print(f'\nsaving to: {fetch_directory} ...')
            posts.to_csv(os.path.join(fetch_directory, 'posts.csv'), index=False)
            posts['text_length'] = posts['post_text'].apply(len)
            posts.describe().to_json(os.path.join(fetch_directory, 'meta.json'), indent=2)
            chunk_posts.clear()

            if post['timestamp'] < time_limit.timestamp():
                print(f'finished: {time_limit.isoformat(), time_limit.timestamp()} with post from {post["timestamp"]}')
                return
    print('done!')
