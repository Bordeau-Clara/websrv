(printf "GET / HTTP/1.1\r\n";
printf "Host: localhost\r\n";
printf "Transfer-encoding:gzip\r\n";
printf "\r\n") | nc localhost 8080
