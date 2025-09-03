#!/usr/bin/env python

from sqlalchemy import create_engine
from pathlib import Path
from tap import Tap
import pandas as pd
from threading import Thread


class CSV2SQLParser(Tap):
    output_file: Path
    input_file: Path
    table_name: str
    rename_json: Path | None = None
    chunksize: int = 2**17
    num_jobs: int = 4
    sep: str = "\t"


def convert_csv_2_sqlite(args: CSV2SQLParser):
    engine = create_engine(f"sqlite:///{args.output_file}")

    chunks = pd.read_csv(
        args.input_file,
        sep=args.sep,
        chunksize=args.chunksize,
        low_memory=False,
        on_bad_lines="warn",
    )

    def write_chunk(chunk: pd.DataFrame, append=True):
        columns = {column: column.replace("-", "_") for column in chunk.columns}
        chunk = chunk.rename(columns=columns)
        chunk.to_sql(
            args.table_name, con=engine, if_exists="append" if append else "replace"
        )
        print(f"wrote {len(chunk)} lines")

    for chunk in chunks:
        write_chunk(chunk, False)
        break

    with Pool(4) as p:
        for chunk in chunks:
            Thread(target=write_chunk, args=(chunk,)).start()


def main():
    parser = CSV2SQLParser()
    args = parser.parse_args()
    print(args)

    convert_csv_2_sqlite(args)


if __name__ == "__main__":
    main()
