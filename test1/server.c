/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:48:19 by cbordeau          #+#    #+#             */
/*   Updated: 2025/10/11 13:22:24 by cbordeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	printf("Server started.\n");

	const size_t bufferSize = 1000;
	const char *portNumber = "6969";
	const int backlog = 1;
	
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));


	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	struct addrinfo *results;

	if ((getaddrinfo(NULL, portNumber, &hints, &results)) != 0)
	{
		perror("Failed to translate client socket.");
		exit(EXIT_FAILURE);
	}
	printf("Server socket translated into results.\n");

	int serverSocket;
	struct addrinfo *record;
	for (record = results; record != NULL; record = record->ai_next)
	{
		serverSocket = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
		if (serverSocket == -1)
			continue;
		int enable = 1;
		setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
		if (bind(serverSocket, record->ai_addr, record->ai_addrlen) == 0)
			break;
		close(serverSocket);
	}
	if (record == NULL)
	{
		perror("Failed to create or onnet cliaent socket.\n");
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(results);

	printf("Server socket created and bound.\n");

	if (listen(serverSocket, backlog) == -1)
	{
		perror("Failed to start server socket listen.");
		exit(EXIT_FAILURE);
	}

	printf("Server started listening.\n");

	while (1)
	{
		printf("Server still running.\n");

		int clientSocket;
		struct sockaddr clientAddress;
		socklen_t clientAddressLenght = sizeof(clientAddress);

		if ((clientSocket = accept(serverSocket, &clientAddress, &clientAddressLenght)) < 0)
		{
			perror("Failed to accept client socket.");
			exit(EXIT_FAILURE);
		}

		printf("Client socket accepted.\n");
		char buffer[bufferSize];

		if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1)
		{
			perror("Failed to receive message");
			exit(EXIT_FAILURE);
		}
		printf("Message received\n");
		char message[] = "Hi from server\n";
		if (send(clientSocket, message, strlen(message), 0) == -1)
		{
			perror("Failed to perform cognitive recalibrication.");
			exit(EXIT_FAILURE);
		}
		printf("Response sent\n");
		close(clientSocket);
		printf("Client socket closed.\n");
	}
	return 0;
}
