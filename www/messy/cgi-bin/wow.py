#!/usr/bin/env python3 -u
import os
import sys
import platform
import datetime
import subprocess

# On force le CRLF pour être ultra-compatible avec ton handler
CRLF = "\r\n"

def get_uptime():
    try:
        uptime = subprocess.check_output(['uptime', '-p']).decode().strip()
    except:
        uptime = "Non disponible"
    return uptime

# 1. Headers
print(f"Content-Type: text/html; charset=utf-8{CRLF}", end="")
print(CRLF, end="")

# 2. Le HTML/CSS
html_content = f"""
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Python CGI Dashboard</title>
    <style>
        :root {{
            --bg: #0f172a;
            --card: rgba(30, 41, 59, 0.7);
            --accent: #38bdf8;
            --text: #f1f5f9;
        }}
        body {{
            background: radial-gradient(circle at top left, #1e293b, #0f172a);
            color: var(--text);
            font-family: 'Segoe UI', system-ui, sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
            margin: 0;
        }}
        .container {{
            background: var(--card);
            backdrop-filter: blur(10px);
            border: 1px solid rgba(255, 255, 255, 0.1);
            padding: 2rem;
            border-radius: 24px;
            box-shadow: 0 25px 50px -12px rgba(0, 0, 0, 0.5);
            width: 90%;
            max-width: 500px;
            text-align: center;
        }}
        h1 {{
            color: var(--accent);
            font-size: 1.5rem;
            margin-bottom: 1.5rem;
            text-transform: uppercase;
            letter-spacing: 2px;
        }}
        .stat-grid {{
            display: grid;
            gap: 1rem;
        }}
        .stat-item {{
            background: rgba(15, 23, 42, 0.5);
            padding: 1rem;
            border-radius: 12px;
            display: flex;
            justify-content: space-between;
            align-items: center;
            border-left: 4px solid var(--accent);
        }}
        .label {{ opacity: 0.7; font-size: 0.9rem; }}
        .value {{ font-weight: bold; color: #fff; }}
        .pulse {{
            width: 10px;
            height: 10px;
            background: #22c55e;
            border-radius: 50%;
            display: inline-block;
            box-shadow: 0 0 10px #22c55e;
            animation: blink 2s infinite;
        }}
        @keyframes blink {{
            0%, 100% {{ opacity: 1; }}
            50% {{ opacity: 0.3; }}
        }}
    </style>
</head>
<body>
    <div class="container">
        <div style="margin-bottom: 1rem;"><span class="pulse"></span> LIVE CGI STATUS</div>
        <h1>System Monitor</h1>
        
        <div class="stat-grid">
            <div class="stat-item">
                <span class="label">OS</span>
                <span class="value">{platform.system()} {platform.release()}</span>
            </div>
            <div class="stat-item">
                <span class="label">Node</span>
                <span class="value">{platform.node()}</span>
            </div>
            <div class="stat-item">
                <span class="label">Python Version</span>
                <span class="value">{platform.python_version()}</span>
            </div>
            <div class="stat-item">
                <span class="label">Server Time</span>
                <span class="value">{datetime.datetime.now().strftime('%H:%M:%S')}</span>
            </div>
            <div class="stat-item">
                <span class="label">Uptime</span>
                <span class="value">{get_uptime()}</span>
            </div>
        </div>

        <p style="margin-top: 2rem; font-size: 0.8rem; opacity: 0.5;">
            Généré via execve() • Protocole CGI/1.1
        </p>
    </div>
</body>
</html>
"""
print(html_content)
