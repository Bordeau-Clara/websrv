/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strNext.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aykrifa <aykrifa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 09:28:40 by aykrifa           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

std::string	strNext(const std::string &str)
{
    const char char_max = '~'; 
    const char char_min = '!'; 

	std::string	next(str);

    if (next.empty() || next.at(next.size() - 1) == char_max)
		next.append(1, char_min);
	else
		next.at(next.size() - 1)++;
	return (next);
}
//
// int main (int argc, char *argv[])
// {
// 	std::string	str("!");
//
// 	for (int i = 0; i != 1000; i++)
// 	{
// 		std::cout << str << std::endl;
// 		str = strNext(str);
// 	}
// }
