#!/usr/bin/env python3
# CGI script (Python) that renders a styled server time page.

import time
import sys
from datetime import datetime

server_ts = int(time.time())

page_template = """
<!doctype html>
<html>
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width,initial-scale=1">
    <title>Server Time — Python CGI</title>
    <style>
        .btn-home {
            position: fixed;
            top: 20px;
            left: 20px;
            padding: 10px 20px;
            background: rgba(255, 255, 255, 0.95);
            color: #d84315;
            border: none;
            border-radius: 8px;
            cursor: pointer;
            font-size: 14px;
            font-weight: 600;
            text-decoration: none;
            display: inline-block;
            transition: all 0.2s ease;
            box-shadow: 0 2px 8px rgba(0, 0, 0, 0.15);
            z-index: 1000;
        }
        .btn-home:hover {
            background: white;
            box-shadow: 0 4px 12px rgba(0, 0, 0, 0.25);
            transform: translateY(-2px);
        }
        body {
            margin: 0; display:flex; min-height:100vh; align-items:center; justify-content:center;
            background: linear-gradient(135deg, #FCE38A 0%, #F38181 100%);
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
            color: #222;
        }
        .card {
            background: rgba(255,255,255,0.95); border-radius:12px; padding:28px 36px; max-width:540px; width:100%; text-align:center;
            box-shadow:0 10px 30px rgba(0,0,0,0.12);
        }
        h1 { margin:0 0 10px 0; font-size:20px; color:#222; }
        .time { font-family:'Courier New', Courier, monospace; font-weight:700; font-size:42px; color:#d84315; margin:8px 0; }
        .date { font-size:16px; font-weight:600; color:#333; margin-bottom:4px; }
        .meta { font-size:13px; color:#666; margin-bottom:16px; }
        .controls { display:flex; gap:10px; justify-content:center; margin-top:20px; border-top:1px solid #eee; padding-top:20px; }
        .btn { border:1px solid #ddd; background:#fff; padding:8px 14px; border-radius:6px; cursor:pointer; font-size:13px; transition:all 0.2s; }
        .btn:hover { background:#f9f9f9; border-color:#ccc; }
    </style>
</head>
<body>
    <a href="/" class="btn-home" onclick="window.location.href='/'; return false;">← Accueil</a>
    <div class="card">
        <h1>Server Time (Python CGI)</h1>
        
        <div id="dow" class="date">--</div>
        <div id="clock" class="time">--:--:--</div>
        <div id="date" class="meta">--</div>

        <div class="controls">
            <button id="btnFormat" class="btn">Use 12h</button>
        </div>
    </div>

    <script>
    (function(){
        // Récupération du timestamp injecté par Python
        var serverTs = Number(__SERVER_TS__) * 1000;
        var dt = new Date(serverTs);
        var use12 = false;
        var source = 'server'; // 'server' or 'client'

        function pad(n) { return (n<10? '0'+n : n); }
        
        function formatDate(d) {
            var hh = d.getHours(); var ampm = '';
            if (use12) { ampm = (hh>=12) ? ' PM' : ' AM'; hh = (hh%12) || 12; }
            var mm = pad(d.getMinutes()); var ss = pad(d.getSeconds());
            return ((use12) ? ((hh<10? '0'+hh : hh)) : pad(d.getHours())) + ':' + mm + ':' + ss + ampm;
        }

        var elClock = document.getElementById('clock');
        var elDate = document.getElementById('date');
        var elDow = document.getElementById('dow');
        var btnFormat = document.getElementById('btnFormat');
        function render() {
            var current = (source === 'server') ? dt : new Date();
            
            // Mise à jour de l'heure
            elClock.textContent = formatDate(current);
            
            // Mise à jour de la date (YYYY-MM-DD)
            elDate.textContent = current.getFullYear() + '-' + pad(current.getMonth()+1) + '-' + pad(current.getDate());
            
            // Mise à jour du jour de la semaine
            elDow.textContent = current.toLocaleDateString(undefined, { weekday: 'long' });
        }

        function tick() { 
            // Si on est en mode "serveur", on ajoute 1 seconde manuellement à chaque tick
            if (source === 'server') dt.setSeconds(dt.getSeconds() + 1); 
            render(); 
        }

        // Lancement initial
        render(); 
        var timer = setInterval(tick, 1000);

        // Events listeners (ne plantent plus car les boutons existent maintenant)
        if(btnFormat) btnFormat.addEventListener('click', function(){ use12 = !use12; btnFormat.textContent = use12? 'Use 24h' : 'Use 12h'; render(); });
    })();
    </script>

    <noscript>
        <div style="text-align:center; margin-top:12px; color:#666;">JavaScript is required to show a live clock. Reload to sync base time.</div>
    </noscript>
</body>
</html>
"""

page = page_template.replace('__SERVER_TS__', str(server_ts))

# Print success headers then page
sys.stdout.write('Content-Type: text/html\r\n')
sys.stdout.write('Cache-Control: no-cache, no-store, must-revalidate\r\n')
sys.stdout.write('Pragma: no-cache\r\n')
sys.stdout.write('Expires: 0\r\n\r\n')
sys.stdout.write(page)
sys.exit(0)
