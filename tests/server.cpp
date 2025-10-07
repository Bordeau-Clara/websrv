#include <string>
#include <vector>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>



int main() {

	const size_t bufferSize = 1024;
	const char *portNumber = "8080";
	const int backlog = 2;
	int serverSocket;
	struct serverSocket;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	struct addrinfo *record;
	struct addrinfo *results;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(NULL, portNumber, &hints, &results) != 0) {
		std::cerr << "Can't get server's adress" << std::endl;
		exit(-1);
	}

	for (record = results; record != NULL; record = record->ai_next) {
		serverSocket = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
		if (serverSocket == -1)
			continue;
		int enable = 1;
		setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
		if (bind(serverSocket, record->ai_addr, record->ai_addrlen) == 0)
			break;
		close(serverSocket);
	}

	freeaddrinfo(results);

	if (listen(serverSocket, backlog) == -1) {
		std::cerr << "I'm deaf" << std::endl;
		exit(1);
	}

// ADD EPOLL LOGIC BLOC

    // 2. Créer une instance epoll
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    // Structure pour les événements
    struct epoll_event event, events[1024];
    event.events = EPOLLIN;
    event.data.fd = serverSocket;

    // 3. Ajouter le socket serveur à epoll
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serverSocket, &event) == -1) {
        perror("epoll_ctl: serverSocket");
        exit(EXIT_FAILURE);
    }

/*
	std::vector<int> clients;

	while (1) {

		int clientSocket;
		struct sockaddr clientAdress;
		socklen_t clientAdressLenght = sizeof(clientAdress);

		if ((clientSocket = accept(serverSocket, &clientAdress, &clientAdressLenght)) < 0) {
			std::cerr << "Aled" << std::endl;
			exit(1);
		}

		int received;
			
		char buffer[bufferSize] = {0};
		received = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (received == -1) {
			std::cerr << "I,m blind" << std::endl;
			exit(1);
		} else {
			std::cout << "Message: " << buffer << std::endl;
		}

		close(clientSocket);

	}
*/
    // Boucle principale
    while (1) {
        int nfds = epoll_wait(epoll_fd, events, 1024, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nfds; ++i) {
            // 4. Traiter les événements
            if (events[i].data.fd == serverSocket) {
                // Nouvelle connexion
                client_fd = accept(serverSocket, (struct sockaddr *)&client_addr, &client_len);
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
                printf("Nouvelle connexion acceptée\n");
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

	close(serverSocket);
	return 0;
}
