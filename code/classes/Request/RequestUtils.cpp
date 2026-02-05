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
	for (int i = 0; str[i] !=':'; i++)
	{
		if (str[i] != '-' && !(str[i] >= 'A' && str[i] <= 'Z') && !(str[i] >= 'a' && str[i] <= 'z'))
		{
			streams.get(LOG_REQUEST) << "[ERROR]" << std::endl
				<< "Wrong character in field :" << str[i]
				<< std::endl;
			return -1;
		}
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
	if (index <= 0)
		return -1;
	if (index > 207)
	{
		return 0;
	}
	for (int i = 0; i < 3; i++)
	{
		if (!Request::fields[index][i].empty() && !Request::fields[index][i].compare(str))
			return index + i;
	}
	return 0;
}
