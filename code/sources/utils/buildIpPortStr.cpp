/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildIpPortStr.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 09:36:43 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "helpers.hpp"
#include <string>

std::string	buildIpPortStr(unsigned int	ip, unsigned short int port)
{
	std::string	retStr;
    // Extraction des 4 octets de l'IP (Network Byte Order)
    // On décale de 24, 16, 8 bits et on applique un masque 0xFF
    retStr.append(nbrToString((ip >> 24) & 0xFF));
    retStr.append(".");
    retStr.append(nbrToString((ip >> 16) & 0xFF));
    retStr.append(".");
    retStr.append(nbrToString((ip >> 8) & 0xFF));
    retStr.append(".");
    retStr.append(nbrToString(ip & 0xFF));
	// ajout du port a la fin
    retStr.append(":");
    retStr.append(nbrToString(port));
	return (retStr);
}
