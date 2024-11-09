import secrets
import json
import os

import jwt
from flask import Flask, abort, make_response


app = Flask(__name__)
pins = set()


with open('/home/michael/talking-clock/clock-web/nginx/key.jwk', 'r') as f:
    keys = json.load(f)['keys']
    SECRET = keys[0]['k']



@app.route(
    "/pin",
)
def pin():
    pin = secrets.token_urlsafe(8)

    while pin in pins:
        pin = secrets.token_urlsafe(8)

    pins.add(pin)
    return pin


@app.route(
    "/login/<pin>",
)
def login(pin):
    if not pin in pins:
        abort(403)

    pins.remove(pin)
    token = jwt.encode({}, SECRET)
    
    response = make_response()
    response.set_cookie("Bearer", value=token)
    return response

if __name__ == "__main__":
    app.run()