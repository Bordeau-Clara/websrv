printf "GET / HTTP/1.1\r\nHost: localhost\r\nTransfer-encoding:gzip\r\n\r\n" | nc localhost 8080
