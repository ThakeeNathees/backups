
## The source is downloaded (and modified) from:
## https://gist.github.com/prideout/09af26cef84eef3e06a1e3f20a499a48
## Author: github.com/prideout

import http.server
import socketserver

import os
os.chdir('out')

PORT = 4000

Handler = http.server.SimpleHTTPRequestHandler
Handler.extensions_map.update({
    '.wasm': 'application/wasm',
})

socketserver.TCPServer.allow_reuse_address = True
with socketserver.TCPServer(("", PORT), Handler) as httpd:
    httpd.allow_reuse_address = True
    print("serving at port", PORT)
    httpd.serve_forever()