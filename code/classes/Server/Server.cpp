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

const std::vector<Location>	&Server::getLocations(void) const
{
	return (this->_locations);
}

void	Server::setLocations(std::vector<Location>	&locations)
{
	this->_locations = locations;
}
