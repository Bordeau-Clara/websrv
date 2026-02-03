/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hexToLong.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 11:05:51 by aykrifa           #+#    #+#             */
/*   Updated: 2026/02/03 10:21:20 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <cstring>
#include <string>

#include "FileStream.hpp"
extern FileStream	streams;

// const std::string hex = "0123456789abcdefABCDEF";
// unsigned long hexToLong(std::string line)
// {
// 	unsigned long chunk_size;
// 	const char* semicolon = std::strchr(line.data(), ';');
//
// 	//verifier que tout les chiffre font parti de la base 16 (a tester)
// 	for(std::string::iterator it = line.begin(); *it != ';' && it != line.end(); ++it)
// 	{
// 		if (hex.find(*it) == std::string::npos)
// 		{
// 			//error
// 		}
// 	}
//
// 	chunk_size = std::strtoul(line.data(), semicolon ? (char**)&semicolon : NULL, 16);
//
// 		streams.get(LOG_REQUEST) << "[CHUNK SIZE]" << std::endl
// 			<< "octet to read: " << chunk_size
// 			<< std::endl;
// 	return chunk_size;
// }
