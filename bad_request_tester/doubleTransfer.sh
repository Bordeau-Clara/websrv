printf "GET / HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding:chunked\r\nTransfer-encoding:chunked\r\n\r\n" | nc localhost 8080
