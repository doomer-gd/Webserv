#!/usr/bin/env python3

import os
import sys

print("Content-Type: text/html")
print()

method = os.environ.get("REQUEST_METHOD", "N/A")
content_length = int(os.environ.get("CONTENT_LENGTH", "0"))

body = ""
if content_length > 0:
    body = sys.stdin.read(content_length)

print("<html><head><title>CGI POST Test</title></head><body>")
print("<h1>CGI POST Result</h1>")
print("<p>Method: {}</p>".format(method))
print("<p>Content-Length: {}</p>".format(content_length))
print("<p>Body: {}</p>".format(body))
print("<p>Query String: {}</p>".format(os.environ.get("QUERY_STRING", "")))
print("</body></html>")
