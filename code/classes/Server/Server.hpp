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
		const std::map<std::string, Location>	&getLocations(void) const;
		void						setLocationsMap(std::map<std::string, Location>&);
		void						addLocations(std::string&, Location&);
	
	private:
		std::map<std::string, Location>	_locations;
		unsigned short int	_port;
		unsigned int		_interface;
		static const int	socket_in_family = AF_INET;
};
