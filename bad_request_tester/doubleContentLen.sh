printf "GET / HTTP/1.1\r\nHost: localhost\r\nContent-length:20\r\nContent-Length:30\r\n\r\n" | nc localhost 8080
