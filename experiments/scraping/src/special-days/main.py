from sources import UnitedNationsSource, KurioseFeiertageSource
from utils import SpecialDay
import csv


def main():
    sources = [
        # UnitedNationsSource()
        KurioseFeiertageSource()
    ]

    for source in sources:
        days = source.fetch()
        print(days)

        with open(f"{source.filename}.csv", "w") as file:
            writer = csv.DictWriter(file, list(SpecialDay.__annotations__))
            writer.writerows((vars(day) for day in days))


if __name__ == "__main__":
    main()
