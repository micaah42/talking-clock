# std
import os
import datetime
from pytz import UTC

# pip
import pandas as pd
import facebook_scraper as fbs

# custom
from fetcher import Fetcher
from configuration import Configuraton

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

def download(source_name: str, source_type: str, time_limit: datetime.datetime, reactions: bool = True, comments: int = 0):
    chunk_posts = []
    posts_per_chunk = 5
    posts = pd.DataFrame()



    # cookies = os.path.join(scralib.utils.get_folder(), 'resources', 'cookies.txt')
    source_spec = {source_type: source_name}

    post_iterator = None

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
