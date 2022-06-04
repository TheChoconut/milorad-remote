from flask import Flask, send_from_directory
from flask_sock import Sock
import serial
import json

app = Flask(__name__, static_url_path='', static_folder='frontend/build')
sock = Sock(app)
ser = serial.Serial('/dev/ttyACM0', 9600)

@sock.route('/main')
def mainRoute(ws):
    while True:
        jsonEncoded = ws.receive()
        obj = json.loads(jsonEncoded)
        if obj['cmd'] == 'direct':
            ser.write(obj['val'].encode() + b'\r\n')
        elif obj['cmd'] == 'speed':
            ser.write(b'S' + bytes([obj['val']]))

@app.route('/')
def root():
    return send_from_directory('frontend/build', 'index.html')