#!/usr/bin/env python3
import cgi
import cgitb
import sys

# Active le débogage dans le navigateur (très utile en développement)
cgitb.enable()
sys.stdout.write("Content-Type: text/html; charset=utf-8\r\n")
sys.stdout.write("\r\n")  # Ligne vide obligatoire après l'en-tête

print("<html>")
print("<head><title>Mon premier CGI</title></head>")
print("<body>")
print("<h1>Salut depuis Python 3 !</h1>")
print("<p>Ceci est un script CGI généré dynamiquement.</p>")
print("</body>")
print("</html>")
