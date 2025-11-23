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

#include <vector>
class	Location;

class	Server
{
	public:
		const std::vector<Location>	&getLocations(void) const;
		void						setLocations(std::vector<Location>&);
	private:
		std::vector<Location>	_locations;
		// std::string			_
};
