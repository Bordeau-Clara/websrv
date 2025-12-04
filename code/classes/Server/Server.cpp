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
	_port(DEFAULT_SERVER_PORT),
	_interface(DEFAULT_SERVER_INTERFACE),
	_fd(-1)
{}

Server::Server(const Server &copy):
	_locations(copy._locations),
	_port(copy.getPort()),
	_interface(copy._interface),
	_fd(getFd())
{}

Server::~Server(void)
{}

Server	Server::operator=(const Server &rhs)
{
	if (&rhs == this)
		return (*this);
	this->setLocationsMap(rhs.getLocations());
	this->setPort(rhs.getPort());
	this->setInterface(rhs.getInterface());
	this->setFd(rhs.getFd());
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
	streams.print(LOG_SERVER) << *this << SEPARATOR << std::endl;
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

void	printServerInfo(const std::vector<Server> servers)
{
	int	i = 0;
	for (std::vector<Server>::const_iterator it = servers.begin(); it != servers.end(); it++)
	{
		streams.print(LOG_SERVER) << "SERVER " << i << ":" << std::endl << *it << std::endl;
	}
}
