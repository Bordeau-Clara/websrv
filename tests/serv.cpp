#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sstream>

std::vector<int> http_clients;
std::vector<pollfd> poll_fds;
std::string last_message;

void send_to_all_http_clients(const std::string& message) {
    for (int fd : http_clients) {
        std::string response = "data: " + message + "\n\n";
        send(fd, response.c_str(), response.size(), 0);
    }
    last_message = message;
}

void handle_http_request(int client_fd) {
    const char* headers =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n";
    const char* html =
        "<!DOCTYPE html><html><head><title>Messages en temps réel</title>"
        "<script>"
        "const eventSource = new EventSource('/events');"
        "eventSource.onmessage = (e) => {"
        "  document.body.innerHTML += '<p>' + e.data + '</p>';"
        "};"
        "</script>"
        "</head><body><h1>Messages en temps réel</h1></body></html>";
    send(client_fd, headers, strlen(headers), 0);
    send(client_fd, html, strlen(html), 0);
}

void handle_sse_request(int client_fd) {
    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/event-stream\r\n"
        "Cache-Control: no-cache\r\n"
        "Connection: keep-alive\r\n\r\n";
    send(client_fd, response.c_str(), response.size(), 0);
    if (!last_message.empty()) {
        std::string event = "data: " + last_message + "\n\n";
        send(client_fd, event.c_str(), event.size(), 0);
    }
    http_clients.push_back(client_fd);
}

void run_server() {
    // Socket HTTP (port 8080)
    int http_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in http_addr = {AF_INET, htons(8080), INADDR_ANY};
    bind(http_fd, (struct sockaddr*)&http_addr, sizeof(http_addr));
    listen(http_fd, 5);

    // Socket TCP (port 8081)
    int tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in tcp_addr = {AF_INET, htons(8081), INADDR_ANY};
    bind(tcp_fd, (struct sockaddr*)&tcp_addr, sizeof(tcp_addr));
    listen(tcp_fd, 5);

    poll_fds.push_back({http_fd, POLLIN, 0});
    poll_fds.push_back({tcp_fd, POLLIN, 0});

    while (true) {
        int ret = poll(poll_fds.data(), poll_fds.size(), -1);
        if (ret < 0) {
            std::cerr << "Erreur poll()" << std::endl;
            break;
        }

        for (size_t i = 0; i < poll_fds.size(); ++i) {
            if (poll_fds[i].revents & POLLIN) {
                if (poll_fds[i].fd == http_fd) {
                    // Nouvelle connexion HTTP
                    sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_fd = accept(http_fd, (struct sockaddr*)&client_addr, &client_len);
                    if (client_fd < 0) continue;

                    char buffer[1024] = {0};
                    recv(client_fd, buffer, sizeof(buffer), 0);
                    std::string request(buffer);

                    if (request.find("/events") != std::string::npos) {
                        handle_sse_request(client_fd);
                        poll_fds.push_back({client_fd, POLLOUT, 0});
                    } else {
                        handle_http_request(client_fd);
                        close(client_fd);
                    }
                } else if (poll_fds[i].fd == tcp_fd) {
                    // Nouvelle connexion TCP (message depuis terminal)
                    sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_fd = accept(tcp_fd, (struct sockaddr*)&client_addr, &client_len);
                    if (client_fd < 0) continue;

                    char buffer[1024] = {0};
                    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
                    if (bytes_received > 0) {
                        std::string message(buffer, bytes_received);
                        std::cout << "Message reçu : " << message << std::endl;
                        send_to_all_http_clients(message);
                    }
                    close(client_fd);
                } else {
                    // Envoi de messages aux clients SSE
                    if (poll_fds[i].revents & POLLOUT) {
                        if (!last_message.empty()) {
                            std::string event = "data: " + last_message + "\n\n";
                            send(poll_fds[i].fd, event.c_str(), event.size(), 0);
                        }
                    }
                }
            }
        }
    }
}

int main() {
    run_server();
    return 0;
}

