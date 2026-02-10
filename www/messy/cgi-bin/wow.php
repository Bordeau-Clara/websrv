#!/usr/bin/php
<?php
// En CGI, PHP gère souvent les headers tout seul, 
// mais on peut forcer le header avec la fonction header() ou un echo.
echo "Content-Type: text/html; charset=utf-8\r\n\r\n";
?>
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <style>
        body {
            background-color: #050505;
            color: #00ff41; /* Vert Matrix */
            font-family: 'Courier New', monospace;
            padding: 20px;
            line-height: 1.4;
        }
        .matrix-container {
            border: 1px solid #00ff41;
            padding: 20px;
            box-shadow: 0 0 15px #00ff41;
            background: rgba(0, 255, 65, 0.05);
        }
        h1 {
            text-align: center;
            text-transform: uppercase;
            letter-spacing: 5px;
            border-bottom: 2px solid #00ff41;
            padding-bottom: 10px;
        }
        .env-var {
            margin: 5px 0;
            display: flex;
        }
        .key { color: #fff; font-weight: bold; min-width: 250px; }
        .val { color: #00ff41; word-break: break-all; }
        .blink { animation: blinker 1s linear infinite; }
        @keyframes blinker { 50% { opacity: 0; } }
    </style>
</head>
<body>
    <div class="matrix-container">
        <h1><span class="blink">></span> PHP_CGI_EXECUTED</h1>
        
        <p><strong>Date du système :</strong> <?php echo date('Y-m-d H:i:s'); ?></p>
        <p><strong>PHP Version :</strong> <?php echo phpversion(); ?></p>
        
        <hr style="border: 0.5px solid #00ff41;">
        
        <h3>ENVIRONNEMENT CGI :</h3>
        <?php
        // On récupère toutes les variables d'environnement passées par le serveur
        $env_vars = [
            'SERVER_SOFTWARE', 'SERVER_NAME', 'GATEWAY_INTERFACE',
            'REQUEST_METHOD', 'QUERY_STRING', 'REMOTE_ADDR', 
            'HTTP_USER_AGENT', 'SCRIPT_NAME'
        ];

        foreach ($env_vars as $var) {
            $value = getenv($var) ?: 'NOT_SET';
            echo "<div class='env-var'>";
            echo "<span class='key'>$var :</span>";
            echo "<span class='val'>$value</span>";
            echo "</div>";
        }
        ?>
    </div>
</body>
</html>
