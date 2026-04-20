#!/usr/bin/env python

from os.path import dirname, join
import pandas as pd
import argparse
import datetime
import logging
import ollama
import json

ACTION_DAYS_FILE = join(
    dirname(__file__),
    "..",
    "actiondays",
    "sources",
    "actiondays-v2.csv",
)

class Prompt:
    pre_prompt: str
    post_prompt: str


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "--actiondays_file",
        default=ACTION_DAYS_FILE,
        help="Path to actiondays csv file.",
    )

    parser.add_argument(
        "--pre_prompt",
        default="You are a clock and an alarm has gone off.",
        help="Static start string for the prompt.",
    )

    parser.add_argument(
        "--post_prompt",
        default="Write a summary in natural language as if you were a mediaval knight's assistant.",
        help="Static end string for the prompt.",
    )

    parser.add_argument(
        "--skip_actiondays",
        action="store_true",
        help="Skip actiondays infos in prompt.",
    )

    parser.add_argument(
        "--alarm_name",
        default="Wake up man!",
        help="Alarm name passed in the prompt.",
    )
    parser.add_argument(
        "--alarm_time",
        default=datetime.datetime.now().strftime('%H:%M'),
        help="Alarm start time passed in the prompt.",
    )

    parser.add_argument(
        "--model",
        default="qwen3.5:0.8b",
        help="Ollama model to use.",
    )

    args = parser.parse_args()
    logging.info(f"args: {vars(args)}")

    prompt = args.pre_prompt

    if args.alarm_name is not None:
        prompt += f'\n\nThe name of the alarm is "{args.alarm_name}".'
    if args.alarm_time is not None:
        prompt += f'\nIt is scheduled at "{args.alarm_time}".'


    if not args.skip_actiondays:
        actiondays = pd.read_csv(ACTION_DAYS_FILE)
        today = datetime.date.today().strftime("%m-%d")
        actiondays_today = actiondays[actiondays["date"] == today]
        prompt += "\n\nThese are some action days taking place today, reference them as needed but don't just list them:\n"
        prompt += "\n".join(
            actiondays_today.apply(
                lambda day: f'  * {day["name"]}: {day["desc"]}', axis=1
            )
        )

    if args.post_prompt is not None:
        prompt += f'\n\n{args.post_prompt}'

    logging.info(f'prompt:\n{prompt}')
    stream = ollama.generate(
        model=args.model,
        prompt=prompt,
        options={'temperature': 1.1},
        keep_alive='10m',
        stream=True,
    )

    for chunk in stream:
        print(chunk["response"], end="", flush=True)

    print("\n")

if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    main()
