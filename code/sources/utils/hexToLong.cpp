/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HexToLong.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 11:05:51 by aykrifa           #+#    #+#             */
/*   Updated: 2025/12/23 11:07:04 by aykrifa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <cstring>
#include <string>

#include "FileStream.hpp"
extern FileStream	streams;

unsigned long hexToLong(std::string line)
{
	unsigned long chunk_size;
	const char* semicolon = std::strchr(line.data(), ';');

	chunk_size = std::strtoul(line.data(), semicolon ? (char**)&semicolon : NULL, 16);

		streams.get(LOG_REQUEST) << "[CHUNK SIZE]" << std::endl
			<< "octet to read: " << chunk_size
			<< std::endl;
	return chunk_size;
}
