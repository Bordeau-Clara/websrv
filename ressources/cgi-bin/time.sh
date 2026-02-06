#!/bin/sh
# CGI script that displays a live clock initialized from the server time.
# This script prints the HTTP header then a nicer HTML page with
# JavaScript that updates the clock every second and provides a simple UI.

# Get server timestamp (seconds since epoch)
server_ts=$(date +%s)

printf "Content-Type: text/html\r\n"
printf "Cache-Control: no-cache, no-store, must-revalidate\r\n"
printf "Pragma: no-cache\r\n"
printf "Expires: 0\r\n\r\n"

cat <<EOF
<!doctype html>
<html>
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width,initial-scale=1">
    <title>Server Time — Webserv</title>
    <style>
        .btn-home {
            position: fixed;
            top: 20px;
            left: 20px;
            padding: 10px 20px;
            background: rgba(255, 255, 255, 0.1);
            color: #eef7f8;
            border: 1px solid rgba(255, 255, 255, 0.3);
            border-radius: 8px;
            cursor: pointer;
            font-size: 14px;
            font-weight: 600;
            text-decoration: none;
            display: inline-block;
            transition: all 0.2s ease;
            box-shadow: 0 2px 8px rgba(0, 0, 0, 0.3);
            z-index: 1000;
        }
        .btn-home:hover {
            background: rgba(255, 255, 255, 0.2);
            box-shadow: 0 4px 12px rgba(0, 0, 0, 0.4);
            transform: translateY(-2px);
        }
        body {
            margin: 0;
            display: flex;
            min-height: 100vh;
            align-items: center;
            justify-content: center;
            background: linear-gradient(135deg, #0f2027 0%, #203a43 50%, #2c5364 100%);
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
            color: #eef7f8;
        }
        .card {
            background: rgba(255, 255, 255, 0.06);
            border-radius: 12px;
            padding: 28px 36px;
            box-shadow: 0 8px 24px rgba(0,0,0,0.4);
            max-width: 520px;
            width: 100%;
            text-align: center;
        }
        h1 { margin: 0 0 12px 0; font-size: 20px; letter-spacing: 0.5px; }
        .time {
            font-family: 'Courier New', Courier, monospace;
            font-weight: 700;
            font-size: 44px;
            margin: 8px 0 12px 0;
            color: #ffffff;
            text-shadow: 0 3px 12px rgba(0,0,0,0.35);
        }
        .meta { font-size: 12px; color: rgba(255,255,255,0.8); }
        .row { display: flex; justify-content: space-between; margin-top: 12px; }
        .small { font-size: 12px; color: rgba(255,255,255,0.7); }
        .toggle { border: none; background: rgba(255,255,255,0.06); color: #fff; padding: 6px 12px; border-radius: 6px; cursor: pointer; }
        .footer { margin-top: 14px; font-size: 11px; color: rgba(255,255,255,0.6); }
    </style>
</head>
<body>
    <a href="/" class="btn-home" onclick="window.location.href='/'; return false;">← Accueil</a>
    <div class="card">
        <h1>Server Time</h1>
        <div id="clock" class="time">--:--:--</div>
    </div>

    <script>
    (function(){
        var serverTs = Number($server_ts) * 1000; // milliseconds
        var dt = new Date(serverTs);
        var showingLocal = false; // toggle between server and local
        function pad(n){ return (n<10? '0'+n : n); }
        function formatDate(d){
            var y = d.getFullYear();
            var m = pad(d.getMonth()+1);
            var day = pad(d.getDate());
            var hh = pad(d.getHours());
            var mm = pad(d.getMinutes());
            var ss = pad(d.getSeconds());
            return y+'-'+m+'-'+day+' '+hh+':'+mm+':'+ss;
        }

        var elClock = document.getElementById('clock');
        var elServerStamp = document.getElementById('serverStamp');
        var elLocalStamp = document.getElementById('localStamp');
        var elToggle = document.getElementById('toggleBtn');

        function render(){
            elClock.textContent = formatDate(dt);
        }

        function tick(){
            dt.setSeconds(dt.getSeconds()+1);
            render();
        }

        // Start ticking using setInterval
        render();
        setInterval(tick, 1000);

        // Toggle server/local display of the big time when clicking toggle
        elToggle.addEventListener('click', function(){
            showingLocal = !showingLocal;
            if (showingLocal) {
                var now = new Date();
                elClock.textContent = formatDate(now);
                elToggle.textContent = 'Show Server Time';
            } else {
                elClock.textContent = formatDate(dt);
                elToggle.textContent = 'Show Browser Time';
            }
        });
    })();
    </script>
    
    <noscript>
        <div style="text-align:center; margin-top:12px; color:#ddd;">JavaScript is required to show a live clock; this page will show server time only.</div>
    </noscript>
</body>
</html>
EOF

exit 0

