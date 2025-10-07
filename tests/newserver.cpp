#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#define MAX_EVENTS 10

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // 1. Créer le socket serveur
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configurer le socket pour le réutiliser rapidement
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Lier le socket à une adresse et un port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

	std::cout << "htons: " << server_addr.sin_port << std::endl;

    // Écouter les connexions entrantes
    if (listen(server_fd, SOMAXCONN) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // 2. Créer une instance epoll
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    // Structure pour les événements
    struct epoll_event event, events[MAX_EVENTS];
    event.events = EPOLLIN;
    event.data.fd = server_fd;

    // 3. Ajouter le socket serveur à epoll
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("epoll_ctl: server_fd");
        exit(EXIT_FAILURE);
    }

    // Boucle principale
    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nfds; ++i) {
            // 4. Traiter les événements
            if (events[i].data.fd == server_fd) {
                // Nouvelle connexion
                client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
                if (client_fd == -1) {
                    perror("accept");
                    continue;
                }

                // Configurer le socket client en non-bloquant
                fcntl(client_fd, F_SETFL, O_NONBLOCK);

                // Ajouter le socket client à epoll
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
                    perror("epoll_ctl: client_fd");
                    close(client_fd);
                    continue;
                }
                printf("Nouvelle connexion acceptée: %d\n", client_fd);
            } else {
                // Données prêtes à être lues sur un socket client
                char buffer[1024];
                ssize_t count = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                if (count <= 0) {
                    // Fermeture ou erreur de connexion
                    close(events[i].data.fd);
                    printf("Connexion fermée\n");
                } else {
                    // Traiter les données reçues (ex: envoyer une réponse)
                    send(events[i].data.fd, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!", 48, 0);
                    close(events[i].data.fd);
                }
            }
        }
    }

    close(server_fd);
    return 0;
}

