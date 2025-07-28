import dataclasses
import datetime
import requests
import bs4
import re


def sanitize_string(string: str):
    string = string.replace("-", " ")
    re.sub("\s\s+", " ", string)
    return string.strip()


@dataclasses.dataclass
class SpecialDay:
    date: datetime.date
    name: str
    desc: str
    link: str

    def __post_init__(self):
        self.name = sanitize_string(self.name)
        self.desc = sanitize_string(self.desc)


class SpecialDaySource:
    def __init__(self, name, filename, links):
        self.fetch_time: datetime.datetime = None
        self.links: list[str] = links
        self.filename: str = filename
        self.name: str = name

    def fetch(self):
        headers = {
            "User-Agent": "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36"
        }

        days = []
        for link in self.links:
            resp = requests.get(link, headers=headers)
            soup = bs4.BeautifulSoup(resp.content)
            days.append(self.parse(soup, link))

        self.fetch_time = datetime.datetime.now()
        return sum(days, start=[])

    def parse(soup: bs4.BeautifulSoup, link: str):
        pass
