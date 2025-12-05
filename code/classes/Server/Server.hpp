/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 15:26:42 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
class	Location;

class	Server
{
	public:
		Server(void);
		~Server(void);
		// Server(const Server&);
		// Server	operator=(const Server&);

	// man socket
		// The domain argument specifies a communication domain; this selects the protocol family which will be used for communication.
		static const int				socket_adress_family = AF_INET;
	// man socket
		// The socket has the indicated type, which specifies the communication semantics.
		static const int				socket_type = SOCK_STREAM;
	// man socket
		// The  protocol  specifies  a  particular protocol to be used with the socket.
		// Normally only a single protocol exists to support a particular socket type within a given protocol family, in which case
		//    protocol can be specified as 0.
		// However, it is possible that many protocols may exist, in which case a particular protocol must be specified in this manner.
		// The protocol number to use is  specific to the “communication domain” in which communication is to take place;
		// see protocols(5).
		static const int				socket_protocol = 0;
	//
		// max connection until listen will fail
		static const int				socket_max_connection = SOMAXCONN;

		const std::map<std::string, Location>	&getLocations(void) const;
		void									setLocationsMap(const std::map<std::string, Location>&);
		void									addLocations(const std::string&, const Location&);

		unsigned short int						getPort(void) const;
		void									setPort(unsigned short int);

		unsigned int							getInterface(void) const;
		void									setInterface(unsigned int);

		int										getFd(void) const;
		void									setFd(const int);
	
		void									startListen(void);
	private:
		std::map<std::string, Location>	_locations;
		unsigned short int				_port;
		unsigned int					_interface;
		int								_fd;
};

std::ostream	&operator<<(std::ostream &lhs, const Server &rhs);
void			printServerInfo(const std::vector<Server> &servers);
