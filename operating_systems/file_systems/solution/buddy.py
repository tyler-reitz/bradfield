"""
A coprocess to lifefs.c that does stuff that's easy in Python,
like making HTTP requests, parsing JSON and maintaing an LRU.
"""

from datetime import datetime
import json
import os
from urllib import request
import socket

API_URL = "https://api.opentreeoflife.org/v3/tree_of_life/subtree"

logf = open('./log', 'a')


def log(s):
    logf.write('\x1b[36m')  # cyan
    logf.write(datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
    logf.write(': ')
    logf.write(s)
    logf.write('\x1b[0m')  # reset
    logf.flush()


path_to_id = {b'/': 'ott93302'}
children = {}


def fetch_children(path):
    """
    Given path as a byte sequence, return a list of child names,
    also in bytes. Retrieve this from the opentreeoflife API but
    cache responses.

    TODO if no current path_to_id mapping, traverse the tree
    """
    try:
        return children[path]
    except KeyError:
        pass

    data = (
        '{"node_id": "%s", "height_limit": 1, "format": "arguson"}'
        % path_to_id[path])
    req = request.Request(API_URL, data=data.encode('utf-8'))
    resp = request.urlopen(req)

    child_names = []
    for ch in json.loads(resp.read())['arguson']['children']:
        log(str(ch))
        try:
            name_bytes = ch['taxon']['name'].encode('utf-8')
        except KeyError:
            continue
        path_to_id[os.path.join(path, name_bytes)] = ch['node_id']
        child_names.append(name_bytes)
    children[path] = child_names
    return child_names


if __name__ == '__main__':
    log('Buddy starting\n')
    s = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)
    addr = '/tmp/lifesock-server'
    if os.path.exists(addr):
        os.remove(addr)
    s.bind(addr)
    s.connect('/tmp/lifesock-client')

    while True:
        path = s.recv(4096)
        log(f'Received {path}\n')
        for child_name in fetch_children(path):
            s.send(child_name)
        s.send(b'.')
