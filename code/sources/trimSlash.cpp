/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trimSlash.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 15:12:28 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include <string>

void	trimSlash(std::string &str)
{
    size_t pos = 0;
    while ((pos = str.find("//", pos)) != std::string::npos)
	{
        str.replace(pos, 2, "/");
        pos += 1;
    }
}
