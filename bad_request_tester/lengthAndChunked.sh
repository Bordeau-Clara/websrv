(printf "GET / HTTP/1.1\r\n";
"Host: localhost\r\n";
printf "Content-length:20\r\n";
printf "Transfer-encoding:chunked\r\n";
printf "\r\n") | nc localhost 8080
