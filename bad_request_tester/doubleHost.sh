(printf "GET / HTTP/1.1\r\n";
printf "Host: localhost\r\n";
printf "Host: example.com\r\n";
printf "\r\n") | nc localhost 8080
