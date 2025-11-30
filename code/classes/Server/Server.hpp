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

#include <map>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
class	Location;

class	Server
{
	public:
		Server(void);
		Server(const Server&);
		~Server(void);
		Server	operator=(const Server&);

	public:
		const std::map<std::string, Location>	&getLocations(void) const;
		void									setLocationsMap(const std::map<std::string, Location>&);
		void									addLocations(std::string&, Location&);
	private:
		std::map<std::string, Location>	_locations;

	public:
		unsigned short int						getPort(void) const;
		void									setPort(unsigned short int);
	private:
		unsigned short int				_port;

	public:
		unsigned int							getInterface(void) const;
		void									setInterface(unsigned int);
	private:
		unsigned int					_interface;
	
	public:
		static const int				adress_family = AF_INET;
};
