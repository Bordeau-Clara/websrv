/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMap.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <bordeau@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 15:44:11 by cbordeau          #+#    #+#             */
/*   Updated: 2025/10/19 15:49:17 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <exception>
#include <iostream>
#include <map>
#include <string>
int main()
{
	std::map<int, std::string> tab;
	tab[404] = "error 404";
	tab[405] = "error 405";
	tab[406] = "error 406";
	tab[407] = "error 407";

	try
	{
		std::cout << tab[10] << std::endl;
		std::cout << "Success" << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "Error" << std::endl;
	}
}
