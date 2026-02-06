#!/usr/bin/env python3
"""Simple CGI test script."""
import os, sys, time

# CGI header - Content-Type is required
print("Content-Type: text/html")
print()  # Empty line separates headers from body

# HTML body
print("<html>")
print("<head><title>CGI Test</title></head>")
print("<body>")
print("<h1>CGI Test - Python</h1>")
print("<h2>Environment Variables:</h2>")
print("<ul>")
for key in sorted(os.environ.keys()):
    if key.startswith(('SCRIPT', 'QUERY', 'REQUEST', 'CONTENT', 'PATH', 'SERVER', 'GATEWAY')):
        print(f"<li><b>{key}</b>: {os.environ[key]}</li>")
time.sleep(100)
print("</ul>")
print("</body>")
print("</html>")
