#!/usr/bin/sh

cd "$(dirname "$0")"

. venv/bin/activate
gunicorn -b 127.0.0.1:21121 app:app
