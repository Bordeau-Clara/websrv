#!/bin/bash

# 1. En-tête HTTP avec CRLF (le \r est crucial pour ton handler)
echo -ne "Content-Type: text/html; charset=utf-8\r\n"
echo -ne "\r\n"

# 2. Début du HTML
cat <<EOF
<!DOCTYPE html>
<html>
<head>
    <title>Bash CGI Monitor</title>
    <style>
        body { background: #1a1a1a; color: #ffca28; font-family: 'Monaco', monospace; padding: 20px; }
        .box { border: 1px dashed #ffca28; padding: 15px; margin-bottom: 20px; background: #263238; }
        h2 { color: #81d4fa; font-size: 1.1rem; border-bottom: 1px solid #444; }
        pre { color: #cddc39; overflow-x: auto; }
        .blink { animation: blink 1s infinite; }
        @keyframes blink { 0% { opacity: 0; } 50% { opacity: 1; } 100% { opacity: 0; } }
    </style>
</head>
<body>
    <h1><span class="blink">></span> BASH_CGI_SYSTEM_REPORT</h1>
EOF

# 3. Utilisation des commandes système Linux
echo "<div class='box'><h2>Disk Usage (df -h)</h2><pre>"
df -h | grep '^/dev/'
echo "</pre></div>"

echo "<div class='box'><h2>Memory (free -m)</h2><pre>"
free -m
echo "</pre></div>"

echo "<div class='box'><h2>Network (IP addresses)</h2><pre>"
hostname -I
echo "</pre></div>"

echo "<div class='box'><h2>Last 5 Logins</h2><pre>"
last -n 5
echo "</pre></div>"

# 4. Fin du HTML
cat <<EOF
    <p style="text-align:right; font-size: 0.8rem;">Script: $0 | PID: $$</p>
</body>
</html>
EOF
