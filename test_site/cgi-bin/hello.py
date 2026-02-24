#!/usr/bin/env python3

import os

print("Content-Type: text/html")
print()
print("<html><head><title>CGI Test</title></head><body>")
print("<h1>Hello from CGI!</h1>")
print("<p>Request Method: {}</p>".format(os.environ.get("REQUEST_METHOD", "N/A")))
print("<p>Query String: {}</p>".format(os.environ.get("QUERY_STRING", "N/A")))
print("<p>Server Name: {}</p>".format(os.environ.get("SERVER_NAME", "N/A")))
print("<p>Server Port: {}</p>".format(os.environ.get("SERVER_PORT", "N/A")))
print("</body></html>")
