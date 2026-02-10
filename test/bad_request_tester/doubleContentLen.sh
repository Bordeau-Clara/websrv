(printf "GET / HTTP/1.1\r\n";
printf "Host: localhost\r\n";
printf "Content-length:20\r\n";
printf "Content-Length:30\r\n";
printf "\r\n") | nc localhost 8080
