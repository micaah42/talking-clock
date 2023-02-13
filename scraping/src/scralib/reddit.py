# std
import datetime

# pip
import praw
from pytz import UTC

# custom
from .fetcher import Fetcher

reddit = praw.Reddit(
    client_id="FbVRc_1fSEiE8g",
    client_secret="-1d59vCdJNcvcoscPEUrkmJvbg89vg",
    user_agent="the_rush_dude",
)


class RedditFetcher(Fetcher):
    def __init__(self, source_name: str, time_limit = None):
        super().__init__('reddit', 'subreddit', source_name, time_limit)
        self.generator = reddit.subreddit(source_name).new(limit=None)

    def get_generator(self):
        return self.generator

    def sanitize(self, post):
        post_data = vars(post)
        post_data['timestamp'] = post.created_utc
        post_data['text'] = post.selftext
        return post_data

    def get_time(self, post) -> datetime.datetime:
        return datetime.datetime.fromtimestamp(post.created_utc, UTC)
