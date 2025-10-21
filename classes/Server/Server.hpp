/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 12:11:18 by aykrifa           #+#    #+#             */
/*   Updated: 2025/10/21 10:07:26 by aykrifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>

class	Location;

class	Server
{
	public:

		Server(void);
		Server(const Server& src);
		~Server();

		Server& operator=(const Server& server);

	private:
		std::string				_name;
		std::vector<Location>	_location;
};
