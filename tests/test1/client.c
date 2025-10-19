/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:48:26 by cbordeau          #+#    #+#             */
/*   Updated: 2025/10/09 08:51:38 by aykrifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	printf("Client started.\n");

	const char *hostName = "127.0.0.1";
	const char *portNumber = "6969";
	int clientSocket;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	struct addrinfo *head;
	struct addrinfo *current;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if ((getaddrinfo(hostName, portNumber, &hints, &head)) != 0)
	{
		perror("Failed to translate client socket.");
		exit(EXIT_FAILURE);
	}

	printf("Client socket translated.\n");

	for (current = head; current != NULL; current = current->ai_next)
	{
		clientSocket = socket(current->ai_family, current->ai_socktype, current->ai_protocol);
		if (clientSocket == -1)
			continue;
		if (connect(clientSocket, current->ai_addr, current->ai_addrlen)!= -1)
			break;
		close(clientSocket);
	}
	if (current == NULL)
	{
		perror("Failed to create or connect client socket.");
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(head);
	printf("Client socket created and connected.\n");
//
	char buffer[1000];

	if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1)
	{
		perror("Failed to receive message");
		exit(EXIT_FAILURE);
	}
	printf("---------------RECEIVED MESSAGE------------------\n %s\n-------------END OF MESSAGE-----------\n", buffer);
//
	/*char *message = "Like, subscribe, comment, send money.\n#SubliminalMessagingDefinitelyWorks\n";*/
	/**/
	/*if (send(clientSocket, message, strlen(message), 0) == -1)*/
	/*{*/
	/*	perror("Failed to perform cognitive recalibrication.");*/
	/*	exit(EXIT_FAILURE);*/
	/*}*/
	/*else*/
	/*	printf("Subliminal masseage has been planted\n");*/

	close(clientSocket);
	printf("Client socket close.\n");
}

//gcc -o client client.c
