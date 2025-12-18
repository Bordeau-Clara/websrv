/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nbrToString.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 14:15:21 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>

std::string	nbrToString(size_t nbr)
{
	std::ostringstream oss;
	oss << nbr;
	std::string str = oss.str();
	return str;
}
