/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 15:26:33 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Location.hpp"
#include <stdexcept>
#include "FileStream.hpp"
#include "logfiles.hpp"

extern FileStream	streams;

static const unsigned int				DEFAULT_SERVER_INTERFACE = 0;
static const unsigned short				DEFAULT_SERVER_PORT = 8080;
Server::Server(void):
	Event(SRV),
	_port(DEFAULT_SERVER_PORT),
	_interface(DEFAULT_SERVER_INTERFACE),
	_fd(-1)
{}

#include <unistd.h>
Server::~Server(void)
{
	if (_fd != -1)
		close (_fd);
}

Server::Server(const Server &copy):
	Event(SRV),
	_locations(copy._locations),
	_port(copy.getPort()),
	_interface(copy._interface),
	_fd(copy.getFd())
{
	if (_fd != -1)
		throw (std::runtime_error("FD IS NOT -1"));
}

Server	Server::operator=(const Server &rhs)
{
	if (&rhs == this)
		return (*this);
	this->setLocationsMap(rhs.getLocations());
	this->setPort(rhs.getPort());
	this->setInterface(rhs.getInterface());
	this->setFd(rhs.getFd());
	if (_fd != -1)
		throw (std::runtime_error("FD IS NOT -1"));
	return (*this);
}

const std::map<std::string, Location>	&Server::getLocations(void) const
{
	return (this->_locations);
}
// copy map into object
void	Server::setLocationsMap(const std::map<std::string, Location>	&locations)
{
	this->_locations = locations;
}
// take string as key and try to push a location,
// if key alaready exists, throw
void	Server::addLocations(const std::string &key, const Location &value)
{
	if (this->_locations.find(key) != this->_locations.end())
		throw (std::runtime_error("URL already exist"));
	this->_locations[key] = Location(value);
}

unsigned short int	Server::getPort(void) const
{
	return (this->_port);
}

void	Server::setPort(unsigned short int port)
{
	this->_port = port;
}

unsigned int	Server::getInterface(void) const
{
	return (this->_interface);
}

void	Server::setInterface(unsigned int interface)
{
	this->_interface = interface;
}

int		Server::getFd(void) const
{
	return (this->_fd);
}

void	Server::setFd(const int fd)
{
	this->_fd = fd;
}

#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h> // Souvent inclus par arpa/inet.h ou netinet/in.h
#include <netinet/in.h>

std::string intToIPv4(uint32_t ip_int) {
    // 1. Déclarer la structure pour l'adresse réseau
    struct in_addr ip_addr;
    
    // 2. Assigner l'entier à la structure. 
    // On assume que ip_int est déjà en ordre "network byte order".
    // Si ce n'est pas le cas (i.e., si elle a été lue avec un 'htonl' manquant),
    // il faudrait d'abord l'appeler : ip_addr.s_addr = htonl(ip_int);
    // Mais en général, elle est déjà en ordre réseau si elle vient d'une structure socket.
    ip_addr.s_addr = htonl(ip_int);

    // 3. Buffer pour stocker la chaîne de caractères (taille max IPv4 est 16)
    char presentation_buffer[INET_ADDRSTRLEN];

    // 4. Conversion : Network To Presentation
    const char* result = inet_ntop(
        AF_INET,                    // Famille d'adresse : IPv4
        &(ip_addr.s_addr),          // Pointeur vers l'adresse binaire (uint32_t)
        presentation_buffer,        // Buffer de destination
        INET_ADDRSTRLEN             // Taille du buffer
    );

    if (result == NULL)
	{
        // En cas d'erreur
        return "Erreur de conversion";
    }

    // 5. Retourner la chaîne de caractères convertie
    return std::string(result);
}

#include <iostream>
#include <fcntl.h>
void	Server::startListen(void)
{

    // 1. Créer le socket serveur
	/* Create a new socket of type TYPE in domain DOMAIN, using
   protocol PROTOCOL.  If PROTOCOL is zero, one is chosen automatically.
   Returns a file descriptor for the new socket, or -1 for errors.  */
    this->setFd(socket(this->socket_adress_family, this->socket_type, this->socket_protocol));
    if (this->getFd() == -1)
	{
		throw (std::runtime_error("socket"));
    }
    // Configurer le socket pour le réutiliser rapidement
    int opt = 1;
    setsockopt(this->getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // bind socket to interface port
    struct sockaddr_in server_addr;
    server_addr.sin_family = this->socket_adress_family;
    server_addr.sin_addr.s_addr = htonl(this->getInterface());
    server_addr.sin_port = htons(this->getPort());
    if (bind(this->getFd(), (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		throw (std::runtime_error("bind"));
    }

	//listen
    if (listen(this->getFd(), this->socket_max_connection) == -1)
	{
		throw (std::runtime_error("listen"));
    }

	// Configurer le socket server en non-bloquant
	fcntl(this->_fd, F_SETFL, O_NONBLOCK);
	std::cout << "listening " << getPort() << "on interface: " << intToIPv4(getInterface()) << std::endl;
	streams.get(LOG_SERVER) << *this << SEPARATOR << std::endl;
}

std::ostream	&operator<<(std::ostream &lhs, const Server &rhs)
{
	lhs << "listen="
		<< rhs.getPort()
		<< ":"
		<< rhs.getInterface()
		<< std:: endl;
	lhs << "fd="
		<< rhs.getFd()
		<< std::endl;
	{
		const std::map<std::string, Location> &locations = rhs.getLocations();
		for (std::map<std::string,Location>::const_iterator it = locations.begin(); it != locations.end(); it++)
		{
			lhs << "location "
				<< it->first
				<< std::endl;
			lhs << it->second
				<< std::endl;
		}
	}
	return (lhs);
}

void	printServerInfo(const std::vector<Server> &servers)
{
	int	i = 0;
	for (std::vector<Server>::const_iterator it = servers.begin(); it != servers.end(); it++)
	{
		streams.get(LOG_SERVER) << "SERVER " << i << ":" << std::endl << *it << std::endl;
	}
}
