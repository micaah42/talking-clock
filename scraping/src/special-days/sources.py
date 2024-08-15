import bs4
import datetime

from utils import SpecialDay, SpecialDaySource


class UnitedNationsSource(SpecialDaySource):
    def __init__(self):
        super().__init__(
            name="United Nations International Days",
            filename="united-nations-days",
            links=["https://www.un.org/en/observances/list-days-weeks"],
        )

    def parse(self, soup: bs4.BeautifulSoup):

        attrs = {"class": lambda x: "views-row views-row-1" in x if x else False}
        entries = soup.find_all("div", attrs)

        def parse_entry(entry: bs4.BeautifulSoup):
            main_span = entry.find_next("span", {"class": "field-content"})
            name = main_span.find_next("a").text
            link = main_span.find_next("a").attrs["href"]

            date_div = entry.find_next("span", {"class": "date-display-single"})
            date = datetime.datetime.fromisoformat(date_div.attrs["content"]).date()
            date.year = 0
            return SpecialDay(date, name, "", link)

        return list(map(parse_entry, entries))


class KurioseFeiertageSource(SpecialDaySource):
    months = [
        "januar",
        "februar",
        "maerz",
        "april",
        "mai",
        "juni",
        "juli",
        "august",
        "september",
        "oktober",
        "november",
        "dezember",
    ]
    links = list(
        map(lambda x: f"https://www.kuriose-feiertage.de/kalender/{x}/", months)
    )

    def __init__(self):
        super().__init__("Kuriose Feiertage", "kuriose-feiertage", self.links)

    def parse(self, soup: bs4.BeautifulSoup, link: str):
        days = soup.find_all("h2")
        month = self.links.index(link) + 1

        def parse_entry(day: bs4.BeautifulSoup):
            print(day.text)
            date = datetime.date(4, month, int(day.text.split(".")[0]))
            day_list = day.next_sibling.find_all("a")

            return [
                SpecialDay(
                    date=date,
                    name=entry.text.split("\u2013")[0],
                    desc=entry.text.split("\u2013")[-1],
                    link=entry.attrs["href"],
                )
                for entry in day_list
            ]

        return sum(map(parse_entry, days[1:]), start=[])
