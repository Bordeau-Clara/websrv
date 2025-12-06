/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 17:23:59 by cbordeau          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "colors.hpp"

int	find_type(std::string str)
{
	int index = 0;
	// std::cout << str << std::endl;
	for (int i = 0; str[i] !=':'; i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			index += str[i] - 97;
		}
		else
			index += str[i];
	}
	if (!str.empty())
		str.resize(str.size() - 1);
	std::cout << "index is " << index << " str is " << str << std::endl;
	if (index <= 0)
		return -1;
	if (index > 207)
	{
		std::cout << RED << "Wrong index is " << index << WHITE << std::endl;
		return 0;
	}
	for (int i = 0; i < 3; i++)
	{
	  // std::cout << "Field at index is "<< HeaderParsing::fields[index][i] << std::endl;
		if (!Request::fields[index][i].empty() && !Request::fields[index][i].compare(str))
			return index + i;
	}
	return 0;
	// should not return 0 if not in fct tab but in field tab so it doesnt invalidate valid headers that should not be parsed
}

unsigned long hexToLong(std::string line)
{
	unsigned long chunk_size;
	const char* semicolon = std::strchr(line.data(), ';');

	chunk_size = std::strtoul(line.data(), semicolon ? (char**)&semicolon : NULL, 16);

	std::cout << "Chunk-size is :" << chunk_size << std::endl;
	return chunk_size;
}
